#pragma once

#include <string>
using std::string;

#include "utils.h"
#include "globals.h"

class Boat {

	public:
		static void InitModel();
		static void DestroyModel();

		Boat();
		void update(float dt);
		void draw();
		void onkeydown(string keyname);
		void onkeyup(string keyname);

	private:
		static glModelData model;
		static GLint uniProj;
		static GLint uniView;
		static GLint uniModel;
		static GLint uniWave;

		glm::mat4 transform;
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

		float speed;
		bool movingLeft, movingRight, movingForward, movingBack;

		float totalTime;

		glm::vec3 goalRot, startRot;
		float goalRotTimer;
};