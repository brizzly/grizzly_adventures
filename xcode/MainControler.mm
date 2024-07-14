
#import "MainController.h"
#import "Scene.h"
#import "KInput.h"

@implementation MainController

- (id) init
{
	if(self = [super init])
	{
		[self performSelector:@selector(startRendering) withObject:nil afterDelay:1.0];
	}
	return self;
}

- (void) startRendering
{
	scene = [[Scene alloc] init];
/*
	glfwSetInputMode([scene getWindow], GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported()) {
		glfwSetInputMode([scene getWindow], GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}*/
	glfwSetCursorPosCallback([scene getWindow], cursor_position_callback);
	glfwSetMouseButtonCallback([scene getWindow], mouse_button_callback);
	
	if(scene) {
		[scene render];
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if(action == GLFW_PRESS) {
			KInput::setScreenPressed(1, KInput::getMouseX(), KInput::getMouseY());
		}
		else if(action == GLFW_RELEASE) {
			KInput::setScreenReleased();
		}
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	KInput::setScreenMoving(xpos, ypos);
	
	/*
	if(instance->mouseX == -1) {
		instance->mouseX = (int)xpos;
	}
	if(instance->mouseY == -1) {
		instance->mouseY = (int)ypos;
	}

	instance->mouseOffsetX = instance->mouseX - (int)xpos;
	instance->mouseOffsetY = instance->mouseY - (int)ypos;

	instance->mouseX = (int)xpos;
	instance->mouseY = (int)ypos;
	*/
	
	//printf("Cursor %d %d\n", (int)xpos, (int)ypos);
}

- (void) awakeFromNib
{
}

- (void) dealloc
{
	[scene release];
	[super dealloc];
}

- (Scene*) scene
{
	return scene;
}

- (CFAbsoluteTime) renderTime
{
	return renderTime;
}

- (void) setRenderTime:(CFAbsoluteTime)time
{
	renderTime = time;
}

- (void) startAnimation
{
	if(scene) {
		[scene render];
	}
}

- (void) stopAnimation
{
}

- (void) toggleAnimation
{
}

- (void) mouseUp:(NSEvent *)theEvent
{
	KInput::setScreenReleased();
}

- (void) mouseDown:(NSEvent *)theEvent
{
}

- (void) setMouseMoved:(NSPoint)pos
{
}

- (void) enterWindow
{
	KInput::hidePointer();
}

- (void) exitWindow
{
	KInput::showPointer();
}

- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
    return YES;
}


@end
