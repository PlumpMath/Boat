#version 150

in vec3 position;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec3 vertColor;

void main() {
	
	vertColor = vec3(1, 1, 1);
	gl_Position = proj * view * model * vec4(position, 1.0);
	
}