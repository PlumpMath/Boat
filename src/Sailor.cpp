#include "Sailor.h"

#include <iostream>

glModelData Sailor::model_body;
GLint Sailor::uniProj_body;
GLint Sailor::uniView_body;
GLint Sailor::uniModel_body;

void Sailor::InitGeometry() {
	GLfloat vertices[] = { //test geometry
		-1, 1, 1,
		1, 1, 1,
		1, -1, 1,
		-1, -1, 1,
		-1, 1, -1,
		1, 1, -1,
		1, -1, -1,
		-1, -1, -1,
	};

	GLuint elements[] = {
		0, 1, 2,
		0, 3, 2,
		4, 5, 6,
		4, 7, 6,
		0, 4, 5,
		0, 5, 1,
	};

	Sailor::model_body = buildModel(
		"sailor_vert.glsl", "sailor_frag.glsl",
		vertices, sizeof(vertices),
		elements, sizeof(elements),
		{
			{"position", 3}
		}
	);

	Sailor::uniProj_body = glGetUniformLocation(Sailor::model_body.shader.program, "proj");
	Sailor::uniView_body = glGetUniformLocation(Sailor::model_body.shader.program, "view");
	Sailor::uniModel_body = glGetUniformLocation(Sailor::model_body.shader.program, "model");
}

void Sailor::DestroyGeometry() {
	destroyModel(Sailor::model_body);
}

Sailor::Sailor() {
	position = randomPositionInDropZone();
	rotation = glm::vec3(0,0,0);
	scale = glm::vec3(0.1, 0.1, 0.1);

	mode = MODE_DRIFTING;
}

Sailor::Sailor(glm::vec3 tossStart, glm::vec3 tossDir) {
	//position = randomPositionInDropZone();
	float r = ((rand() % 100) / 100.0f);
	dropPosition = tossStart + (tossDir * (minTossDist + ((maxTossDist - minTossDist) * r)));
	startPosition = tossStart;
	fallingTimer = 1.0f;
	mode = MODE_FALLING;

	position = startPosition;

	rotation = glm::vec3(0,0,0);
	scale = glm::vec3(0.1, 0.1, 0.1);
}

glm::vec3 Sailor::randomPositionInDropZone() {
	float rad = glm::radians( (rand() % 360) * 1.0f );
	float dist = dropRadius * ((rand() % 100) / 100.0f);
	glm::vec3 unit = glm::vec3( cos(rad), 0, sin(rad) );
	glm::vec3 pos = unit * dist;
	return pos;
}

void Sailor::update(float dt) {
	if (mode == MODE_DRIFTING) {
		position.z -= driftSpeed * dt; //drift away
	}
	else if (mode == MODE_FALLING) {
		fallingTimer -= dt;

		if (fallingTimer > 0) {
			//the sailor falls along a neville's curve (it's inefficient to do this calculation every frame, but oh well)
			glm::vec3 a = startPosition;
			glm::vec3 c = dropPosition;
			glm::vec3 b = (a + ((c - a) * 0.5f)) + glm::vec3(0, 1, 0);

			glm::vec3 ab = b - a;
			glm::vec3 cb = b - c;

			glm::vec3 abPos = a + (ab * 2.0f * (1 - fallingTimer));
			glm::vec3 cbPos = c + (cb * 2.0f * (fallingTimer));

			glm::vec3 nevillePos = abPos + ((cbPos - abPos) * (1 - fallingTimer));

			position = nevillePos; //update the sailor
		}
		else {
			position = dropPosition;
			mode = MODE_DRIFTING;
		}
	}
	else if (mode == MODE_RESCUING) {
		rescueTimer -= dt;

		if (rescueTimer > 0) {
			//resuce animation
		}
		else {
			scale = glm::vec3(0,0,0);
			isRescued = true;
		}
	}
	

	updateTransform();
}

void Sailor::updateTransform() {
	//update transform
	transform = glm::mat4(); //return to identity matrix

	transform = glm::translate(transform, position); //position
	transform = glm::scale(transform, scale); //scale

	//rotation
	transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1,0,0));
	transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0,1,0));
	transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0,0,1));
}

bool Sailor::collisionWithBoat(glm::vec3 boatPosition) {
	//std::cout << glm::distance(boatPosition, position) << std::endl;
	return (mode == MODE_DRIFTING) && (glm::distance(boatPosition, position) < 0.6f); //test number
}

bool Sailor::goneOffScreen() {
	return position.z <= -2.3f;
}

void Sailor::rescue(glm::vec3 boatPosition) {
	startPosition = position;
	rescuePosition = boatPosition;
	rescueTimer = 0.5f;

	mode == MODE_RESCUING;
}

void Sailor::draw() {
	//bind model --- abstract into function?
	glBindVertexArray(Sailor::model_body.vao);
	glUseProgram(Sailor::model_body.shader.program); //only the shader is necessary for uniforms

	//update camera
	glUniformMatrix4fv(Sailor::uniView_body, 1, GL_FALSE, glm::value_ptr(camera.view));
	glUniformMatrix4fv(Sailor::uniProj_body, 1, GL_FALSE, glm::value_ptr(camera.proj));

	//update transform
	glUniformMatrix4fv(Sailor::uniModel_body, 1, GL_FALSE, glm::value_ptr(transform));

	//draw
	drawModel(Sailor::model_body);
}