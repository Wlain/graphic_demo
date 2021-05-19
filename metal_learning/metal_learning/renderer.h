//
//  mtlView.h
//  metal_learning
//
//  Created by william on 2021/5/14.
//

#import <Foundation/Foundation.h>
#import <MetalKit/MetalKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface Renderer : NSObject<MTKViewDelegate>

- (id)initWithMetalKitView:(MTKView*) mtkView;

@end

NS_ASSUME_NONNULL_END
