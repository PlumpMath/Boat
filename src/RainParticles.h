#pragma once

#include "utils.h"
#include "globals.h"
#include "ParticleSystem.h"

class RainParticles: public ParticleSystem {

	public:
		RainParticles();
		void init();

	protected:
		Particle generateParticle();
		//void updateParticle(Particle p, float dt);
	
};