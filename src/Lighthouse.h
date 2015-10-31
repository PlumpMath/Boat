#pragma once

#include "utils.h"
#include "globals.h"

class Lighthouse {
	public:
		static void InitModel();
		static void DestroyModel();

		Lighthouse();
		void update(float dt);
		void draw();
		void moveTo(glm::vec3 pos, float t);

		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

	private:
		static glModelData model;
		static GLint uniProj;
		static GLint uniView;
		static GLint uniModel;
		static GLint uniStorm;
		static GLint uniLightning;

		glm::mat4 transform;

		glm::vec3 prevPos, goalPos;
		float moveTimer, moveTimerMax;
};