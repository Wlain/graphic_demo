//
//  CGRenderer.h
//  metal_demo
//
//  Created by william on 2020/11/6.
//

#import <MetalKit/MetalKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface CGRenderer : NSObject<MTKViewDelegate>

-(id)initWithMetalKitView:(MTKView *)mtkView;

@end

NS_ASSUME_NONNULL_END
