//
//  GLView.h
//  opengl_demo
//
//  Created by william on 2020/11/7.
//

#import <UIKit/UIKit.h>


#define GLERROR_CHECK() \
{ \
GLenum glError = glGetError(); \
if (glError != GL_NO_ERROR) {\
printf("glGetError() = %i (0x%.8x) in filename = %s, line  = %i\n", glError, glError, __FILE__ , __LINE__); \
} \
}

NS_ASSUME_NONNULL_BEGIN

@interface GLView : UIView

@end

NS_ASSUME_NONNULL_END
