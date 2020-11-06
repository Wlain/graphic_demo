//
//  CGRenderer.m
//  metal_demo
//
//  Created by william on 2020/11/6.
//

#import "CGRenderer.h"

@implementation CGRenderer
{
    id<MTLDevice> _device;
    id<MTLCommandQueue> _commandQueue;
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
        _device = mtkView.device;
        _commandQueue = [_device newCommandQueue];
    }
    return self;
}


- (Color)makeCycleColor
{
    static BOOL growing = YES;
    static NSUInteger primaryChannel = 0;
    static float colorChannels[] = {1.0, 0.0, 0.0, 1.0};
    const float dynamicColorRate = 0.015f;
    
    if (growing)
    {
        // 动态颜色通道
        NSUInteger dynamicChannelsIndex = (primaryChannel + 1) % 3;
        colorChannels[dynamicChannelsIndex] += dynamicColorRate;
        if (colorChannels[dynamicChannelsIndex] >= 1.0)
        {
            growing = NO;
            primaryChannel = dynamicChannelsIndex;
        }
    }
    else
    {
        // 动态颜色通道
        NSUInteger dynamicChannelsIndex = (primaryChannel + 2) % 3;
        colorChannels[dynamicChannelsIndex] -= dynamicColorRate;
        if (colorChannels[dynamicChannelsIndex] <= 0.0)
        {
            growing = YES;
        }
    }
    
    Color color;
    color.red   = colorChannels[0];
    color.green = colorChannels[1];
    color.blue  = colorChannels[2];
    color.alpha = colorChannels[3];
    return color;
}

#pragma mark - MTKViewDelegate methods


- (void)drawInMTKView:(nonnull MTKView *)view
{
    // 获取颜色值
    Color color = [self makeCycleColor];
    view.clearColor = MTLClearColorMake(color.red, color.green, color.blue, color.alpha);
    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"CommandBuffer";
    
    // 渲染描述符
    MTLRenderPassDescriptor *renderPassDescriptor = view.currentRenderPassDescriptor;
    
    if (renderPassDescriptor != nil)
    {
        // CommandEncoder 对象
        id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
        renderEncoder.label = @"RenderEncoder";
        [renderEncoder endEncoding];
        // 显示到可绘制的View
        [commandBuffer presentDrawable:view.currentDrawable];
    }
    // 渲染命令结束之后，提交给GPU之后才能显示
    [commandBuffer commit];
    
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size
{
    
}

@end
