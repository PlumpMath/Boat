#version 150

in vec3 position;
in vec3 color;
in vec3 normal;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

flat out vec3 triColor;

void main() {
	triColor = color;
	gl_Position = proj * view * model * vec4(position, 1.0);
}