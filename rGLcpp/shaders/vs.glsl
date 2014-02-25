#version 330 core
float sine;
in int gl_VertexID;
uniform float time;
layout(location=0) in vec4 position;
void main()
{
sine=sin(time);	
gl_Position=vec4(position[0]*sine,position[1]*sine,position[2]*sine,position[3]);

}