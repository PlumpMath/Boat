#version 150

in vec3 position;
in vec3 color;
in vec3 normal;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform float waveOffset;

flat out vec3 triColor;
//out vec3 triColor;

void main() {

	//bouncy wave effect
	float waveY = sin(waveOffset * 8) / 2;

	//calculate the basic lighting
	vec3 lightV = normalize(vec3(1.5,2,-1));
	//vec3 mNorm = vec3(model * vec4(normal, 0));
	vec3 mNorm = normalize( vec3(model * vec4(normal, 0)) );
	float diffuse = max(dot(mNorm, lightV), 0.3);
	//float diffuse = max(dot(mNorm, lightV) * 10, 0.3);

	triColor = color * diffuse;
	gl_Position = proj * view * model * vec4(position + vec3(0, waveY, 0), 1.0);
}