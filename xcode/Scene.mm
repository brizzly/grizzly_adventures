
#import "Scene.h"
#import <OpenGL/glu.h>
#import "KInput.h"
#import "grizzly.h"

@implementation Scene


static void close_callback(GLFWwindow* window)
{
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
//	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, GLFW_TRUE);
	
	if(action == GLFW_PRESS)
	{
		if(key == GLFW_KEY_UP) {
			KInput::setKeyPressed(K_VK_UP);
		}
		if(key == GLFW_KEY_DOWN) {
			KInput::setKeyPressed(K_VK_DOWN);
		}
		if(key == GLFW_KEY_LEFT) {
			KInput::setKeyPressed(K_VK_LEFT);
		}
		if(key == GLFW_KEY_RIGHT) {
			KInput::setKeyPressed(K_VK_RIGHT);
		}
		
		if(key == GLFW_KEY_ENTER) {
			KInput::setKeyPressed(K_VK_RETURN);
		}
		if(key == GLFW_KEY_ESCAPE) {
			KInput::setKeyPressed(K_VK_ESCAPE);
		}
		if(key == GLFW_KEY_SPACE) {
			KInput::setKeyPressed(K_VK_SPACE);
		}
		if(key == GLFW_KEY_BACKSPACE) {
			KInput::setKeyPressed(K_VK_BACK);
		}
		
		if(key == GLFW_KEY_A) {
			KInput::setKeyPressed(K_VK_A);
		}
		if(key == GLFW_KEY_B) {
			KInput::setKeyPressed(K_VK_B);
		}
		if(key == GLFW_KEY_C) {
			KInput::setKeyPressed(K_VK_C);
		}
		if(key == GLFW_KEY_D) {
			KInput::setKeyPressed(K_VK_D);
		}
		if(key == GLFW_KEY_E) {
			KInput::setKeyPressed(K_VK_E);
		}
		if(key == GLFW_KEY_F) {
			KInput::setKeyPressed(K_VK_F);
		}
		if(key == GLFW_KEY_G) {
			KInput::setKeyPressed(K_VK_G);
		}
		if(key == GLFW_KEY_H) {
			KInput::setKeyPressed(K_VK_H);
		}
		if(key == GLFW_KEY_I) {
			KInput::setKeyPressed(K_VK_I);
		}
		if(key == GLFW_KEY_J) {
			KInput::setKeyPressed(K_VK_J);
		}
		if(key == GLFW_KEY_K) {
			KInput::setKeyPressed(K_VK_K);
		}
		if(key == GLFW_KEY_L) {
			KInput::setKeyPressed(K_VK_L);
		}
		if(key == GLFW_KEY_M) {
			KInput::setKeyPressed(K_VK_M);
		}
		if(key == GLFW_KEY_N) {
			KInput::setKeyPressed(K_VK_N);
		}
		if(key == GLFW_KEY_O) {
			KInput::setKeyPressed(K_VK_O);
		}
		if(key == GLFW_KEY_P) {
			KInput::setKeyPressed(K_VK_P);
		}
		if(key == GLFW_KEY_Q) {
			KInput::setKeyPressed(K_VK_Q);
		}
		if(key == GLFW_KEY_R) {
			KInput::setKeyPressed(K_VK_R);
		}
		if(key == GLFW_KEY_S) {
			KInput::setKeyPressed(K_VK_S);
		}
		if(key == GLFW_KEY_T) {
			KInput::setKeyPressed(K_VK_T);
		}
		if(key == GLFW_KEY_U) {
			KInput::setKeyPressed(K_VK_U);
		}
		if(key == GLFW_KEY_V) {
			KInput::setKeyPressed(K_VK_V);
		}
		if(key == GLFW_KEY_W) {
			KInput::setKeyPressed(K_VK_W);
		}
		if(key == GLFW_KEY_X) {
			KInput::setKeyPressed(K_VK_X);
		}
		if(key == GLFW_KEY_Y) {
			KInput::setKeyPressed(K_VK_Y);
		}
		if(key == GLFW_KEY_Z) {
			KInput::setKeyPressed(K_VK_Z);
		}
	}
	else if(action == GLFW_RELEASE)
	{
		if(key == GLFW_KEY_UP) {
			KInput::setKeyUnPressed(K_VK_UP);
		}
		if(key == GLFW_KEY_DOWN) {
			KInput::setKeyUnPressed(K_VK_DOWN);
		}
		if(key == GLFW_KEY_LEFT) {
			KInput::setKeyUnPressed(K_VK_LEFT);
		}
		if(key == GLFW_KEY_RIGHT) {
			KInput::setKeyUnPressed(K_VK_RIGHT);
		}
		
		if(key == GLFW_KEY_ENTER) {
			KInput::setKeyUnPressed(K_VK_RETURN);
		}
		if(key == GLFW_KEY_ESCAPE) {
			KInput::setKeyUnPressed(K_VK_ESCAPE);
		}
		if(key == GLFW_KEY_SPACE) {
			KInput::setKeyUnPressed(K_VK_SPACE);
		}
		if(key == GLFW_KEY_BACKSPACE) {
			KInput::setKeyUnPressed(K_VK_BACK);
		}
		
		if(key == GLFW_KEY_A) {
			KInput::setKeyUnPressed(K_VK_A);
		}
		if(key == GLFW_KEY_B) {
			KInput::setKeyUnPressed(K_VK_B);
		}
		if(key == GLFW_KEY_C) {
			KInput::setKeyUnPressed(K_VK_C);
		}
		if(key == GLFW_KEY_D) {
			KInput::setKeyUnPressed(K_VK_D);
		}
		if(key == GLFW_KEY_E) {
			KInput::setKeyUnPressed(K_VK_E);
		}
		if(key == GLFW_KEY_F) {
			KInput::setKeyUnPressed(K_VK_F);
		}
		if(key == GLFW_KEY_G) {
			KInput::setKeyUnPressed(K_VK_G);
		}
		if(key == GLFW_KEY_H) {
			KInput::setKeyUnPressed(K_VK_H);
		}
		if(key == GLFW_KEY_I) {
			KInput::setKeyUnPressed(K_VK_I);
		}
		if(key == GLFW_KEY_J) {
			KInput::setKeyUnPressed(K_VK_J);
		}
		if(key == GLFW_KEY_K) {
			KInput::setKeyUnPressed(K_VK_K);
		}
		if(key == GLFW_KEY_L) {
			KInput::setKeyUnPressed(K_VK_L);
		}
		if(key == GLFW_KEY_M) {
			KInput::setKeyUnPressed(K_VK_M);
		}
		if(key == GLFW_KEY_N) {
			KInput::setKeyUnPressed(K_VK_N);
		}
		if(key == GLFW_KEY_O) {
			KInput::setKeyUnPressed(K_VK_O);
		}
		if(key == GLFW_KEY_P) {
			KInput::setKeyUnPressed(K_VK_P);
		}
		if(key == GLFW_KEY_Q) {
			KInput::setKeyUnPressed(K_VK_Q);
		}
		if(key == GLFW_KEY_R) {
			KInput::setKeyUnPressed(K_VK_R);
		}
		if(key == GLFW_KEY_S) {
			KInput::setKeyUnPressed(K_VK_S);
		}
		if(key == GLFW_KEY_T) {
			KInput::setKeyUnPressed(K_VK_T);
		}
		if(key == GLFW_KEY_U) {
			KInput::setKeyUnPressed(K_VK_U);
		}
		if(key == GLFW_KEY_V) {
			KInput::setKeyUnPressed(K_VK_V);
		}
		if(key == GLFW_KEY_W) {
			KInput::setKeyUnPressed(K_VK_W);
		}
		if(key == GLFW_KEY_X) {
			KInput::setKeyUnPressed(K_VK_X);
		}
		if(key == GLFW_KEY_Y) {
			KInput::setKeyUnPressed(K_VK_Y);
		}
		if(key == GLFW_KEY_Z) {
			KInput::setKeyUnPressed(K_VK_Z);
		}
	}
}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

- (id) init
{
    if (self = [super init])
	{
		glfwSetErrorCallback(error_callback);
		
		if (!glfwInit())
			exit(EXIT_FAILURE);
		
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		
		//extern	bool	fullscreen;
		
		bool fullscreen = false;
		if(fullscreen == true)
		{
			GLFWmonitor * monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);

			window = glfwCreateWindow(ScreenX, ScreenY, NAME, monitor, NULL);
			//window = glfwCreateWindow(1280, 800, NAME, monitor, NULL);
		}
		else
		{
			window = glfwCreateWindow(1280, 960, NAME, NULL, NULL);
			//window = glfwCreateWindow(ScreenX, ScreenY, NAME, NULL, NULL);
		}
		
		if (!window)
		{
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		
		glfwGetFramebufferSize(window, &frame_width, &frame_height);
		printf("Framebuffer size: %d %d\n", frame_width, frame_height);
		glViewport(0,0,frame_width,frame_height);
		
		
		glfwSetKeyCallback(window, key_callback);
		
		glfwSetWindowCloseCallback(window, close_callback);
		
		glfwMakeContextCurrent(window);
		//gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
		glfwSwapInterval(1);
		
	
		
		// Use Gouraud (smooth) shading
		glShadeModel(GL_SMOOTH);
		
		// Switch on the z-buffer
		glEnable(GL_DEPTH_TEST);

		glPointSize(2.0);
		
		// Background color is black
		glClearColor(0, 0, 0, 0);
		
        animationPhase = 0.0;
		//Instance.initGame();
		initGame();
	}
    return self;
}

- (void) dealloc
{
	cleanupGame();
	[super dealloc];
}

- (GLFWwindow*) getWindow
{
	return window;
}

- (void) setFocus
{
	if(window) {
		glfwFocusWindow(window);
	}
}

- (void) advanceTimeBy:(float)seconds
{
    float phaseDelta = seconds - floor(seconds);
    float newAnimationPhase = animationPhase + 0.015625 * phaseDelta;
    newAnimationPhase = newAnimationPhase - floor(newAnimationPhase);
    [self setAnimationPhase:newAnimationPhase];
}

- (void) setAnimationPhase:(float)newAnimationPhase
{
    animationPhase = newAnimationPhase;
}

- (void) setViewportRect:(NSRect)bounds
{
	int x_offset = 0;
	int y_offset = 0;
	int w = bounds.size.width;
	int h = bounds.size.height;
	float currentRatio = bounds.size.width / bounds.size.height;
	float ratio = (float)ScreenX / (float)ScreenY;
	if(currentRatio > ratio)
	{
		w = bounds.size.height * ratio;
		x_offset = (bounds.size.width - w)/2;
	}
	else if(currentRatio < ratio)
	{
		h = bounds.size.width / ratio;
		y_offset = (bounds.size.height - h)/2;
	}
	glViewport(x_offset, y_offset, w, h);
}

- (void) render
{
	while (!glfwWindowShouldClose(window))
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		
		loopGame();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	exit(0);
}

@end
