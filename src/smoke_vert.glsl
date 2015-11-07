#version 150

in vec3 position;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform float alpha;

out vec4 vertColor;

void main() {
	
	vertColor = vec4(1,1,1,alpha);
	gl_Position = proj * view * model * vec4(position, 1.0);
	
}