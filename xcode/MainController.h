
#import <Cocoa/Cocoa.h>
#import <OpenGL/OpenGL.h>

@class Scene;

@interface MainController : NSResponder
{
	Scene *scene;
	BOOL isAnimating;
	CFAbsoluteTime renderTime;
}

- (IBAction) goFullScreen:(id)sender;
- (void) goWindow;
- (Scene*) scene;
- (CFAbsoluteTime) renderTime;
- (void) setRenderTime:(CFAbsoluteTime)time;
- (void) setMouseMoved:(NSPoint)pos;
- (void) enterWindow;
- (void) exitWindow;
- (void) mouseUp:(NSEvent *)theEvent;
- (void) mouseDown:(NSEvent *)theEvent;

@end
