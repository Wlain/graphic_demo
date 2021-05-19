//
//  ViewController.m
//  metal_learning
//
//  Created by william on 2021/5/14.
//

#import "ViewController.h"
#import "renderer.h"

@interface ViewController ()
{
    MTKView *_view;
    Renderer *_renderer;
}
@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    _view = (MTKView*)self.view;
    _view.device = MTLCreateSystemDefaultDevice();
    _view.backgroundColor = UIColor.blackColor;
    if (!_view.device)
    {
        NSLog(@"Mteal is not support on this device");
        self.view = [[UIView alloc] initWithFrame:self.view.frame];
        return;
    }
    _renderer = [[Renderer alloc] initWithMetalKitView:_view];
    if (!_renderer)
    {
        NSLog(@"MtlView init failed!");
        return;
    }
    [_renderer mtkView:_view drawableSizeWillChange:_view.bounds.size];
    _view.delegate = _renderer;
    _view.preferredFramesPerSecond = 30;
    // Do any additional setup after loading the view.
}


@end
