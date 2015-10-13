#pragma once

#include "utils.h"
#include "globals.h"

class Boat {

	public:
		static void InitModel();
		static void DestroyModel();

		Boat();
		void update(float dt);
		void draw();

	private:
		static glModelData model;
		static GLint uniProj;
		static GLint uniView;
		static GLint uniModel;

		glm::mat4 transform;
};