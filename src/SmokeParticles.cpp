#include "SmokeParticles.h"

#include <iostream>
#include <stdlib.h> 

void SmokeParticles::init() {
	std::cout << "init smoke system" << std::endl;

	GLfloat vertices[] = {
		//x, y, z,
		-1, 1, 0,
		1, 1, 0,
		-1, -1, 0,
		1, -1, 0,
	};

	GLuint elements[] = {
		0, 1, 2,
		1, 2, 3,
	};

	model = buildModel(
		"smoke_vert.glsl", "smoke_frag.glsl", 
		vertices, sizeof(vertices),
		elements, sizeof(elements),
		{
			{"position", 3}
		}
	);

	uniProj = glGetUniformLocation(model.shader.program, "proj");
	uniView = glGetUniformLocation(model.shader.program, "view");
	uniModel = glGetUniformLocation(model.shader.program, "model");
	uniAlpha = glGetUniformLocation(model.shader.program, "alpha");
}

SmokeParticles::SmokeParticles() {
	lifetimeMax = 1;
	maxTimeBetweenBursts = 0.5;
	particleBurstCount = 100;
}

void SmokeParticles::updateParticle(Particle* p, float dt) {
	ParticleSystem::updateParticle(p, dt);

	SmokeParticle* s = (SmokeParticle*)p;
	s->scale += glm::vec3(0.01, 0.01, 0.01) * dt;
	s->rotation.z += s->rotationSpeed * dt;
}

Particle* SmokeParticles::generateParticle(int i) {
	float rad;
	if (i % 2 == 0) { //this is to help me draw from back to front (necessary for alpha - but why?)
		rad = glm::radians( 90 + ((i * 0.5f) / (particleBurstCount * 0.5f)) * 180.0f );
	}
	else {
		rad = glm::radians( 90 + ((i * 0.5f) / (particleBurstCount * 0.5f)) * -180.0f );
	}
	float size = 0.001 + (0.015 * ((rand() % 100) / 100.0f));
	float rotSpeed = -100 + ((rand() % 200) * 1.0f);

	SmokeParticle* s = new SmokeParticle();
	s->position = glm::vec3(position.x,position.y,position.z);
	s->scale = glm::vec3(size, size, size);
	s->rotation = glm::vec3(0,0,0);
	s->velocity = glm::vec3(cos(rad) * 0.075, 0.3, sin(rad) * 0.075);
	s->lifetime = 0;
	s->rotationSpeed = rotSpeed;

	return s;
}

void SmokeParticles::drawParticle(Particle* p) {
	glUniform1f(uniAlpha, (1 - (p->lifetime / lifetimeMax)) * 0.1 );
	ParticleSystem::drawParticle(p);
}