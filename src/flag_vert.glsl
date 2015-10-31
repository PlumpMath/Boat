#version 150

in vec3 position;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform float waveOffset;
uniform float storminess;
uniform float lightningTimer;

out vec3 vertColor;

void main() {
	vec3 baseColor = vec3(0.8, 0.6, 0);
	vec3 stormColor = vec3(0.6, 0.4, 0.2);
	vec3 lightningColor = vec3(0, 0, 0);
	
	vertColor = baseColor + ((stormColor - baseColor) * storminess);
	
	if (lightningTimer < 0.3) {
		vertColor = vec3(1,1,1);
	}
	else if (lightningTimer < 3) {
		if (lightningTimer < 1) {
			float lightningDelta = (lightningTimer - 0.3) / 0.7;
			vertColor = vec3(1,1,1) + ((lightningColor - vec3(1,1,1)) * lightningDelta);
		}
		else if (lightningTimer > 2) {
			float lightningDelta = 1 - ((lightningTimer - 2) / 1);
			vertColor = vertColor + ((lightningColor - vertColor) * lightningDelta);
		}
		else {
			vertColor = lightningColor;
		}
	}

	if (position.z < 0) {
		gl_Position = proj * view * model * vec4(sin((waveOffset * 8 * (1 + storminess)) + position.z), position.y, position.z, 1.0);	
	}
	else {
		gl_Position = proj * view * model * vec4(position, 1.0);	
	}
	
}