#version 150

in vec3 position;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform float lightningTimer;

out vec3 vertColor;

void main() {
	
	vertColor = vec3(0.5, 0.5, 0.7);
	gl_Position = proj * view * model * vec4(position, 1.0);

	if (lightningTimer < 3) {
		if (lightningTimer > 2) {
			float lightningDelta = 1 - ((lightningTimer - 2) / 1);
			vertColor = vertColor + ((vec3(1,1,1) - vertColor) * lightningDelta);
		}
		else {
			vertColor = vec3(1,1,1);
		}
	}
	
}