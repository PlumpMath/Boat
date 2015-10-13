#version 150

in vec3 position;
in vec3 color;
in vec3 normal;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

flat out vec3 triColor;
//out vec3 triColor;

void main() {

	//calculate the basic lighting
	vec3 lightV = normalize(vec3(0,2,-1));
	vec3 mNorm = vec3(model * vec4(normal, 0));
	float diffuse = max(dot(mNorm, lightV) * 10, 0.3); //max(dot(mNorm, lightV), 0.8);	

	triColor = color * diffuse;
	gl_Position = proj * view * model * vec4(position, 1.0);
}