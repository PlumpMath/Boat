#pragma once

#include "utils.h"
#include "globals.h"

class Sailor {
	public:
		static void InitGeometry();
		static void DestroyGeometry();

		Sailor();
		Sailor(glm::vec3 tossStart, glm::vec3 tossDir);
		void update(float dt);
		void draw();
		bool collisionWithBoat(glm::vec3 boatPosition);
		bool goneOffScreen();
		void rescue();
		bool rescued();

		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

	private:
		//body
		static glModelData model_body;
		static GLint uniProj_body;
		static GLint uniView_body;
		static GLint uniModel_body;

		//head
		static glModelData model_head;
		static GLint uniProj_head;
		static GLint uniView_head;
		static GLint uniModel_head;

		//arms

		//floatie

		glm::mat4 transform;

		float driftSpeed = 0.2f;

		float minTossDist = 1.0f;
		float maxTossDist = 2.0f;
		float dropRadius = 1.5f;

		glm::vec3 startPosition;
		glm::vec3 dropPosition;
		float fallingTimer;

		float rescueTimer;
		bool isRescued = false;

		int MODE_FALLING = 0;
		int MODE_DRIFTING = 1;
		int MODE_RESCUING = 2;
		int mode;

		void updateTransform();
		glm::vec3 randomPositionInDropZone();
};