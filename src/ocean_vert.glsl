#version 150

in vec3 position;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform float waveOffset;
uniform vec3 bigWavePos;
uniform float storminess;
uniform float lightningTimer;

flat out vec3 color;
//flat varying vec3 color;

void main()
{
	//float storminess = 1;
	//storm factors
	//float s1 = 1 + (min(1, storminess * 5 * 3) * 0.7);
	float s1 = 1;
	if (storminess >= 1) {
		s1 = 1.7;
	}
	float s2 = 1 + (storminess * 0.5);

	//a bunch of magical sine wave variables for all the different wave effects
	float waveY = sin((waveOffset + position.z) * 8) / 8;
	float waveZ = sin((waveOffset + position.x) * 8 * s1) / 8;
	float waveNormY = (1 + sin((waveOffset + position.z) * 8)) * 0.5;
	float waveNormZ = sin((waveOffset + 1 + position.z) * 8);

	//make the normal
	vec3 normal = vec3(0, waveNormY, waveNormZ);

	//calculate the basic lighting
	vec3 lightV = normalize(vec3(0,1,-1));
	vec3 mNorm = vec3(model * vec4(normal, 0));
	float diffuse = max(dot(mNorm, lightV), 0.6);	
	
	//make a big wave!
	vec3 bigWaveFactor = vec3(0,0,0);
	float bigWaveDist = distance( vec2(position.x, position.z), vec2(bigWavePos.x, bigWavePos.z) );
	if (bigWaveDist < bigWavePos.y) {
		bigWaveFactor = vec3(0, min((bigWavePos.y - bigWaveDist), bigWavePos.y * 0.8), 0);
		diffuse = diffuse + (diffuse * ((bigWavePos.y - bigWaveDist) / bigWavePos.y));
	}

	//set the color
	vec3 startColor = vec3(0.0, 0.5, 1.0); //very blue
	vec3 stormColor = vec3(0.2, 0.3, 0.5);
	vec3 curColor = startColor + ((stormColor - startColor) * storminess);
	color = curColor * diffuse;

	if (lightningTimer < 0.3) {
		color = vec3(1,1,1);	
	}
	else if (lightningTimer < 3) {
		vec3 lightningColor = vec3(1,1,1);

		if (diffuse < 1) {
			lightningColor = vec3(0,0,0);
		}

		if (lightningTimer < 1) {
			float lightningDelta = (lightningTimer - 0.3) / 0.7;
			color = vec3(1,1,1) + ((lightningColor - vec3(1,1,1)) * lightningDelta);
		}
		else if (lightningTimer > 2) {
			float lightningDelta = 1 - ((lightningTimer - 2) / 1);
			color = color + ((lightningColor - color) * lightningDelta);
		}
		else {
			color = lightningColor;
		}
	} 

	//deform the mesh
	vec3 newPos = position + vec3(0, waveY * s2, waveZ * s2) + bigWaveFactor;
	gl_Position = proj * view * model * vec4(newPos, 1.0);
}