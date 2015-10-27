#version 150

in vec3 position;
in vec3 color;
in vec3 normal;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform float waveOffset;
uniform float storminess;
uniform float lightningTimer;

flat out vec3 triColor;
//out vec3 triColor;

void main() {

	//storminess = 1;
	float s1 = 1 - (0.8 * storminess);
	float s2 = 1 - (0.4 * storminess);
	float s3 = 1 + (1 * storminess);

	//bouncy wave effect
	float waveY = sin(waveOffset * 8) / 2;

	//calculate the basic lighting
	vec3 lightV = normalize(vec3(1.5,2,-1));
	//vec3 mNorm = vec3(model * vec4(normal, 0));
	vec3 mNorm = normalize( vec3(model * vec4(normal, 0)) );
	float diffuse = max(dot(mNorm, lightV), 0.3);
	//float diffuse = max(dot(mNorm, lightV) * 10, 0.3);

	triColor = color * diffuse;

	triColor.r = triColor.r * s1;
	triColor.g = triColor.g * s1;
	triColor.b = triColor.b * s2;

	if (lightningTimer < 0.3) {
		triColor = vec3(1,1,1);	
	}
	else if (lightningTimer < 3) {
		vec3 lightningColor = vec3(0,0,0);

		if (normal.y > normal.x && normal.y > normal.z) {
			lightningColor = vec3(1,1,1);
		}

		if (lightningTimer < 1) {
			float lightningDelta = (lightningTimer - 0.3) / 0.7;
			triColor = vec3(1,1,1) + ((lightningColor - vec3(1,1,1)) * lightningDelta);
		}
		else if (lightningTimer > 2) {
			float lightningDelta = 1 - ((lightningTimer - 2) / 1);
			triColor = triColor + ((lightningColor - triColor) * lightningDelta);
		}
		else {
			triColor = lightningColor;
		}
	} 
	
	gl_Position = proj * view * model * vec4(position + vec3(0, waveY * s3, 0), 1.0);
}