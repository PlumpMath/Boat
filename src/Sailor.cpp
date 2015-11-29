#include "Sailor.h"

#include <iostream>

glModelData Sailor::model_body;
GLint Sailor::uniProj_body;
GLint Sailor::uniView_body;
GLint Sailor::uniModel_body;
GLint Sailor::uniColor1_body;
GLint Sailor::uniColor2_body;
GLint Sailor::uniLightning_body;
GLint Sailor::uniStorm_body;

glModelData Sailor::model_floatie;
GLint Sailor::uniProj_floatie;
GLint Sailor::uniView_floatie;
GLint Sailor::uniModel_floatie;
GLint Sailor::uniColor1_floatie;
GLint Sailor::uniColor2_floatie;
GLint Sailor::uniLightning_floatie;
GLint Sailor::uniStorm_floatie;

glModelData Sailor::model_limb;
GLint Sailor::uniProj_limb;
GLint Sailor::uniView_limb;
GLint Sailor::uniModel_limb;
GLint Sailor::uniColor1_limb;
GLint Sailor::uniColor2_limb;
GLint Sailor::uniLightning_limb;
GLint Sailor::uniStorm_limb;

void Sailor::InitGeometry() {
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
	Sailor::uniStorm_body = glGetUniformLocation(Sailor::model_body.shader.program, "storminess");


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
	Sailor::uniStorm_floatie = glGetUniformLocation(Sailor::model_floatie.shader.program, "storminess");

	glBindVertexArray(Sailor::model_floatie.vao);
	glUseProgram(Sailor::model_floatie.shader.program);

	glUniform3f(uniColor1_floatie, 1, 1, 1);
	glUniform3f(uniColor2_floatie, 1, 0.2, 0);

	glBindVertexArray(0);

	//LIMB
	GLfloat vertices_limb[] = {
		-(maxWidth / 4), 0, 0,		0,
		(maxWidth / 4), 0, 0,		0,
		-(maxWidth / 4), -(height * 1.5f),	0,		0,
		(maxWidth / 4), -(height * 1.5f),	0,		0,
		-(maxWidth / 4), 0, -(maxWidth / 4),		0,
		(maxWidth / 4), 0, -(maxWidth / 4),			0,
		-(maxWidth / 4), -(height * 1.5f),	-(maxWidth / 4),		0,
		(maxWidth / 4), -(height * 1.5f),	-(maxWidth / 4),		0,
	};

	GLuint elements_limb[] = {
		0, 1, 2,
		1, 2, 3,

		4, 5, 6,
		5, 6, 7,
		
		0, 4, 5,
		0, 1, 5,
		
		0, 2, 4,
		2, 4, 6,

		1, 3, 5,
		5, 3, 7

	};

	Sailor::model_limb = buildModel(
		"sailor_vert.glsl", "sailor_frag.glsl",
		vertices_limb, sizeof(vertices_limb),
		elements_limb, sizeof(elements_limb),
		{
			{"position", 3},
			{"colorChoice", 1}
		}
	);

	Sailor::uniProj_limb = glGetUniformLocation(Sailor::model_limb.shader.program, "proj");
	Sailor::uniView_limb = glGetUniformLocation(Sailor::model_limb.shader.program, "view");
	Sailor::uniModel_limb = glGetUniformLocation(Sailor::model_limb.shader.program, "model");
	Sailor::uniColor1_limb = glGetUniformLocation(Sailor::model_limb.shader.program, "primaryColor");
	Sailor::uniColor2_limb = glGetUniformLocation(Sailor::model_limb.shader.program, "secondaryColor");
	Sailor::uniLightning_limb = glGetUniformLocation(Sailor::model_limb.shader.program, "lightningTimer");
	Sailor::uniStorm_limb = glGetUniformLocation(Sailor::model_limb.shader.program, "storminess");

	glBindVertexArray(Sailor::model_limb.vao);
	glUseProgram(Sailor::model_limb.shader.program);

	glUniform3f(uniColor2_limb, 1, 1, 1); //set this but we're never actually using it

	glBindVertexArray(0);
}

void Sailor::DestroyGeometry() {
	destroyModel(Sailor::model_body);
	destroyModel(Sailor::model_floatie);
	destroyModel(Sailor::model_limb);
}

Sailor::Sailor() {
	position = randomPositionInDropZone() - glm::vec3(0, 0.5, 0);
	rotation = glm::vec3(0,0,0);
	scale = glm::vec3(baseScale.x, baseScale.y, baseScale.z);

	mode = MODE_DRIFTING;

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

	splashDownDepth = 0.5f;
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

	splashDownDepth = 0.5f;

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

		float bobWithBoat = (0.06 * (1 + storminess) * sin(totalTime * 0.5 * 8) / 2); //lotsa magic numbers
		position.y = bobWithBoat + 0.1f;

		if (splashDownDepth > 0) {
			splashDownDepth -= 1 * dt;
			position.y -= splashDownDepth;
		}
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

			rotation.z = 360 * 2 * fallingTimer;
		}
		else {
			splashes.splash( glm::vec3(dropPosition.x, 0, dropPosition.z) );
			position = dropPosition;
			rotation.z = 0;
			mode = MODE_DRIFTING;
		}
	}
	else if (mode == MODE_RESCUING) {
		rescueTimer -= dt;

		if (rescueTimer > 0) {
			//resuce animation
			float maxHeight = 0.8f;
			if (rescueTimer > 0.7f) {
				float d = (1 - ((rescueTimer - 0.7f) / 0.3f));
				position = boat.worldPosition() + glm::vec3(0, 0.2, 0) + ( d * glm::vec3(0, maxHeight - 0.2f, 0) );
				scale = baseScale + (d * 0.5f * baseScale);
			}
			else if (rescueTimer > 0.4f) {
				position = boat.worldPosition() + glm::vec3(0, maxHeight, 0); //stay over the boat
				//make a happy tilt-y animation?
				float d = (1 - ((rescueTimer - 0.4f) / 0.2f));
				float tiltDeg = 20;
				if (d < 0.25f) {
					rotation.z = tiltDeg * (d / 0.25f);
				}
				else if (d < 0.75f) {
					rotation.z = tiltDeg - ( tiltDeg * 2 * ((d - 0.25f) / 0.5f) );
				}
				else {
					rotation.z = -tiltDeg + ( tiltDeg * ((d - 0.5f) / 0.25f) );
				}
			}
			else {
				float d = (rescueTimer / 0.4f);
				float d2 = 1 - d;
				glm::vec3 upPos = boat.worldPosition() + glm::vec3(0, maxHeight, 0);
				glm::vec3 endPos = boat.sailorPickupAnchorPoint();
				position = upPos + ( d2 * (endPos - upPos) );
				scale = d * baseScale * 1.5f; //shrink
				rotation.z = 0;
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
	//return (mode == MODE_DRIFTING) && (glm::distance(boatPosition, position) < 1.0f);
}

bool Sailor::goneOffScreen() {
	return position.z <= -2.3f;
}

void Sailor::rescue() {
	startPosition = glm::vec3(position.x, position.y, position.z);
	rescueTimer = 1.0f;

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
	glUniform1f(uniStorm_body, storminess);

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
	glUniform1f(uniStorm_floatie, storminess);

	//draw
	drawModel(Sailor::model_floatie);

	//

	//bind model --- abstract into function?
	glBindVertexArray(Sailor::model_limb.vao);
	glUseProgram(Sailor::model_limb.shader.program); //only the shader is necessary for uniforms

	//update camera
	glUniformMatrix4fv(Sailor::uniView_limb, 1, GL_FALSE, glm::value_ptr(camera.view));
	glUniformMatrix4fv(Sailor::uniProj_limb, 1, GL_FALSE, glm::value_ptr(camera.proj));

	//special effects
	glUniform1f(uniLightning_limb, lightningTimer);
	glUniform1f(uniStorm_limb, storminess);

	//LEGS
	glUniform3f(uniColor1_limb, 0.2, 0.4, 1);
	//LEFT
	glm::mat4 tr1 = glm::mat4();
	tr1 = glm::translate(tr1, glm::vec3(-1, 0, 0));
	tr1 = transform * tr1;
	glUniformMatrix4fv(Sailor::uniModel_limb, 1, GL_FALSE, glm::value_ptr(tr1));
	drawModel(Sailor::model_limb);
	//RIGHT
	glm::mat4 tr2 = glm::mat4();
	tr2 = glm::translate(tr2, glm::vec3(1, 0, 0));
	tr2 = transform * tr2;
	glUniformMatrix4fv(Sailor::uniModel_limb, 1, GL_FALSE, glm::value_ptr(tr2));
	drawModel(Sailor::model_limb);

	//ARMS
	glUniform3f(uniColor1_limb, this->skintoneChoice.x, this->skintoneChoice.y, this->skintoneChoice.z);
	float waveDeg = 20;
	//LEFT
	glm::mat4 tr3 = glm::mat4();
	tr3 = glm::translate(tr3, glm::vec3(0,1.5,0));
	float deg1 = (90 + waveDeg) + (sin(totalTime * 8) * waveDeg);
	tr3 = glm::rotate(tr3, glm::radians(deg1), glm::vec3(0,0,1));
	tr3 = transform * tr3;
	glUniformMatrix4fv(Sailor::uniModel_limb, 1, GL_FALSE, glm::value_ptr(tr3));
	drawModel(Sailor::model_limb);
	//RIGHT
	glm::mat4 tr4 = glm::mat4();
	tr4 = glm::translate(tr4, glm::vec3(0,1.5,0));
	float deg2 = -(90 + waveDeg) + (-1 * sin(totalTime * 8) * waveDeg);
	tr4 = glm::rotate(tr4, glm::radians(deg2), glm::vec3(0,0,1));
	tr4 = transform * tr4;
	glUniformMatrix4fv(Sailor::uniModel_limb, 1, GL_FALSE, glm::value_ptr(tr4));
	drawModel(Sailor::model_limb);
}