#pragma once

#include "utils.h"
#include "globals.h"

class Sailor {
	public:
		static void InitGeometry();
		static void DestroyGeometry();

		Sailor();
		void update(float dt);
		void draw();

		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

	private:
		static glModelData model_body;
		static GLint uniProj_body;
		static GLint uniView_body;
		static GLint uniModel_body;

		glm::mat4 transform;

		float driftSpeed = 0.2f;

		float dropRadius = 1.5f;
		glm::vec3 dropPosition;

		void updateTransform();
		glm::vec3 randomPositionInDropZone();
};