//
// Created by william on 2020/11/15.
//
#if defined(TARGET_IOS) || defined(TARGET_TVOS)
#import <UIKit/UIKit.h>
#import <TargetConditionals.h>
#import "AAPLAppDelegate.h"
#else
#import <Cocoa/Cocoa.h>
#endif

int main(int argc, const char * argv[]) {
    return NSApplicationMain(argc, argv);
}
