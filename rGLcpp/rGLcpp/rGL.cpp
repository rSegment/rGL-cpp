#include <GL/glfw.h>	  //Includes gl.h & glu.h
#include <stdlib.h>


int main(void){

	/*********************
	0.Indicates that the program is running
	*********************/
	int running= GL_TRUE;
	
	/**********************
	1.Initializes glfw parts that are not dependent on a window, such as threading, timer and joystick input. or else exits
	***********************/

	if (!glfwInit()) exit(EXIT_FAILURE);   

	
	
	
	
	/***********************
	2.
	Opening an OpenGL window is done with the glfwOpenWindow function. The function takes nine 	arguments, which are used to describe the following properties of the requested window:
	 -Window dimensions (width and height) in pixels.
	 -Color and alpha buffer bit depth.
	 -Depth buffer (Z-buffer) bit depth.
	 -Stencil buffer bit depth.
	 -Whether to use fullscreen or windowed mode.

	int glfwOpenWindow( int width, int height,
						int redbits, int greenbits, int bluebits,
						int alphabits, int depthbits, int stencilbits,
						int mode )
	
	If mode is GLFW_FULLSCREEN, the window will cover the entire screen and no window border or
		decorations will be visible. If possible, the video mode will be changed to the mode that closest matches
		the width, height, redbits, greenbits, bluebits and alphabits arguments. Furthermore, the mouse pointer
		will be hidden, and screensavers are prohibited. This is usually the best mode for games and demos.
	If mode is GLFW_WINDOW, the window will be opened as a normal, decorated window on the
		desktop. The mouse pointer will not be hidden and screensavers are allowed to be activated.

		int depthbits, int stencilbits->related to depth_buffer and stencil_buffer but in what way?
	************************/
	
	
	if( !glfwOpenWindow( 300,300, 0,0,0,0,0,0, GLFW_WINDOW ) )
		{
		glfwTerminate();
		exit( EXIT_FAILURE );
		}

	






	/*********************************
	Last.This releases any resources allocated by GLFW, closes the window if it is open and kills any secondary threads created by GLFWA˙ fter this call, you must call glfwInit again before using any GLFW functions).
	*********************************/

	glfwTerminate();                        	return 0;
}