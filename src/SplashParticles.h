#pragma once

#include "utils.h"
#include "globals.h"
#include "ParticleSystem.h"

class SplashParticles: public ParticleSystem {

	public:
		SplashParticles();
		void init();
		void splash(glm::vec3 pos);

	protected:
		Particle* generateParticle(int i);
		void updateParticle(Particle* p, float dt);
		void drawParticle(Particle* p);

		GLint uniAlpha;
		float gravity = 4;

};