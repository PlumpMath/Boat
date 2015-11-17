#include "ParticleSystem.h"

#include <iostream>

void ParticleSystem::init() {
	std::cout << "init particle system" << std::endl;
}

void ParticleSystem::cleanup() {
	destroyModel(model);
}

ParticleSystem::ParticleSystem() {
}

void ParticleSystem::update(float dt) {

	//create particles
	particleBurstTimer -= dt;
	if (particleBurstTimer <= 0) {
		for (int i = 0; i < particleBurstCount; i++) {
			Particle* p = generateParticle(i);
			particles.push_back(p);
		}
		particleBurstTimer = maxTimeBetweenBursts;
	}

	//update particles
	int count = 0;
	while (!particles.empty() && count < particles.size()) {

		updateParticle(particles[count], dt);

		//destroy particles
		if (particles[count]->lifetime > lifetimeMax) {
			delete particles[count]; //memory management
			particles.erase(particles.begin() + count); //remove current particle
		}
		else {
			count++; //go to next particle
		}

	}
	
}

Particle* ParticleSystem::generateParticle(int i) {
	std::cout << "particle system generate" << std::endl;

	Particle* p = new Particle();
	p->position = glm::vec3(0, 0, 0);
	p->scale = glm::vec3(1, 1, 1);
	p->rotation = glm::vec3(0, 0, 0);
	p->velocity = glm::vec3(0, 1, 0);
	p->lifetime = 0;

	return p;
}

void ParticleSystem::updateParticle(Particle* p, float dt) {
	//update particle motion & lifetime
	p->lifetime += dt;
	p->position += p->velocity * dt;	
}

void ParticleSystem::draw() {
	//setup drawing environment
	glBindVertexArray(model.vao);
	glUseProgram(model.shader.program); //only the shader is necessary for uniforms
	//update camera
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(camera.view));
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(camera.proj));

	//draw all the particles
	/*
	for (int i = 0; i < particles.size(); i++) {
		//get particle data
		drawParticle(particles[i]);
	}
	*/

	//hack for the smoke particles (there has to be a better way to design this)
	for (int i = particles.size() - 1; i >= 0; i--) {
		//get particle data
		drawParticle(particles[i]);
	}
		
}

void ParticleSystem::drawParticle(Particle* p) {

	//update transform
	glm::mat4 transform = glm::mat4(); //would it be better to do all this math in the shader??
	transform = glm::translate(transform, p->position);
	transform = glm::scale(transform, p->scale);
	transform = glm::rotate(transform, glm::radians(p->rotation.x), glm::vec3(1,0,0));
	transform = glm::rotate(transform, glm::radians(p->rotation.y), glm::vec3(0,1,0));
	transform = glm::rotate(transform, glm::radians(p->rotation.z), glm::vec3(0,0,1));

	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(transform));

	//draw particle
	drawModel(model);

}