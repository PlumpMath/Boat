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
		static GLint uniColor1_body;
		static GLint uniColor2_body;
		static GLint uniLightning_body;
		static GLint uniStorm_body;

		//floatie
		static glModelData model_floatie;
		static GLint uniProj_floatie;
		static GLint uniView_floatie;
		static GLint uniModel_floatie;
		static GLint uniColor1_floatie;
		static GLint uniColor2_floatie;
		static GLint uniLightning_floatie;
		static GLint uniStorm_floatie;

		//limbs
		static glModelData model_limb;
		static GLint uniProj_limb;
		static GLint uniView_limb;
		static GLint uniModel_limb;
		static GLint uniColor1_limb;
		static GLint uniColor2_limb;
		static GLint uniLightning_limb;
		static GLint uniStorm_limb;

		glm::mat4 transform;
		glm::vec3 baseScale = glm::vec3(0.025, 0.025, 0.025);

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

		glm::vec3 skintoneChoice;

		float splashDownDepth;

		void updateTransform();
		glm::vec3 randomPositionInDropZone();
};