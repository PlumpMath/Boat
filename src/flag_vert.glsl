#version 150

in vec3 position;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform float waveOffset;

out vec3 vertColor;

void main() {
	vertColor = vec3(0.8, 0.6, 0);
	if (position.z < 0) {
		gl_Position = proj * view * model * vec4(sin((waveOffset * 16) + position.z) * 0.5, position.y, position.z, 1.0);	
	}
	else {
		gl_Position = proj * view * model * vec4(position, 1.0);	
	}
	
}