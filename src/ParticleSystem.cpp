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
			Particle p = generateParticle();
			particles.push_back(p);
		}
		particleBurstTimer = maxTimeBetweenBursts;
	}

	//update particles
	int countdown = particles.size() - 1; //do this backward
	while (!particles.empty() && countdown >= 0) {

		updateParticle(&particles[countdown], dt);

		//destroy particles
		if (particles[countdown].lifetime > lifetimeMax) {
			particles.pop_back(); //remove current particle
		}
		else {
			countdown--; //go to next particle
		}

	}
	
}

Particle ParticleSystem::generateParticle() {
	std::cout << "particle system generate" << std::endl;
	return {
		glm::vec3(0, 0, 0), //pos 
		glm::vec3(1, 1, 1), //scale
		glm::vec3(0, 0, 0), //rot
		glm::vec3(0, 1, 0), //velocity
		0 //lifetime
	};
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
	for (int i = 0; i < particles.size(); i++) {
		//get particle data
		Particle p = particles[i];

		//update transform
		glm::mat4 transform = glm::mat4(); //would it be better to do all this math in the shader??
		transform = glm::translate(transform, p.position);
		transform = glm::scale(transform, p.scale);
		transform = glm::rotate(transform, glm::radians(p.rotation.x), glm::vec3(1,0,0));
		transform = glm::rotate(transform, glm::radians(p.rotation.y), glm::vec3(0,1,0));
		transform = glm::rotate(transform, glm::radians(p.rotation.z), glm::vec3(0,0,1));

		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(transform));

		//draw particle
		drawModel(model);
	}
		
}