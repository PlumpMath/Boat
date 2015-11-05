#include "RainParticles.h"

#include <iostream>

void RainParticles::init() {
	std::cout << "init rain system" << std::endl;

	GLfloat vertices[] = {
		//x, y, z,
		-1, 0, 0,
		1, 0, 0,
		-1, -20, 0,
		1, -20, 0,
	};

	GLuint elements[] = {
		0, 1, 2,
		1, 2, 3,
	};

	model = buildModel(
		"rain_vert.glsl", "rain_frag.glsl", 
		vertices, sizeof(vertices),
		elements, sizeof(elements),
		{
			{"position", 3}
		}
	);

	uniProj = glGetUniformLocation(model.shader.program, "proj");
	uniView = glGetUniformLocation(model.shader.program, "view");
	uniModel = glGetUniformLocation(model.shader.program, "model");
}

RainParticles::RainParticles() {
	std::cout << "rain constructor" << std::endl;

	position = glm::vec3(0, 3, 0);

	lifetimeMax = 4;
	maxTimeBetweenBursts = 1;
	particleBurstCount = 1;
}

/*
void RainParticles::update(float dt) {
	std::cout << "hello" << std::endl;
}
*/


Particle RainParticles::generateParticle() {
	return {
		glm::vec3(position.x, position.y, position.z), //pos 
		glm::vec3(0.005, 0.005, 0.005), //scale
		glm::vec3(0, 0, 0), //rot
		glm::vec3(0, -1, 0), //velocity
		0 //lifetime
	};
}