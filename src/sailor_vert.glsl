#version 150

in vec3 position;
in float colorChoice;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform vec3 primaryColor;
uniform vec3 secondaryColor;
uniform float lightningTimer;
uniform float storminess;

flat out vec3 vertColor;

void main() {
	
	//primary and secondary colors
	vertColor = vec3(1,1,1);
	if (colorChoice < 1) {
		vertColor = primaryColor;
	}
	else {
		vertColor = secondaryColor;
	}
	vertColor = vertColor * (1 - (0.5 * storminess));

	//lightning
	vec3 lightningColor = vec3(1,1,1);
	if (colorChoice >= 1) {
		lightningColor = vec3(0,0,0);
	}

	if (lightningTimer < 0.3) {
		vertColor = vec3(1,1,1);
	}
	else if (lightningTimer < 1) {
		float lightningDelta = (lightningTimer - 0.3) / 0.7;
		vertColor = vec3(1,1,1) + ((lightningColor - vec3(1,1,1)) * lightningDelta);
	}
	else if (lightningTimer < 2) {
		vertColor = lightningColor;
	}
	else if (lightningTimer < 3) {
		float lightningDelta = 1 - ((lightningTimer - 2) / 1);
		vertColor = vertColor + ((lightningColor - vertColor) * lightningDelta);
	}

	gl_Position = proj * view * model * vec4(position, 1.0);	
	
}