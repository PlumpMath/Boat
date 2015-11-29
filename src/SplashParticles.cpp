#include "SplashParticles.h"

#include <iostream>

void SplashParticles::init() {
	std::cout << "init splash system" << std::endl;

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
		"smoke_vert.glsl", "smoke_frag.glsl", //using the same shaders here
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

SplashParticles::SplashParticles() {
	particleBurstCount = 0; //only generate particles on command
	maxTimeBetweenBursts = 0;
	lifetimeMax = 0.6f;
}

void SplashParticles::updateParticle(Particle* p, float dt) {
	ParticleSystem::updateParticle(p, dt);
	p->velocity.y -= gravity * dt;
}


void SplashParticles::splash(glm::vec3 pos) {
	position = pos;
	particleBurstCount = 12; //make the burst occur
}

Particle* SplashParticles::generateParticle(int i) {
	float rad = glm::radians( 360.0f * ((i * 1.0f) / (particleBurstCount * 1.0f)) );
	float r = 0.8f + ( 0.4f * ((rand() % 100) / 100.0f) );

	Particle* p = new Particle();
	p->position = glm::vec3(position.x + (cos(rad) * 0.05f), position.y, position.z + (sin(rad) * 0.05f));
	p->scale = glm::vec3(0.02f, 0.02f, 0.02f);
	p->rotation = glm::vec3(0, 0, 0);
	p->velocity = glm::vec3(cos(rad) * 0.5f * r, gravity * 0.3f, sin(rad) * 0.5f * r);
	p->lifetime = 0;

	//stop making more particles after one burst
	if (i == (particleBurstCount - 1)) {
		particleBurstCount = 0;
	}

	return p;
}

void SplashParticles::drawParticle(Particle* p) {
	glUniform1f(uniAlpha, (1 - (p->lifetime / lifetimeMax)) * 0.8f);
	ParticleSystem::drawParticle(p);
}