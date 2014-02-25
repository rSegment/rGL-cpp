#version 330 core
float sine,cosine;
uniform float time;
out vec4 outputColour;

void main() {
	sine=sin(time);
	cosine=cos(time);		
	if (sine<0)
	sine=-sine;
  outputColour = vec4(1.0*sine,1.0*cosine,1.0,1.0);
}