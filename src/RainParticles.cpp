#include "RainParticles.h"

#include <iostream>
#include <stdlib.h> 

void RainParticles::init() {
	std::cout << "init rain system" << std::endl;

	GLfloat vertices[] = {
		//x, y, z,
		-1, 0, 0,
		1, 0, 0,
		-1, -100, 0,
		1, -100, 0,
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
	uniLightning = glGetUniformLocation(model.shader.program, "lightningTimer");
}

RainParticles::RainParticles() {
	std::cout << "rain constructor" << std::endl;

	position = glm::vec3(0, 3, 4);

	lifetimeMax = 1.5;
	maxTimeBetweenBursts = 0.1;
	particleBurstCount = 20;

	//why necessary???
	std::cout << xRange << std::endl;
	std::cout << zRange << std::endl;
	xRange = 8;
	zRange = 6;

	//windDirection = glm::vec3(0,-2,-4); //angle ok
	windDirection = glm::normalize( glm::vec3(0,-1,-2) ); //speed ok
}

void RainParticles::update(float dt) {
	
	float startOfStormTime = 65; //hack
	float impossibleModeTime = 135; //hack
	float fakeCalmTime = 155; //hack

	float raininess = 0;
	if (totalPlayTime < startOfStormTime - 5) {
		raininess = 0;
	}
	else if (totalPlayTime < startOfStormTime) {
		raininess = (totalPlayTime - (startOfStormTime - 5)) / 5;
	}
	else if (totalPlayTime < impossibleModeTime) {
		raininess = 1;
	}
	else if (totalPlayTime < fakeCalmTime) {
		raininess = 2; //more?
	}
	else if (totalPlayTime < fakeCalmTime + 5) {
		raininess = (totalPlayTime - (fakeCalmTime)) / 5;
	}

	particleBurstCount = 20 * raininess;
	//particleBurstCount = 20 * storminess;

	//if (gustTimer > 0) gustTimer -= dt;

	ParticleSystem::update(dt);

}

void RainParticles::draw() {
	//setup drawing environment
	glBindVertexArray(model.vao);
	glUseProgram(model.shader.program);
	glUniform1f(uniLightning, lightningTimer);
	//
	ParticleSystem::draw();
}

/*
void RainParticles::gust(glm::vec3 dir) {
	gustDirection = dir;
	gustTimer = 3.0f;
}

void RainParticles::updateParticle(Particle* p, float dt) {
	ParticleSystem::updateParticle(p, dt);

	float gustStrength = 0;
	float gustMax = 3;
	if (gustTimer > 2) {
		gustStrength = gustMax * ((gustTimer - 2) / 1);
	}
	else {
		gustStrength = gustMax * (gustTimer / 2);
	}

	if (gustTimer > 0) {
		p->position += gustDirection * gustStrength * dt;
	}
}
*/

Particle* RainParticles::generateParticle(int i) {

	float r1 = (rand() % 100) / 100.0f;
	float r2 = (rand() % 100) / 100.0f;
	float r3 = (rand() % 100) / 100.0f;
	float r4 = (rand() % 100) / 100.0f;

	float randX = xRange - (xRange * 2 * r1);
	float randZ = zRange - (zRange * 2 * r2);

	float directionVariance = 0.05;
	glm::vec3 dir = windDirection + glm::vec3(directionVariance - (directionVariance * 2 * r4), 0, 0);

	glm::vec3 rot;
	//rot.x = windDirection.z * -15;
	//rot.z = windDirection.x * 15;
	rot.x = 60;
	float rotVariance = 3;
	rot.z = rotVariance - (rotVariance * 2 * r4);

	float speed = minSpeed + ((maxSpeed - minSpeed) * r3);

	Particle* p = new Particle();
	p->position = glm::vec3(position.x + randX, position.y, position.z + randZ);
	p->scale = glm::vec3(0.005, 0.005, 0.005);
	p->rotation = rot;
	p->velocity = dir * speed;
	p->lifetime = 0;

	return p;
}