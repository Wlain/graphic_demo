//
//  GLView.m
//  opengl_demo
//
//  Created by william on 2020/11/7.
//

#import "GLView.h"
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

@interface GLView()
@property(nonatomic, strong)CAEAGLLayer *glLayer;
@property(nonatomic, strong)EAGLContext *context;;
@property(nonatomic, assign)GLuint colorRbo;
@property(nonatomic, assign)GLuint colorFbo;
@property(nonatomic, assign)GLuint program;
@property(nonatomic, assign)GLuint vbo;
@end



@implementation GLView

-(void)layoutSubviews
{
    [self setupLayer];
    
    [self setupContext];
    
    [self deleteBuffer];
    
    [self setupRenderBuffer];
    
    [self setFramebuffer];
    
    [self render];
}

-(void)render
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    GLERROR_CHECK();
    GLfloat scale = [[UIScreen mainScreen] scale];
    
    glViewport(self.frame.origin.x * scale, self.frame.origin.y * scale, self.frame.size.width * scale, self.frame.size.height * scale);
    // 获取顶点着色器，片元着色器程序文件的位置
    NSString* vertFile = [[NSBundle mainBundle] pathForResource:@"base" ofType:@"vert"];
    NSString* fragFile = [[NSBundle mainBundle] pathForResource:@"base" ofType:@"frag"];
    
    if (self.program)
    {
        glDeleteProgram(self.program);
        self.program = 0;
    }
    
    self.program = [self createProgramWith:vertFile fragFile:fragFile];
    glUseProgram(self.program);
    if (self.program == 0)
    {
        return;
    }
    // 判断顶点缓存是否为空，如果为空则申请一个缓存标识符
    if (self.vbo == 0)
    {
        glGenBuffers(1, &_vbo);
    }
    
    // 顶点数组
    static const float vertices[] =
    {
        0.5f,  -0.25f, 0.0f,  1.0f, 0.0f, 0.0f,
        -0.5f, -0.25f, 0.0f,  0.0f, 1.0f, 0.0f,
        0.0f,   0.25f, 0.0f,  0.0f, 0.0f, 1.0f,
    };
    GLERROR_CHECK();
    // 处理顶点数据
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    GLuint positionLoc = glGetAttribLocation(self.program, "aPosition");
    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)0);
    glEnableVertexAttribArray(positionLoc);
    
    GLuint colorLoc = glGetAttribLocation(self.program, "aColor");
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)(0 + 3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(colorLoc);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    [self.context presentRenderbuffer:GL_RENDERBUFFER];
}

// 设置frameBuffer
-(void)setFramebuffer
{
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    self.colorFbo = fbo;
    glBindFramebuffer(GL_FRAMEBUFFER, self.colorFbo);
    // 将colorFbo装配到GL_COLOR_ATTACHMENT0 附着点上
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, self.colorRbo);
}

// 设置renderBuffer
-(void)setupRenderBuffer
{
    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    self.colorRbo = rbo;
    glBindRenderbuffer(GL_RENDERBUFFER, self.colorRbo);
    // frame buffer仅仅是管理者，不需要分配空间，render buffer的存储空间的分配，对于不同的render buffer，使用不用的
    // API进行分配，而只有分配空间的时候，render buffer句柄才确定其类型
    [self.context renderbufferStorage:GL_RENDERBUFFER fromDrawable:self.glLayer];
}


// 清空缓冲区
-(void)deleteBuffer
{
    /* frame buffer 相当于renderbuffer的管理者。frame buffer object 即FBO，
     常用语离屏渲染缓存等
     render buffer 又可以分为三类, color Buffer, depthBuffer, stencilBuffer
     */
    glDeleteBuffers(1, &_colorRbo);
    _colorRbo = 0;
    glDeleteBuffers(1, &_colorFbo);
    _colorFbo = 0;
}

// 设置上下文
-(void)setupContext
{
    EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES2;
    EAGLContext *context = [[EAGLContext alloc] initWithAPI:api];
    if(!context)
    {
        NSLog(@"Create context failed");
        return;
    }
    
        // 设置当前上下文
    if(![EAGLContext setCurrentContext:context])
    {
        NSLog(@"Set current Context failed");
        return;
    }
    self.context = context;
}



// 设置图层
-(void)setupLayer
{
    self.glLayer = (CAEAGLLayer*)self.layer;
    [self setContentScaleFactor:[[UIScreen mainScreen]scale]];
    
    // CAlayer默认是透明的，必须将它设置为不透明才能可见
    self.glLayer.opaque = YES;
    
    self.glLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:NO],kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
}

#pragma mark --Shader

-(GLuint)loadShaderWith:(NSString *)vertexShader fragmentShader:(NSString *)fragmentShader
{
    // 创建2个临时的变量，vertShader, fragShader
    GLuint program = glCreateProgram();
    GLuint vertShader = [self createShader:GL_VERTEX_SHADER srcPath:vertexShader];
    GLuint fragShader = [self createShader:GL_VERTEX_SHADER srcPath:fragmentShader];
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    
    glDeleteProgram(vertShader);
    glDeleteProgram(fragShader);
    return program;
}

-(GLuint)createProgramWith:(NSString*)vertFile fragFile:(NSString*)fragFile
{
    GLuint vertShader = 0;
    GLuint fragShader = 0;
    GLuint program = 0;
    GLint linked = GL_FALSE;
    
    vertShader = [self createShader:GL_VERTEX_SHADER srcPath:vertFile];
    if (!vertShader)
        goto exit;
    
    fragShader = [self createShader:GL_FRAGMENT_SHADER srcPath:fragFile];
    if (!fragShader)
        goto exit;
    
    program = glCreateProgram();
    if (!program) {
        goto exit;
    }
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        printf("Could not link program");
        GLint infoLogLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen) {
            GLchar* infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog) {
                glGetProgramInfoLog(program, infoLogLen, NULL, infoLog);
                printf("Could not link program:\n%s\n", infoLog);
                free(infoLog);
            }
        }
        glDeleteProgram(program);
        program = 0;
    }
    
exit:
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    return program;
}


-(GLuint)createShader:(GLenum)shaderType srcPath:(NSString*)srcPath
{
    GLuint shader = glCreateShader(shaderType);
    if (!shader) {
        return 0;
    }
    NSError *error = nil;
    // 读取文件路径的字符串
    NSString *content = [NSString stringWithContentsOfFile:srcPath encoding:NSUTF8StringEncoding error:&error];
    const GLchar *source = (GLchar *)[content UTF8String];
    glShaderSource(shader, 1, &source, NULL);
    GLint compiled = GL_FALSE;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLogLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen > 0) {
            GLchar* infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog) {
                glGetShaderInfoLog(shader, infoLogLen, NULL, infoLog);
                printf("Could not compile %s shader:\n%s\n",
                       shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment",
                       infoLog);
                free(infoLog);
            }
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

@end
