#pragma once

#include "utils.h"
#include "globals.h"
#include "ParticleSystem.h"

class RainParticles: public ParticleSystem {

	public:
		RainParticles();
		void update(float dt);
		void draw();
		void init();

	protected:
		GLint uniLightning;

		float xRange;
		float zRange;

		glm::vec3 windDirection;

		float minSpeed = 8;
		float maxSpeed = 12;

		Particle* generateParticle(int i);

};