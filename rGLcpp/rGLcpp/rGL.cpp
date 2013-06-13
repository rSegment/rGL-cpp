#include <iostream>
#include <stdlib.h>



#include <GL3/gl3w.h>		// GL3w must be included before any other OpenGL related headers
#include <GL/glfw.h>		//Includes gl.h & glu.h
#include <glm/glm.hpp>		// OpenGL Mathematics

using namespace glm;

GLuint running;
GLdouble time;

/*********************
Step 2b 
*********************/


//Global Variables
GLuint positionBufferObject;					//This will store the handle to the first buffer object
const float vertexPositions[]={					//The vertexes of the triangle
    0.75f, 0.75f, 0.0f, 1.0f,					
    0.75f, -0.75f, 0.0f, 1.0f,
    -0.75f, -0.75f, 0.0f, 1.0f,
};

// VBO Function

void InitializeVertexBuffer(){
	glGenBuffers(1,&positionBufferObject);													//generates 1 buffer object and puts his handle in positionBufferObject
	glBindBuffer(GL_ARRAY_BUFFER,positionBufferObject);										// binds a VBO as a bindig target of GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertexPositions),vertexPositions,GL_STATIC_DRAW);  // for the binded target to GL_ARRAY_BUFFER, allocates memory and then saves on the GPU vertexPositions
	glBindBuffer(GL_ARRAY_BUFFER,0);														//unbinds gl_array_buffer							
}



int init(void){



/*********************
	0.Indicates that the program is running and initializes a timer in 0s
	*********************/
	running= GL_TRUE;
	time=0.0f;
	glfwSetTime(0);
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

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 2); // 2x antialiasing - should be optional in final version
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3); // We want OpenGL 3.2
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL
 


	if( !glfwOpenWindow( 800,600, 0,0,0,0,0,0, GLFW_WINDOW ) )			// a 800x600 window is created with default color bits (RGB) and alpha,depth and stencil disabled 
	{
		glfwTerminate();
		exit( EXIT_FAILURE );
	}

	glfwSetWindowTitle( "rGL" );										//sets window title "rGL"



	/**********************
	3. gl3wInit			
	Must right after the context windows had started

	***********************/



	if (gl3wInit()) {
		fprintf(stderr, "failed to initialize OpenGL\n");
		return -1;
	}


	if (!gl3wIsSupported(4, 0)) {
		std::cerr << "OpenGL 4.0 not supported" << std::endl;
		return -1;
	} 



	std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: "  << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl; 		// OpenGL and GL SHADING LANGUAGE VERSION
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;       
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl; 
	
	return 0;
}

int main(void){

	init();
	InitializeVertexBuffer();						//2b. calls VBO creation function

	glClearColor(0,1,0,0);							// ClearScreenColor glClearColor( GLfloat   red,   GLfloat   green,   GLfloat   blue,  GLfloat   alpha) values are in 0,1 range 

	/**********************
	4. Main Loop
	***********************/

	while( running )
	{
		
		
		// OpenGL rendering goes here...


		glClear( GL_COLOR_BUFFER_BIT );							// Clear the buffers currently enabled for color writing(BUFFER BIT - can be OR'ed with GL_DEPTH_BUFFER_BIT and GL_STENCIL_BUFFER_BIT if used). 

		glfwSwapBuffers();										// Swap front and back rendering buffers. GLFW is by default double buffered

		running = !glfwGetKey( GLFW_KEY_ESC ) &&				// Check if ESC key was pressed or window was closed
			glfwGetWindowParam( GLFW_OPENED );
	}




	/*********************************
	Last.This releases any resources allocated by GLFW, closes the window if it is open and kills any secondary threads created by GLFWA˙ fter this call, you must call glfwInit again before using any GLFW functions).
	*********************************/

	glfwTerminate();                        	
	return 0;
}