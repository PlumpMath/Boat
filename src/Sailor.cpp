#include "Sailor.h"

#include <iostream>

glModelData Sailor::model_body;
GLint Sailor::uniProj_body;
GLint Sailor::uniView_body;
GLint Sailor::uniModel_body;
GLint Sailor::uniColor1_body;
GLint Sailor::uniColor2_body;
GLint Sailor::uniLightning_body;

glModelData Sailor::model_floatie;
GLint Sailor::uniProj_floatie;
GLint Sailor::uniView_floatie;
GLint Sailor::uniModel_floatie;
GLint Sailor::uniColor1_floatie;
GLint Sailor::uniColor2_floatie;
GLint Sailor::uniLightning_floatie;

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

	float headRadius = 2;
	int numHeadSlices = 8;

	int countTorsoVertices = numLayers * numVerticesPerLayer * 4;
	int countTorsoElements = (numLayers - 1) * numVerticesPerLayer * 2 * 3;
	int countHeadVertices = numHeadSlices * numHeadSlices * 4;
	int countHeadElements = (numHeadSlices - 1) * numHeadSlices * 2 * 3;

	GLfloat vertices[countTorsoVertices + countHeadVertices];
	GLuint elements[countTorsoElements + countHeadElements];

	for (int i = 0; i < numLayers; i++) {
		for (int j = 0; j < numVerticesPerLayer; j++) {

			float deg = 360.0f * ((j * 1.0f) / (numVerticesPerLayer * 1.0f)); //hack cast to float
			float rad = glm::radians(deg);

			float heightPercent = ((i * 1.0f) / (numLayers - 1));
			float curWidth = maxWidth + (heightPercent * (minWidth - maxWidth));

			float x = cos(rad) * curWidth;
			float y = height * heightPercent; 
			float z = sin(rad) * curWidth;

			vertices[ (i * 4 * numVerticesPerLayer) + (j * 4) + 0 ] = x;
			vertices[ (i * 4 * numVerticesPerLayer) + (j * 4) + 1 ] = y;
			vertices[ (i * 4 * numVerticesPerLayer) + (j * 4) + 2 ] = z;
			vertices[ (i * 4 * numVerticesPerLayer) + (j * 4) + 3 ] = 1; //color choice

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

			vertices[ countTorsoVertices + (i * 4 * numHeadSlices) + (j * 4) + 0 ] = x;
			vertices[ countTorsoVertices + (i * 4 * numHeadSlices) + (j * 4) + 1 ] = y;
			vertices[ countTorsoVertices + (i * 4 * numHeadSlices) + (j * 4) + 2 ] = z;
			vertices[ countTorsoVertices + (i * 4 * numHeadSlices) + (j * 4) + 3 ] = 0; //color choice

			if (i < numHeadSlices - 1) {
				int vert1 = countTorsoVertices/4 + (i * numHeadSlices) + j;
				int vert2 = countTorsoVertices/4 + (i * numHeadSlices) + ((j + 1) % numHeadSlices);
				int vert3 = countTorsoVertices/4 + ((i + 1) * numHeadSlices) + j;
				int vert4 = countTorsoVertices/4 + ((i + 1) * numHeadSlices) + ((j + 1) % numHeadSlices);

				elements[ countTorsoElements + (i * 6 * numHeadSlices) + (j * 6) + 0 ] = vert1;
				elements[ countTorsoElements + (i * 6 * numHeadSlices) + (j * 6) + 1 ] = vert2;
				elements[ countTorsoElements + (i * 6 * numHeadSlices) + (j * 6) + 2 ] = vert4;
				elements[ countTorsoElements + (i * 6 * numHeadSlices) + (j * 6) + 3 ] = vert1;
				elements[ countTorsoElements + (i * 6 * numHeadSlices) + (j * 6) + 4 ] = vert3;
				elements[ countTorsoElements + (i * 6 * numHeadSlices) + (j * 6) + 5 ] = vert4;
			}
		}
	}

	Sailor::model_body = buildModel(
		"sailor_vert.glsl", "sailor_frag.glsl",
		vertices, sizeof(vertices),
		elements, sizeof(elements),
		{
			{"position", 3},
			{"colorChoice", 1}
		}
	);

	Sailor::uniProj_body = glGetUniformLocation(Sailor::model_body.shader.program, "proj");
	Sailor::uniView_body = glGetUniformLocation(Sailor::model_body.shader.program, "view");
	Sailor::uniModel_body = glGetUniformLocation(Sailor::model_body.shader.program, "model");
	Sailor::uniColor1_body = glGetUniformLocation(Sailor::model_body.shader.program, "primaryColor");
	Sailor::uniColor2_body = glGetUniformLocation(Sailor::model_body.shader.program, "secondaryColor");
	Sailor::uniLightning_body = glGetUniformLocation(Sailor::model_body.shader.program, "lightningTimer");


	glBindVertexArray(Sailor::model_body.vao);
	glUseProgram(Sailor::model_body.shader.program);

	glUniform3f(uniColor2_body, 0.2, 0.4, 1);

	glBindVertexArray(0);

	//FLOATIE
	float floatieRadius = 4;
	int numFloatieSlices = 20;
	float tubeRadius = 1;
	int numTubeSlices = 8;

	GLfloat vertices_floatie[numFloatieSlices * numTubeSlices * 4];
	GLuint elements_floatie[numFloatieSlices * numTubeSlices * 2 * 3];

	for (int i = 0; i < numFloatieSlices; i++) {
		
		float floatieDeg = 360.0f * ((i * 1.0f) / (numFloatieSlices - 1));
		float floatieRadians = glm::radians(floatieDeg);
		glm::vec3 unit = glm::vec3(sin(floatieRadians), 0, cos(floatieRadians));
		glm::vec3 centerPos = unit * floatieRadius;

		for (int j = 0; j < numTubeSlices; j++) {

			float tubeDeg = 360.0f * ((j * 1.0f) / (numTubeSlices - 1));
			float tubeRadians = glm::radians(tubeDeg);

			float s = sin(tubeRadians);
			glm::vec3 tubePos = (unit * s) + glm::vec3(0, cos(tubeRadians), 0);
			tubePos = tubePos * tubeRadius;
			tubePos = centerPos + tubePos;

			vertices_floatie[ (i * 4 * numTubeSlices) + (j * 4) + 0 ] = tubePos.x;
			vertices_floatie[ (i * 4 * numTubeSlices) + (j * 4) + 1 ] = tubePos.y;
			vertices_floatie[ (i * 4 * numTubeSlices) + (j * 4) + 2 ] = tubePos.z;

			if (i % (numFloatieSlices / 4) == 2) {
				vertices_floatie[ (i * 4 * numTubeSlices) + (j * 4) + 3 ] = 1; //color choice
			}
			else {
				vertices_floatie[ (i * 4 * numTubeSlices) + (j * 4) + 3 ] = 0; //color choice
			}
			
			int vert1 = (i * numTubeSlices) + j;
			int vert2 = (i * numTubeSlices) + ((j + 1) % numTubeSlices);
			int vert3 = (((i + 1) % numFloatieSlices) * numTubeSlices) + j;
			int vert4 = (((i + 1) % numFloatieSlices) * numTubeSlices) + ((j + 1) % numTubeSlices);

			elements_floatie[ (i * 6 * numTubeSlices) + (j * 6) + 0 ] = vert1;
			elements_floatie[ (i * 6 * numTubeSlices) + (j * 6) + 1 ] = vert2;
			elements_floatie[ (i * 6 * numTubeSlices) + (j * 6) + 2 ] = vert4;
			elements_floatie[ (i * 6 * numTubeSlices) + (j * 6) + 3 ] = vert1;
			elements_floatie[ (i * 6 * numTubeSlices) + (j * 6) + 4 ] = vert3;
			elements_floatie[ (i * 6 * numTubeSlices) + (j * 6) + 5 ] = vert4;

			/*
			if (vert4 < vert3) { //for coloring purposes
				elements_floatie[ (i * 6 * numTubeSlices) + (j * 6) + 0 ] = vert1;
				elements_floatie[ (i * 6 * numTubeSlices) + (j * 6) + 1 ] = vert2;
				elements_floatie[ (i * 6 * numTubeSlices) + (j * 6) + 2 ] = vert4;
				elements_floatie[ (i * 6 * numTubeSlices) + (j * 6) + 3 ] = vert1;
				elements_floatie[ (i * 6 * numTubeSlices) + (j * 6) + 4 ] = vert3;
				elements_floatie[ (i * 6 * numTubeSlices) + (j * 6) + 5 ] = vert4;
			}
			*/
		}
	}

	Sailor::model_floatie = buildModel(
		"sailor_vert.glsl", "sailor_frag.glsl",
		vertices_floatie, sizeof(vertices_floatie),
		elements_floatie, sizeof(elements_floatie),
		{
			{"position", 3},
			{"colorChoice", 1}
		}
	);

	Sailor::uniProj_floatie = glGetUniformLocation(Sailor::model_floatie.shader.program, "proj");
	Sailor::uniView_floatie = glGetUniformLocation(Sailor::model_floatie.shader.program, "view");
	Sailor::uniModel_floatie = glGetUniformLocation(Sailor::model_floatie.shader.program, "model");
	Sailor::uniColor1_floatie = glGetUniformLocation(Sailor::model_floatie.shader.program, "primaryColor");
	Sailor::uniColor2_floatie = glGetUniformLocation(Sailor::model_floatie.shader.program, "secondaryColor");
	Sailor::uniLightning_floatie = glGetUniformLocation(Sailor::model_floatie.shader.program, "lightningTimer");

	glBindVertexArray(Sailor::model_floatie.vao);
	glUseProgram(Sailor::model_floatie.shader.program);

	glUniform3f(uniColor1_floatie, 1, 1, 1);
	glUniform3f(uniColor2_floatie, 1, 0.2, 0);

	glBindVertexArray(0);
}

void Sailor::DestroyGeometry() {
	destroyModel(Sailor::model_body);
	destroyModel(Sailor::model_floatie);
}

Sailor::Sailor() {
	position = randomPositionInDropZone();
	rotation = glm::vec3(0,0,0);
	scale = glm::vec3(baseScale.x, baseScale.y, baseScale.z);

	mode = MODE_DRIFTING;
}

Sailor::Sailor(glm::vec3 tossStart, glm::vec3 tossDir) {
	//float r = ((rand() % 100) / 100.0f);
	//dropPosition = tossStart + (tossDir * (minTossDist + ((maxTossDist - minTossDist) * r)));

	std::cout << "NEW SAILOR" << std::endl;

	dropPosition = randomPositionInDropZone();
	startPosition = tossStart;
	fallingTimer = 1.0f;
	mode = MODE_FALLING;

	position = startPosition;

	rotation = glm::vec3(0,0,0);
	scale = glm::vec3(baseScale.x, baseScale.y, baseScale.z);

	std::cout << scale.x << std::endl;

	//skintone
	glm::vec3 skintone1 = glm::vec3(0.26, 0, 0);
	glm::vec3 skintone2 = glm::vec3(0.73, 0.42, 0.29);
	glm::vec3 skintone3 = glm::vec3(1, 0.87, 0.77);

	skintoneChoice = glm::vec3(0, 1, 0);
	int choice = rand() % 3;
	if (choice == 0) {
		skintoneChoice = skintone1;
	}
	else if (choice == 1) {
		skintoneChoice = skintone2;
	}
	else if (choice == 2) {
		skintoneChoice = skintone3;
	}

	std::cout << skintoneChoice.x << " " << skintoneChoice.y << " " << skintoneChoice.z << std::endl;
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
				scale = (rescueTimer / 0.1f) * baseScale; //shrink
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

	//special effects
	glUniform3f(uniColor1_body, this->skintoneChoice.x, this->skintoneChoice.y, this->skintoneChoice.z);
	glUniform1f(uniLightning_body, lightningTimer);

	//draw
	drawModel(Sailor::model_body);

	//

	//bind model --- abstract into function?
	glBindVertexArray(Sailor::model_floatie.vao);
	glUseProgram(Sailor::model_floatie.shader.program); //only the shader is necessary for uniforms

	//update camera
	glUniformMatrix4fv(Sailor::uniView_floatie, 1, GL_FALSE, glm::value_ptr(camera.view));
	glUniformMatrix4fv(Sailor::uniProj_floatie, 1, GL_FALSE, glm::value_ptr(camera.proj));

	//update transform
	glUniformMatrix4fv(Sailor::uniModel_floatie, 1, GL_FALSE, glm::value_ptr(transform));

	//special effects
	glUniform1f(uniLightning_floatie, lightningTimer);

	//draw
	drawModel(Sailor::model_floatie);
}