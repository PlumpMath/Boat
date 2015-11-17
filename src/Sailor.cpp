#include "Sailor.h"

#include <iostream>

glModelData Sailor::model_body;
GLint Sailor::uniProj_body;
GLint Sailor::uniView_body;
GLint Sailor::uniModel_body;

glModelData Sailor::model_head;
GLint Sailor::uniProj_head;
GLint Sailor::uniView_head;
GLint Sailor::uniModel_head;

void Sailor::InitGeometry() {
	/*
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
	*/

	//BODY
	float minWidth = 1;
	float maxWidth = 2;
	float height = 3;
	int numLayers = 6;
	int numVerticesPerLayer = 8;

	GLfloat vertices[numLayers * numVerticesPerLayer * 3];
	GLuint elements[(numLayers - 1) * numVerticesPerLayer * 2 * 3];

	for (int i = 0; i < numLayers; i++) {
		for (int j = 0; j < numVerticesPerLayer; j++) {

			float deg = 360.0f * ((j * 1.0f) / (numVerticesPerLayer * 1.0f)); //hack cast to float
			float rad = glm::radians(deg);

			float heightPercent = ((i * 1.0f) / (numLayers - 1));
			float curWidth = maxWidth + (heightPercent * (minWidth - maxWidth));

			float x = cos(rad) * curWidth;
			float y = height * heightPercent; 
			float z = sin(rad) * curWidth;

			vertices[ (i * 3 * numVerticesPerLayer) + (j * 3) + 0 ] = x;
			vertices[ (i * 3 * numVerticesPerLayer) + (j * 3) + 1 ] = y;
			vertices[ (i * 3 * numVerticesPerLayer) + (j * 3) + 2 ] = z;

			if (i < numLayers - 1) {
				int vert1 = (i * numVerticesPerLayer) + j;
				int vert2 = (i * numVerticesPerLayer) + ((j + 1) % numVerticesPerLayer);
				int vert3 = ((i + 1) * numVerticesPerLayer) + j;
				int vert4 = ((i + 1) * numVerticesPerLayer) + ((j + 1) % numVerticesPerLayer);

				elements[ (i * 6 * numVerticesPerLayer) + (j * 6) + 0 ] = vert1;
				elements[ (i * 6 * numVerticesPerLayer) + (j * 6) + 1 ] = vert2;
				elements[ (i * 6 * numVerticesPerLayer) + (j * 6) + 2 ] = vert4;
				elements[ (i * 6 * numVerticesPerLayer) + (j * 6) + 3 ] = vert1;
				elements[ (i * 6 * numVerticesPerLayer) + (j * 6) + 4 ] = vert3;
				elements[ (i * 6 * numVerticesPerLayer) + (j * 6) + 5 ] = vert4;
			}
		}
	}

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


	//HEAD
	float headRadius = 2;
	int numHeadSlices = 8;

	GLfloat vertices_head[numHeadSlices * numHeadSlices * 3];
	GLuint elements_head[(numHeadSlices - 1) * numHeadSlices * 2 * 3];
	
	for (int i = 0; i < numHeadSlices; i++) {
		for (int j = 0; j < numHeadSlices; j++) {
			float deg = 360.0f * ((j * 1.0f) / numHeadSlices); //hack cast to float
			float rad = glm::radians(deg);

			float heightPercent = (i * 1.0f) / (numHeadSlices - 1);
			float heightDeg = 360.0f * heightPercent * 0.5f;
			float heightRad = glm::radians(heightDeg);
			float curWidth = sin(heightRad) * headRadius;

			float x = cos(rad) * curWidth;
			//hack to flatten the head out a bit
			if (i == 0) {
				heightPercent = ((i + 1) * 1.0f) / (numHeadSlices - 1);
			}
			else if (i == numHeadSlices - 1) {
				heightPercent = ((i - 1) * 1.0f) / (numHeadSlices - 1);
			}
			float y = height - 1 + (headRadius * 2 * heightPercent); 
			float z = sin(rad) * curWidth;

			vertices_head[ (i * 3 * numHeadSlices) + (j * 3) + 0 ] = x;
			vertices_head[ (i * 3 * numHeadSlices) + (j * 3) + 1 ] = y;
			vertices_head[ (i * 3 * numHeadSlices) + (j * 3) + 2 ] = z;

			if (i < numHeadSlices - 1) {
				int vert1 = (i * numHeadSlices) + j;
				int vert2 = (i * numHeadSlices) + ((j + 1) % numHeadSlices);
				int vert3 = ((i + 1) * numHeadSlices) + j;
				int vert4 = ((i + 1) * numHeadSlices) + ((j + 1) % numHeadSlices);

				elements_head[ (i * 6 * numHeadSlices) + (j * 6) + 0 ] = vert1;
				elements_head[ (i * 6 * numHeadSlices) + (j * 6) + 1 ] = vert2;
				elements_head[ (i * 6 * numHeadSlices) + (j * 6) + 2 ] = vert4;
				elements_head[ (i * 6 * numHeadSlices) + (j * 6) + 3 ] = vert1;
				elements_head[ (i * 6 * numHeadSlices) + (j * 6) + 4 ] = vert3;
				elements_head[ (i * 6 * numHeadSlices) + (j * 6) + 5 ] = vert4;
			}
		}
	}

	Sailor::model_head = buildModel(
		"sailor_vert.glsl", "sailor_frag.glsl",
		vertices_head, sizeof(vertices_head),
		elements_head, sizeof(elements_head),
		{
			{"position", 3}
		}
	);

	Sailor::uniProj_head = glGetUniformLocation(Sailor::model_head.shader.program, "proj");
	Sailor::uniView_head = glGetUniformLocation(Sailor::model_head.shader.program, "view");
	Sailor::uniModel_head = glGetUniformLocation(Sailor::model_head.shader.program, "model");
}

void Sailor::DestroyGeometry() {
	destroyModel(Sailor::model_body);
	destroyModel(Sailor::model_head);
}

Sailor::Sailor() {
	position = randomPositionInDropZone();
	rotation = glm::vec3(0,0,0);
	scale = glm::vec3(0.1, 0.1, 0.1);

	mode = MODE_DRIFTING;
}

Sailor::Sailor(glm::vec3 tossStart, glm::vec3 tossDir) {
	//float r = ((rand() % 100) / 100.0f);
	//dropPosition = tossStart + (tossDir * (minTossDist + ((maxTossDist - minTossDist) * r)));

	dropPosition = randomPositionInDropZone();
	startPosition = tossStart;
	fallingTimer = 1.0f;
	mode = MODE_FALLING;

	position = startPosition;

	rotation = glm::vec3(0,0,0);
	scale = glm::vec3(0.05, 0.05, 0.05);
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
			if (rescueTimer > 0.1f) {
				position = startPosition + ( (1 - ((rescueTimer - 0.1f) / 0.4f)) * glm::vec3(0, 1.5, 0) );
				std::cout << position.y << std::endl;
			}
			else {
				glm::vec3 upPos = startPosition + glm::vec3(0, 1.5, 0);
				glm::vec3 endPos = boat.worldPosition() + glm::vec3(0, 0.5, 0);
				position = upPos + ( (1 - ((rescueTimer) / 0.1f)) * (endPos - upPos) );
				scale = (rescueTimer / 0.1f) * glm::vec3(0.05, 0.05, 0.05); //shrink
			}
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

void Sailor::rescue() {
	startPosition = glm::vec3(position.x, position.y, position.z);
	rescueTimer = 0.5f;

	mode = MODE_RESCUING;
}

bool Sailor::rescued() {
	return isRescued;
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

	//bind model --- abstract into function?
	glBindVertexArray(Sailor::model_head.vao);
	glUseProgram(Sailor::model_head.shader.program); //only the shader is necessary for uniforms

	//update camera
	glUniformMatrix4fv(Sailor::uniView_head, 1, GL_FALSE, glm::value_ptr(camera.view));
	glUniformMatrix4fv(Sailor::uniProj_head, 1, GL_FALSE, glm::value_ptr(camera.proj));

	//update transform
	glUniformMatrix4fv(Sailor::uniModel_head, 1, GL_FALSE, glm::value_ptr(transform));

	//draw
	drawModel(Sailor::model_head);
}