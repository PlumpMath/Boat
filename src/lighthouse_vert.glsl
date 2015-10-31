#version 150

in vec3 position;
in vec3 color;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

flat out vec3 vertColor;

void main() {
	
	//lighting
	vec3 normal = normalize(position - vec3(0, position.y, 0)); //fake-ish normal calculation//calculate the basic lighting
	vec3 lightV = normalize(vec3(1,1,-8));
	vec3 mNorm = normalize( vec3(model * vec4(normal, 0)) );
	float diffuse = max(dot(mNorm, lightV), 0.6);

	vertColor = color * diffuse;

	gl_Position = proj * view * model * vec4(position, 1.0);	
	
}