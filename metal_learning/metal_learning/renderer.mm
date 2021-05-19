//
//  mtlView.m
//  metal_learning
//
//  Created by william on 2021/5/14.
//

#import "renderer.h"
/// Include header shared between C code here, which executes Metal API commands, and .metal files
#import "shaderTypes.h"
#import <array>

@implementation Renderer
{
    id<MTLDevice> _device;
    id<MTLCommandQueue> _commandQueue;
    id<MTLRenderPipelineState> _piplineState;
    MTLVertexDescriptor *_mtlVertexDescriptor;
}

static const Vertex s_vertexes[] = {
    {{0, 0.5}},
    {{0.5, -0.5}},
    {{-0.5, -0.5}}};

- (id)initWithMetalKitView:(MTKView*) view
{
    self = [super init];
    if (self)
    {
        _device = view.device;
        view.colorPixelFormat = MTLPixelFormatBGRA8Unorm_sRGB;
        view.sampleCount = 1;
        _commandQueue = [_device newCommandQueue];
        /// load shader
        id<MTLLibrary> defaultLibrary = [_device newDefaultLibrary];
        id<MTLFunction> vertFunction = [defaultLibrary newFunctionWithName:@"vertShader"];
        id<MTLFunction> fragFunction = [defaultLibrary newFunctionWithName:@"fragShader"];
        MTLRenderPipelineDescriptor *renderPipelineDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
        renderPipelineDescriptor.label = @"myPipeline";
        renderPipelineDescriptor.sampleCount = view.sampleCount;
        renderPipelineDescriptor.vertexFunction = vertFunction;
        renderPipelineDescriptor.fragmentFunction = fragFunction;
        renderPipelineDescriptor.colorAttachments[0].pixelFormat = view.colorPixelFormat;
        NSError *error = nil;
        _piplineState = [_device newRenderPipelineStateWithDescriptor:renderPipelineDescriptor error:&error];
        if (!_piplineState)
        {
            NSLog(@"create piplineState failed!");
        }
    }
    return self;
}

- (void)drawInMTKView:(nonnull MTKView *)view
{
    view.clearColor = MTLClearColorMake(0.0f, 0.0f, 0.0f, 1.0f);
    id<MTLCommandBuffer> commanBuffer = [_commandQueue commandBuffer];
    /// 渲染描述符
    MTLRenderPassDescriptor *renderPassDescriptor = view.currentRenderPassDescriptor;
    if (renderPassDescriptor != nil)
    {
        id<MTLRenderCommandEncoder> renderEncoder = [commanBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
        renderEncoder.label = @"renderEncoder";
        [renderEncoder setRenderPipelineState:_piplineState];
        [renderEncoder setVertexBytes:s_vertexes length:sizeof(s_vertexes) atIndex:0];
        [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
        [renderEncoder endEncoding];
        [commanBuffer presentDrawable:view.currentDrawable];
    }
    [commanBuffer commit];
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    
}

@end
