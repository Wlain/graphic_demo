//
//  ViewController.m
//  metal_demo
//
//  Created by william on 2020/11/6.
//

#import "ViewController.h"
#import "CGRenderer.h"

@interface ViewController ()
{
    MTKView *_view;
    CGRenderer *_render;
}


@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    // 获取_view
    _view = (MTKView *)self.view;
    
    _view.device = MTLCreateSystemDefaultDevice();
    
    if (!_view.device)
    {
        NSLog(@"Metal is not supported on this device");
        return;;
    }
    
    _render = [[CGRenderer alloc] initWithMetalKitView:_view];
    
    if (!_render)
    {
        NSLog(@"Renderer failed initialized");
        return;;
    }
    
    _view.delegate = _render;
    
    _view.preferredFramesPerSecond = 30;
    
}


@end
