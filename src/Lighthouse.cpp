#include "Lighthouse.h"

#include <iostream>

glModelData Lighthouse::model;
GLint Lighthouse::uniProj;
GLint Lighthouse::uniView;
GLint Lighthouse::uniModel;
GLint Lighthouse::uniStorm;
GLint Lighthouse::uniLightning;

void Lighthouse::InitModel() {
	GLfloat vertices[] = {
		//x, y, z,	r, g, b,

		//0
		10, 0, 0,		1, 1, 1,
		5, 0, -10,		1, 1, 1,
		-5, 0, -10,		1, 1, 1,
		-10, 0, 0,		1, 1, 1,
		-5, 0, 10,		1, 1, 1,
		5, 0, 10,		1, 1, 1,
		//6
		10, 8, 0,		1, 1, 1,
		5, 8, -10,		1, 1, 1,
		-5, 8, -10,		1, 1, 1,
		-10, 8, 0,		1, 1, 1,
		-5, 8, 10,		1, 1, 1,
		5, 8, 10,		1, 1, 1,

		//12
		8.0,	 12,	 0,			1, 1, 1,
		4.0,	 12,	 -8.0, 		1, 1, 1,
		-4.0,	 12,	 -8.0, 		1, 1, 1,
		-8.0,	 12,	 0,			1, 1, 1,
		-4.0,	 12,	 8.0, 		1, 1, 1,
		4.0,	 12,	 8.0, 		1, 1, 1,
		//18
		7.7,	 16,	 0,			1, 0, 0,
		3.85,	 16,	 -7.7,			1, 0, 0,
		-3.85,	 16,	 -7.7,			1, 0, 0,
		-7.7,	 16,	 0,			1, 0, 0,
		-3.85,	 16,	 7.7,			1, 0, 0,
		3.85,	 16,	 7.7,			1, 0, 0,
		//24
		7.4,	 20,	 0,			1, 1, 1, 
		3.7,	 20,	 -7.4,			1, 1, 1, 
		-3.7,	 20,	 -7.4,			1, 1, 1, 
		-7.4,	 20,	 0,			1, 1, 1, 
		-3.7,	 20,	 7.4,			1, 1, 1, 
		3.7,	 20,	 7.4,			1, 1, 1, 
		//30
		7.1,	 24,	 0,			1, 0, 0,
		3.55,	 24,	 -7.1,			1, 0, 0,
		-3.55,	 24,	 -7.1,			1, 0, 0,
		-7.1,	 24,	 0,			1, 0, 0,
		-3.55,	 24,	 7.1,			1, 0, 0,
		3.55,	 24,	 7.1,			1, 0, 0,
		//36
		6.8,	 28,	 0,			1, 1, 1, 
		3.4,	 28,	 -6.8,			1, 1, 1, 
		-3.4,	 28,	 -6.8,			1, 1, 1, 
		-6.8,	 28,	 0,			1, 1, 1, 
		-3.4,	 28,	 6.8,			1, 1, 1, 
		3.4,	 28,	 6.8,			1, 1, 1, 
		//42
		6.5,	 32,	 0,			1, 0, 0,
		3.25,	 32,	 -6.5,			1, 0, 0,
		-3.25,	 32,	 -6.5,			1, 0, 0,
		-6.5,	 32,	 0,			1, 0, 0,
		-3.25,	 32,	 6.5,			1, 0, 0,
		3.25,	 32,	 6.5,			1, 0, 0,
		//48
		6.2,	 36,	 0,			1, 1, 1,
		3.1,	 36,	 -6.2,			1, 1, 1,
		-3.1,	 36,	 -6.2,			1, 1, 1,
		-6.2,	 36,	 0,			1, 1, 1,
		-3.1,	 36,	 6.2,			1, 1, 1,
		3.1,	 36,	 6.2,			1, 1, 1,
		//54
		5.9,	 40,	 0,			1, 0, 0,
		2.95,	 40,	 -5.9,		1, 0, 0,
		-2.95,	 40,	 -5.9,		1, 0, 0,
		-5.9,	 40,	 0,			1, 0, 0,
		-2.95,	 40,	 5.9,		1, 0, 0,
		2.95,	 40,	 5.9,		1, 0, 0,

		//66
		8, 46, 0,		1,1,1,
		4, 46, -8,		1,1,1,
		-4, 46, -8,		1,1,1,
		-8, 46, 0,		1,1,1,
		-4, 46, 8,		1,1,1,
		4, 46, 8,		1,1,1,

		//66
		6, 50, 0,		1,0,0,
		3, 50, -6,		1,0,0,
		-3, 50, -6,		1,0,0,
		-6, 50, 0,		1,0,0,
		-3, 50, 6,		1,0,0,
		3, 50, 6,		1,0,0,
		//72
		0, 54, 0,		1,0,0,

	};

	GLuint elements[] = {
		//
		0, 6, 7,
		0, 1, 7,
		1, 7, 8,
		1, 2, 8,
		2, 8, 9,
		2, 3, 9,
		3, 9, 10,
		3, 4, 10,
		4, 10, 11,
		4, 5, 11,
		5, 11, 6,
		5, 0, 6,
		//
		6, 12, 13,
		6, 7, 13,
		7, 13, 14,
		7, 8, 14,
		8, 14, 15,
		8, 9, 15,
		9, 15, 16,
		9, 10, 16,
		10, 16, 17,
		10, 11, 17,
		11, 17, 12,
		11, 6, 12,
		//
		12, 18, 19,
		12, 13, 19,
		13, 19, 20,
		13, 14, 20,
		14, 20, 21,
		14, 15, 21,
		15, 21, 22,
		15, 16, 22,
		16, 22, 23,
		16, 17, 23,
		17, 23, 18,
		17, 12, 18,
		//
		18, 24, 25,
		18, 19, 25,
		19, 25, 26,
		19, 20, 26,
		20, 26, 27,
		20, 21, 27,
		21, 27, 28,
		21, 22, 28,
		22, 28, 29,
		22, 23, 29,
		23, 29, 24,
		23, 18, 24,
		//
		24, 30, 31,
		24, 25, 31,
		25, 31, 32,
		25, 26, 32,
		26, 32, 33,
		26, 27, 33,
		27, 33, 34,
		27, 28, 34,
		28, 34, 35,
		28, 29, 35,
		29, 35, 30,
		29, 24, 30,
		//
		30, 36, 37,
		30, 31, 37,
		31, 37, 38,
		31, 32, 38,
		32, 38, 39,
		32, 33, 39,
		33, 39, 40,
		33, 34, 40,
		34, 40, 41,
		34, 35, 41,
		35, 41, 36,
		35, 30, 36,
		//
		36, 42, 43,
		36, 37, 43,
		37, 43, 44,
		37, 38, 44,
		38, 44, 45,
		38, 39, 45,
		39, 45, 46,
		39, 40, 46,
		40, 46, 47,
		40, 41, 47,
		41, 47, 42,
		41, 36, 42,
		//
		42, 48, 49,
		42, 43, 49,
		43, 49, 50,
		43, 44, 50,
		44, 50, 51,
		44, 45, 51,
		45, 51, 52,
		45, 46, 52,
		46, 52, 53,
		46, 47, 53,
		47, 53, 48,
		47, 42, 48,
		//
		48, 54, 55,
		48, 49, 55,
		49, 55, 56,
		49, 50, 56,
		50, 56, 57,
		50, 51, 57,
		51, 57, 58,
		51, 52, 58,
		52, 58, 59,
		52, 53, 59,
		53, 59, 54,
		53, 48, 54,
		//
		54, 60, 61,
		54, 55, 61,
		55, 61, 62,
		55, 56, 62,
		56, 62, 63,
		56, 57, 63,
		57, 63, 64,
		57, 58, 64,
		58, 64, 65,
		58, 59, 65,
		59, 65, 60,
		59, 54, 60,

		//
		66, 72, 67,
		67, 72, 68,
		68, 72, 69,
		69, 72, 70,
		70, 72, 71,
		71, 72, 66,
	};

	Lighthouse::model = buildModel(
		"lighthouse_vert.glsl", "lighthouse_frag.glsl",
		vertices, sizeof(vertices),
		elements, sizeof(elements),
		{
			{"position", 3},
			{"color", 3},
		}
	);

	Lighthouse::uniProj = glGetUniformLocation(Lighthouse::model.shader.program, "proj");
	Lighthouse::uniView = glGetUniformLocation(Lighthouse::model.shader.program, "view");
	Lighthouse::uniModel = glGetUniformLocation(Lighthouse::model.shader.program, "model");
	Lighthouse::uniStorm = glGetUniformLocation(Lighthouse::model.shader.program, "storminess");
	Lighthouse::uniLightning = glGetUniformLocation(Lighthouse::model.shader.program, "lightningTimer");
}

void Lighthouse::DestroyModel() {
	destroyModel(Lighthouse::model);
}

Lighthouse::Lighthouse() {
	rotation.x = 15;
	scale = glm::vec3(0.07, 0.07, 0.07);
	
	//below waves
	position = glm::vec3(15, -16, 30);

	//start position
	//position = glm::vec3(15, -13, 30);

	//storm position
	//position = glm::vec3(6, -3.7, 10);

	//end position
	//position = glm::vec3(1.8, -0.4, 2.7);
}

void Lighthouse::moveTo(glm::vec3 pos, float t) {
	moveTimer = 0;
	moveTimerMax = t;
	prevPos = position;
	goalPos = pos;
}
	
//position = glm::vec3(10, -8.5, 20);

void Lighthouse::update(float dt) {
	//rotation.y += 10 * dt;

	if (moveTimer < moveTimerMax) {
		moveTimer += dt;

		if (moveTimer >= moveTimerMax) {
			prevPos = goalPos;
			position = goalPos;
		}
		else {
			position = prevPos + ((goalPos - prevPos) * (moveTimer / moveTimerMax));
		}
	}

	//update transform
	transform = glm::mat4(); //return to identity matrix

	transform = glm::translate(transform, position); //position
	transform = glm::scale( transform, scale); //scale

	//rotation
	transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1,0,0));
	transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0,1,0));
	transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0,0,1));
}

void Lighthouse::draw() {
	glBindVertexArray(Lighthouse::model.vao);
	glUseProgram(Lighthouse::model.shader.program);

	//update camera
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(camera.view));
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(camera.proj));
	
	//update transform
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(transform));

	//special effects
	glUniform1f(uniStorm, storminess);
	glUniform1f(uniLightning, lightningTimer);

	drawModel(Lighthouse::model);
}