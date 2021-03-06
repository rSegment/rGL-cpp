﻿#include <iostream>
#include <stdlib.h>
#include <string>

#include <fstream>
#include <vector>
 


#include <GL3/gl3w.h>		// GL3w must be included before any other OpenGL related headers
#include <GL/glfw3.h>		//Includes gl.h only (glu is deprecated)
#include <glm/glm.hpp>		// OpenGL Mathematics

using namespace glm;


GLuint running;
GLdouble time;

/*********************
Step 2b 
*********************/
const GLfloat vertexPositions[]={
    0.75f, 0.75f, 0.0f, 1.0f,
   0.75f, -0.75f, 0.0f, 1.0f,
    -0.75f, -0.75f, 0.0f, 1.0f,
};

const GLfloat vertexColors[] = {
	0.75f, 0.75f, 0.0f, 1.0f,
	0.75f, 0.0f, 0.0f, 1.0f,
	200.0f, -0.75f, 0.0f, 1.0f,
};

//Global Variables
GLuint vao = 0;
GLuint positionBufferObject;					//This will store the handle to the first buffer object

GLfloat timeLapse;



GLFWwindow* window;



// Check Errors simple function

void checkError(const char *functionName)
{
	GLenum error;
	while (( error = glGetError() ) != GL_NO_ERROR) {
		fprintf (stderr, "OpenGL error 0x%X found in %s\n", error, functionName);
	}
}


// Shader pointers
GLuint vs;
GLuint fs;
GLuint program;     // points to shader 



// Shaderdump

std::string readFile(const char *filePath) {

     std::string content;
     std::ifstream fileStream(filePath, std::ios::in);

     if(!fileStream.is_open()) {
         std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
         return "";
     }

     std::string line = "";
     while(!fileStream.eof()) {
         std::getline(fileStream, line);
         content.append(line + "\n");
     }

     fileStream.close();
     return content;
}


 GLuint LoadShader(const char *vertex_path, const char *fragment_path) {
     GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
     GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	 // Read shaders
     std::string vertShaderStr = readFile(vertex_path);
     std::string fragShaderStr = readFile(fragment_path);
     const char *vertShaderSrc = vertShaderStr.c_str();
     const char *fragShaderSrc = fragShaderStr.c_str();


     GLint result = GL_FALSE;
     int logLength;


     // Compile vertex shader
     std::cout << "Compiling vertex shader." << std::endl;
     
	 glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
     glCompileShader(vertShader);
	 

     // Check vertex shader
     glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
     glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
     std::vector<char> vertShaderError(logLength);
     glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]);
     std::cout << &vertShaderError[0] << std::endl;
	 

     // Compile fragment shader
     std::cout << "Compiling fragment shader." << std::endl;
     glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
     glCompileShader(fragShader);

     // Check fragment shader
     glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
     glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
     std::vector<char> fragShaderError(logLength);
     glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);
     std::cout << &fragShaderError[0] << std::endl;


     std::cout << "Linking program" << std::endl;
     GLuint program = glCreateProgram();
     glAttachShader(program, vertShader);
     glAttachShader(program, fragShader);
     glLinkProgram(program);

     glGetProgramiv(program, GL_LINK_STATUS, &result);
     glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
     std::vector<char> programError( (logLength > 1) ? logLength : 1 );
     glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
     std::cout << &programError[0] << std::endl;

     glDeleteShader(vertShader);
     glDeleteShader(fragShader);

     return program;
}


// VBO Function

void InitializeVertexBuffer(){
	
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);

	glGenBuffers(1,&positionBufferObject);													//generates 1 buffer object and puts his handle in positionBufferObject
	glBindBuffer(GL_ARRAY_BUFFER,positionBufferObject);										// binds a VBO as a bindig target of GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertexPositions),vertexPositions,GL_STATIC_DRAW);  // at last, for the binded target to GL_ARRAY_BUFFER, allocates memory and then saves on the GPU vertexPositions
	glBindBuffer(GL_ARRAY_BUFFER,0);														//unbinds gl_array_buffer							
	checkError("InitializeVertexBuffer");
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

	if (!glfwInit()) 
		exit(EXIT_FAILURE);   





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
	
	window = glfwCreateWindow(800, 600, "rGL", NULL, NULL);		// a 800x600 window is created with default color bits (RGB) and alpha,depth and stencil disabled


	glfwWindowHint(GLFW_VERSION_MAJOR, 3); // We want OpenGL 3.2
	glfwWindowHint(GLFW_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL
 

	if(!window)			 
	{
		glfwTerminate();
		exit( EXIT_FAILURE );
	}

	glfwMakeContextCurrent(window);

	


	/**********************
	3. gl3wInit			
	Must right after the context windows had started

	***********************/



	if (gl3wInit()) {
		fprintf(stderr, "failed to initialize OpenGL\n");
		return -1;
	}


	if (!gl3wIsSupported(3, 3)) {
		std::cerr << "OpenGL 4.0 not supported" << std::endl;
		return -1;
	} 



	std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: "  << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl; 		// OpenGL and GL SHADING LANGUAGE VERSION
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;       
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl; 
	

	checkError("init");

	return 0;

}

int main(void){


	
	timeLapse = (float)GetTickCount();
	init();
	InitializeVertexBuffer();						//2b. calls VBO creation function

	program = LoadShader("../shaders/vs.glsl", "../shaders/fs.glsl");						//Loads shaders to GPU

	glClearColor(1,1,0,1);							// ClearScreenColor glClearColor( GLfloat   red,   GLfloat   green,   GLfloat   blue,  GLfloat   alpha) values are in 0,1 range 

		// 2b

	glUseProgram(program);


	glBindBuffer(GL_ARRAY_BUFFER,positionBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,0);		// No need to specify from wihich buffer data comes from. It uses always the one in GL_ARRAY_BUFFER
		
	//
	GLuint loc = glGetUniformLocation(program, "time");
	
	float sine;
	float cosine;
	/**********************
	4. Main Loop
	***********************/

	while(!glfwWindowShouldClose(window)  )
	{
		timeLapse = (float)GetTickCount();
		timeLapse = timeLapse / 1000;
		printf("%f\n", timeLapse);

		sine = sin(timeLapse);
		cosine = cos(timeLapse);

		glClearColor(1*cosine, 1*sine, 1*sine*cosine, 1);

		glClear( GL_COLOR_BUFFER_BIT );				// Clear the buffers currently enabled for color writing(BUFFER BIT - can be OR'ed with GL_DEPTH_BUFFER_BIT and GL_STENCIL_BUFFER_BIT if used). 
				
		loc = glGetUniformLocation(program, "time");
		glUniform1f(loc, timeLapse);
		

		// OpenGL rendering goes here...
		
		glDrawArrays (GL_TRIANGLES, 0, 3);
		
		
		glfwSwapBuffers(window);										// Swap front and back rendering buffers. GLFW is by default double buffered

		glfwPollEvents();
		
		
	}




	/*********************************
	Last.This releases any resources allocated by GLFW, closes the window if it is open and kills any secondary threads created by GLFWA˙ fter this call, you must call glfwInit again before using any GLFW functions).
	*********************************/
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glDisableVertexAttribArray(0);
	glfwTerminate();                        	
	return 0;
}