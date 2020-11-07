//
//  CGRenderer.m
//  metal_demo
//
//  Created by william on 2020/11/6.
//

#import "CGRenderer.h"
#import "CGShaderTypes.h"

@implementation CGRenderer
{
    id<MTLDevice> _device;
    // 渲染管道
    id<MTLRenderPipelineState> _piplineState;
    // 渲染命令
    id<MTLCommandQueue> _commandQueue;
    // 视图的大小
    vector_uint2 _viewportSize;
}
typedef struct _Color
{
    float red;
    float green;
    float blue;
    float alpha;
}Color;

-(id)initWithMetalKitView:(MTKView *)mtkView
{
    if (self = [super init])
    {
        NSError *error = NULL;
        _device = mtkView.device;
        _commandQueue = [_device newCommandQueue];
        // 从bundle中获取.metal文件
        id<MTLLibrary> defaultLibrary = [_device newDefaultLibrary];
        // 从库中加载顶点函数
        id<MTLFunction> vertexFunction = [defaultLibrary newFunctionWithName:@"vertexShader"];
        // 从库中加载片元函数
        id<MTLFunction> fragmentFunction = [defaultLibrary newFunctionWithName:@"fragmentShader"];
        // 配置用于创建管道状态的管道
        MTLRenderPipelineDescriptor *pipelineDescriptor = [[MTLRenderPipelineDescriptor alloc] init];;
        pipelineDescriptor.label = @"Simple pipeline";
        // 可编程函数，用于处理渲染过程中的各个顶点
        pipelineDescriptor.vertexFunction = vertexFunction;
        // 可编程函数，用于处理渲染过程中的各个片元
        pipelineDescriptor.fragmentFunction = fragmentFunction;
        pipelineDescriptor.colorAttachments[0].pixelFormat = mtkView.colorPixelFormat;
        // 同步创建并且返回渲染管线状态对象
        _piplineState = [_device newRenderPipelineStateWithDescriptor:pipelineDescriptor error:&error];
        // 判断是否返回了管线状态对象
        if (!_piplineState)
        {
            NSLog(@"failed to created pipeline state, error %@", error);
            return nil;
        }
        _commandQueue = [_device newCommandQueue];
    }
    return self;
}


#pragma mark - MTKViewDelegate methods

- (void)mtkView:(MTKView *)view drawableSizeWillChange:(CGSize)size
{
    _viewportSize.x = size.width;
    _viewportSize.y = size.height;
}



- (void)drawInMTKView:(nonnull MTKView *)view
{
    // 顶点数据， 颜色数据
    static const CGVertex triangleVerices[] = 
    {
        {{250, -250}, {1, 0, 0, 1}},
        {{-250,-250}, {0, 1, 0, 1}},
        {{  0,  250}, {0, 0, 1, 1}}
    };
    
    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"CommandBuffer";
    
    // 渲染描述符
    MTLRenderPassDescriptor *renderPassDescriptor = view.currentRenderPassDescriptor;

    if (renderPassDescriptor != nil)
    {
        // CommandEncoder 对象
        id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
        renderEncoder.label = @"RenderEncoder";

        MTLViewport viewport = { 0.0, 0.0, _viewportSize.x, _viewportSize.y, -1.0, 1.0};
        [renderEncoder setViewport:viewport];
        // glUseProgram
        [renderEncoder setRenderPipelineState:_piplineState];

        [renderEncoder setVertexBytes:triangleVerices length:sizeof(triangleVerices) atIndex:CGVertexInputIndexVertices];
        [renderEncoder setVertexBytes:&_viewportSize length:sizeof(_viewportSize) atIndex:CGVertexInputIndexViewportSize];
        [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
        [renderEncoder endEncoding];
        // 显示到可绘制的View
        [commandBuffer presentDrawable:view.currentDrawable];
    }
    // 渲染命令结束之后，提交给GPU之后才能显示
    [commandBuffer commit];
    
}


@end
