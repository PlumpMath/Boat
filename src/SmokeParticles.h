#pragma once

#include "utils.h"
#include "globals.h"
#include "ParticleSystem.h"

struct SmokeParticle : Particle {
	float rotationSpeed;
};

class SmokeParticles: public ParticleSystem {

	public:
		SmokeParticles();
		void init();

		void pause();
		void unpause();

		glm::vec3 position;
		bool isPaused = false;

	protected:
		Particle* generateParticle(int i);
		void updateParticle(Particle* p, float dt);
		void drawParticle(Particle* p);

		GLint uniAlpha;
};