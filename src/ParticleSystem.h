#pragma once

#include "utils.h"
#include "globals.h"
#include <vector>

struct Particle {
	glm::vec3 position, scale, rotation;
	glm::vec3 velocity;
	float lifetime;
};

class ParticleSystem {
	public:
		ParticleSystem();
		void update(float dt);
		void draw();

		virtual void init();
		void cleanup();

	protected:
		//can't use statics since then all base classes would use it
		//(interesting problem for future architecture)
		glModelData model;
		GLint uniProj;
		GLint uniView;
		GLint uniModel;

		glm::vec3 position;

		vector<Particle> particles;

		float lifetimeMax;

		float particleBurstTimer;
		float maxTimeBetweenBursts;
		int particleBurstCount;

		virtual Particle generateParticle();
		void updateParticle(Particle* p, float dt);
};