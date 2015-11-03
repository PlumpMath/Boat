#version 150

in vec3 position;
in vec3 color;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform float storminess;
uniform float lightningTimer;

flat out vec4 vertColor;

void main() {
	
	//lighting
	vec3 normal = normalize(position - vec3(0, position.y, 0)); //fake-ish normal calculation//calculate the basic lighting
	vec3 lightV = normalize(vec3(1,1,-8));
	vec3 mNorm = normalize( vec3(model * vec4(normal, 0)) );
	float diffuse = max(dot(mNorm, lightV), 0.6);

	//float a = 1;
	//if (storminess >= 1) a = 0;

	float a = 1 - (storminess * 0.95);

	if (lightningTimer < 0.3) {
		vertColor = vec4(1,1,1,1);
	}
	else if (lightningTimer < 3) {
		vec3 lightningColor = vec3(1,1,1);
		if (color.b == 0 && position.y < 50) {
			lightningColor = vec3(0,0,0);
		}

		if (lightningTimer < 1) {
			float lightningDelta = (lightningTimer - 0.3) / 0.7;
			lightningColor = vec3(1,1,1) + ((lightningColor - vec3(1,1,1)) * lightningDelta);
			vertColor = vec4(lightningColor, 1);
		}
		else if (lightningTimer > 2) {
			float lightningDelta = 1 - ((lightningTimer - 2) / 1);

			if (storminess >= 1) {
				vertColor = vec4(lightningColor, lightningDelta);
			}
			else {
				vertColor = vec4(lightningColor + ((1 - lightningDelta) * ((color * diffuse) - lightningColor)), 1);
			}
			
		}
		else {
			vertColor = vec4(lightningColor, 1);
		}
	}
	else {
		vertColor = vec4(color * diffuse, a);
	}

	

	gl_Position = proj * view * model * vec4(position, 1.0);	
	
}