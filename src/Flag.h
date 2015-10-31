#pragma once

#include "utils.h"
#include "globals.h"

class Flag {
	public:
		static void InitModel();
		static void DestroyModel();

		Flag();
		void update(float dt, glm::vec3 anchorPos, glm::vec3 anchorRot);
		void draw();
		void setRotYGoal(float rotY);

		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

		bool isAttachedToBoat = true;

	private:
		static glModelData model;
		static GLint uniProj;
		static GLint uniView;
		static GLint uniModel;
		static GLint uniWave;
		static GLint uniStorm;
		static GLint uniLightning;

		glm::mat4 transform;

		float totalTime;

		//rotation
		float prevRotY, nextRotY, curRotY;
		float rotTimer;

		//extension
		float stiffnessTimer;
};
