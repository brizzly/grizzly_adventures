
#import <Cocoa/Cocoa.h>
#if defined(__APPLE__)
#include <GLFW/glfw3.h>
#endif

@class Texture;

@interface Scene : NSObject
{
	float animationPhase;
	GLFWwindow	* 	window;
	int				frame_width;
	int				frame_height;
}

- (id)init;
- (GLFWwindow*) getWindow;
- (void)setViewportRect:(NSRect)bounds;
- (void)render;
- (void)advanceTimeBy:(float)seconds;
- (void)setAnimationPhase:(float)newAnimationPhase;

@end
