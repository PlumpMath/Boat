#version 150

in vec3 position;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform float waveOffset;
uniform vec3 bigWavePos;

flat out vec3 color;
//flat varying vec3 color;

void main()
{

	//a bunch of magical sine wave variables for all the different wave effects
	float waveY = sin((waveOffset + position.z) * 8) / 8;
	float waveZ = sin((waveOffset + position.x) * 8) / 8;
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
	
	//vec3 startColor = vec3(0.0, 0.7, 0.7); //green-ish
	//vec3 startColor = vec3(0.0, 0.7, 0.8); //green-ish
	
	//startColor = vec3(0.0, 0.6, 0.7); //green-ish

	color = startColor * diffuse;

	//deform the mesh
	vec3 newPos = position + vec3(0, waveY, waveZ) + bigWaveFactor;
	gl_Position = proj * view * model * vec4(newPos, 1.0);
}