#version 330 core 

uniform vec3 color1;

out vec4 FragColor;

void main() 
{ 
	gl_FragColor = vec4(color1, 1.0);
}
