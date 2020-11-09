//
//  ViewController.m
//  opengl_demo
//
//  Created by william on 2020/11/7.
//

#import "ViewController.h"
#import "GLView.h"

@interface ViewController ()
@property (nonatomic, strong)GLView *glview;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.glview = (GLView*)self.view;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}


@end
