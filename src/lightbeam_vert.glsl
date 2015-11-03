#version 150

in vec3 position;
in float alpha;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
//uniform float storminess;
//uniform float lightningTimer;

out vec4 color;

void main() {
	color = vec4(1, 1, 1, alpha);
	gl_Position = proj * view * model * vec4(position, 1.0);
}