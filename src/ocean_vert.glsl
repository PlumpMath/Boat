#version 150

in vec3 position;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform float waveOffset;

flat out vec3 color;
//flat varying vec3 color;

void main()
{

	float w = sin((waveOffset + position.z) * 8);
	float w2 = sin((waveOffset + 1 + position.z) * 8);
	float w3 = sin((waveOffset + position.x) * 4);
	vec3 normal = vec3(0, (1 + w) * 0.5, w2); //-1 + ((1 - w) * 2)

	vec3 lightV = normalize(vec3(0,1,-1));
	vec3 mNorm = vec3(model * vec4(normal, 0));
	float diffuse = max(dot(mNorm, lightV), 0.5);

	vec3 startColor = vec3(0.0, 0.5, 1.0);
	color = startColor * diffuse;

	vec3 newPos = position + vec3(0, (w / 16), (w3 / 16));
	gl_Position = proj * view * model * vec4(newPos, 1.0);
}