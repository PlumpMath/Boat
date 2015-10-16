#include "Boat.h"

#include <iostream>
#include <algorithm> 

//init static variables (for reasons I still don't fully understand)
glModelData Boat::model;
GLint Boat::uniProj;
GLint Boat::uniView;
GLint Boat::uniModel;
GLint Boat::uniWave;

//static functions
void Boat::InitModel() {
	GLfloat vertices[] = {
		//x, y, z,	r, g, b,	nx, ny, nz
		//all vertices start at the front and go clockwise around

		//HULL
		//layer 1 = 0 - 6
		0, 0, 8,		0.2, 0.2, 0.4,	0, 0, 1,
		2.5, 0, 7,		0.2, 0.2, 0.4, 	0, 0, 1,
		2, 0, -6,		0.2, 0.2, 0.4,	1, 0, 0,
		1.5, 0, -8,		0.2, 0.2, 0.4,	0, 0, -1,
		-1.5, 0, -8,	0.2, 0.2, 0.4,	0, 0, -1,
		-2, 0, -6,		0.2, 0.2, 0.4,	-1, 0, 0,
		-2.5, 0, 7,		0.2, 0.2, 0.4,	0, 0, 1, 
		//layer 2 = 7 - 13
		0, 3, 9,		0.2, 0.2, 0.4,	0, 1, 0,
		3, 2.5, 7,		0.2, 0.2, 0.4, 	0, 1, 0,
		2.5, 2, -6,		0.2, 0.2, 0.4,	0, 1, 0,
		2, 2.5, -9,		0.2, 0.2, 0.4,	0, 1, 0,
		-2, 2.5, -9,	0.2, 0.2, 0.4,	0, 1, 0,
		-2.5, 2, -6,	0.2, 0.2, 0.4,	0, 1, 0,
		-3, 2.5, 7,		0.2, 0.2, 0.4,	0, 1, 0, 

		//LOWER DECK
		//layer 1 = 14 - 18
		0, 2.5, 6,		0.9, 0.9, 0.4, 	0, 0, 1,
		2, 2.5, 5,		0.9, 0.9, 0.4, 	1, 0, 0,
		1.5, 2.5, -5,	0.9, 0.9, 0.4, 	0, 0, -1,
		-1.5, 2.5, -5,	0.9, 0.9, 0.4, 	0, 0, -1,
		-2, 2.5, 5,		0.9, 0.9, 0.4, 	-1, 0, 0,
		//layer 2 = 19 - 23
		0, 5, 6,		0.9, 0.9, 0.4, 	0, 1, 0,
		2, 5, 5,		0.9, 0.9, 0.4, 	0, 1, 0,
		2, 5, -5,		0.9, 0.9, 0.4, 	0, 1, 0,
		-2, 5, -5,		0.9, 0.9, 0.4, 	0, 1, 0,
		-2, 5, 5,		0.9, 0.9, 0.4, 	0, 1, 0,

		//TOP DECK
		//layer 1 = 24 - 28
		0, 5, 4.5,		0.9, 0.9, 0.4, 	0, 0, 1,
		1, 5, 4,		0.9, 0.9, 0.4, 	1, 0, 0,
		1, 5, 0,		0.9, 0.9, 0.4, 	0, 0, -1,
		-1, 5, 0,		0.9, 0.9, 0.4, 	0, 0, -1,
		-1, 5, 4,		0.9, 0.9, 0.4, 	-1, 0, 0,
		//layer 2 = 29 - 33
		0, 8.5, 5,		0.9, 0.9, 0.4, 	0, 1, 0,
		1, 8.25, 4,		0.9, 0.9, 0.4, 	0, 1, 0,
		1, 8.25, 0,		0.9, 0.9, 0.4, 	0, 1, 0,
		-1, 8, 0,		0.9, 0.9, 0.4, 	0, 1, 0,
		-1, 8, 4,		0.9, 0.9, 0.4, 	0, 1, 0,

		//SMOKESTACK
		//layer 1 = 34 - 37
		0, 5, -2.5,		0.1, 0.1, 0.2, 	0, 0, 1,
		0.5, 5, -3,		0.1, 0.1, 0.2, 	1, 0, 0,
		0, 5, -3.5,		0.1, 0.1, 0.2, 	0, 0, -1,
		-0.5, 5, -3,	0.1, 0.1, 0.2, 	1, 0, 0,
		//layer 2 = 38 - 41
		0, 10, -2.2,	0.1, 0.1, 0.2, 	0, 1, 0,
		0.8, 10, -3,	0.1, 0.1, 0.2, 	0, 1, 0,
		0, 10, -3.8,	0.1, 0.1, 0.2, 	0, 1, 0,
		-0.8, 10, -3,	0.1, 0.1, 0.2, 	0, 1, 0,

		//extras (for shading on top deck) = 42, 43
		1, 5, 0,		0.9, 0.9, 0.4,	1, 0, 0, //replaces 26
		-1, 5, 0,		0.9, 0.9, 0.4,	-1, 0, 0, //replaces 27
	};

	GLuint elements[] = {
		//HULL
		//sides
		0, 7, 1,
		8, 7, 1,
		1, 8, 2,
		9, 8, 2,
		2, 9, 3,
		10, 9, 3,
		3, 10, 4,
		11, 10, 4,
		4, 11, 5,
		12, 11, 5,
		5, 12, 6,
		13, 12, 6,
		6, 13, 0,
		7, 13, 0,
		//top
		13, 7, 8,
		13, 12, 8,
		12, 9, 8,
		12, 11, 9,
		10, 11, 9,

		//LOWER DECK
		//sides
		15, 19, 14,
		20, 15, 19,
		20, 15, 16,
		21, 20, 16,
		21, 16, 17,
		22, 21, 17,
		22, 17, 18,
		23, 22, 18,
		23, 19, 18,
		14, 18, 19,
		//top
		19, 20, 23,
		20, 21, 22,
		20, 22, 23,

		//TOP DECK
		//sides
		25, 29, 24,
		30, 29, 25,
		30, 26, 25,
		31, 30, 42, //42 (replacement for 26)
		31, 27, 26,
		32, 31, 27,
		32, 28, 43, //43 (replacement for 27)
		33, 32, 28,
		33, 29, 28,
		29, 28, 24,
		//top
		29, 30, 33,
		30, 31, 32,
		30, 32, 33,

		//SMOKESTACK
		//sides
		35, 38, 34,
		39, 38, 35,
		35, 39, 36,
		40, 39, 36,
		36, 40, 37,
		41, 40, 37,
		38, 41, 37,
		37, 38, 34,
		//top
		38, 39, 40,
		38, 41, 40,
	};

	Boat::model = buildModel(
						"boat_vert.glsl", "boat_frag.glsl", 
						vertices, sizeof(vertices),
						elements, sizeof(elements),
						{
							{"position", 3}, 
							{"color", 3}, 
							{"normal", 3}
						}
					);

	//get uniforms for the various transformation matrices
	Boat::uniProj = glGetUniformLocation(Boat::model.shader.program, "proj");
	Boat::uniView = glGetUniformLocation(Boat::model.shader.program, "view");
	Boat::uniModel = glGetUniformLocation(Boat::model.shader.program, "model");
	Boat::uniWave = glGetUniformLocation(Boat::model.shader.program, "waveOffset");
}

void Boat::DestroyModel() {
	destroyModel(Boat::model);
}

//methods
Boat::Boat() {
	//transform = glm::scale(transform, glm::vec3(0.08, 0.08, 0.08));
	scale = glm::vec3(0.08, 0.08, 0.08);
	speed = 10;

	position.y = 1;
	position.x = 10;
	//transform = glm::translate(transform, glm::vec3(0, 1, 0));
}

void Boat::update(float dt) {
	totalTime += dt;

	//rotation
	goalRotTimer += dt;
	float timerLength = 1;
	if (goalRotTimer > timerLength) {
		rotation = goalRot;
	}
	else {
		rotation = startRot + ((goalRot - startRot) * (goalRotTimer / timerLength));
	}

	//move
	float accelFactor = 0.6;
	float frictFactor = 0.4;

	if (movingLeft) {
		velocity.x += speed * accelFactor * dt;
		velocity.x = std::min(velocity.x, speed);
	}
	else if (movingRight) {
		velocity.x -= speed * accelFactor * dt;
		velocity.x = std::max(velocity.x, -speed);
	}
	else { //friction
		if (velocity.x != 0) {
			float sign = (velocity.x / std::abs(velocity.x));
			float newX = velocity.x - (speed * frictFactor * sign * dt);
			if ( (newX < 0) != (velocity.x < 0) ) newX = 0;
			velocity.x = newX;
		}
	}

	if (movingForward) {
		velocity.z += speed * accelFactor * dt;
		velocity.z = std::min(velocity.z, speed);
	}
	else if (movingBack) {
		velocity.z -= speed * accelFactor * dt;
		velocity.z = std::max(velocity.z, -speed * 0.5f);
	}
	else { //friction
		if (velocity.z != 0) {
			float sign = (velocity.z / std::abs(velocity.z));
			float newZ = velocity.z - (speed * frictFactor * sign * dt);
			if ( (newZ < 0) != (velocity.z < 0) ) newZ = 0;
			velocity.z = newZ;
		}
	}

	//velocity
	position += (velocity * dt);

	//the current
	position.z += -2 * dt;

	//bounds
	//std::cout << position.x << " " << position.z << std::endl;
	if (position.x > 72) position.x = 72;
	if (position.x < -72) position.x = -72;
	if (position.z > 16) position.z = 16;
	if (position.z < -12) position.z = -12;

	//update transform
	transform = glm::mat4(); //return to identity matrix

	transform = glm::scale(transform, scale); //scale
	transform = glm::translate(transform, position); //position
	//rotation
	transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1,0,0));
	transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0,1,0));
	transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0,0,1));

	if (isStunned) {
		stunSpin(dt);
	}
}

void Boat::stunSpin(float dt) {
	float maxStunTime = 2.0f;

	stunTimer += dt;
	transform = glm::rotate(transform, glm::radians(720 * (stunTimer / maxStunTime)), glm::vec3(0,1,0));

	if (stunTimer > maxStunTime) {
		isStunned = false;
	}
}

void Boat::onkeydown(string keyname) {
	if (!isStunned) {
		if (keyname == "Left" && !movingLeft) {
			movingLeft = true;
			movingRight = false;
			tiltLeft();
		}
		else if (keyname == "Right" && !movingRight) {
			movingRight = true;
			movingLeft = false;
			tiltRight();
		}

		if (keyname == "Up" && !movingForward) {
			movingForward = true;
			movingBack = false;
		}
		else if (keyname == "Down" && !movingBack) {
			movingBack = true;
			movingForward = false;
		}
	}
}

void Boat::onkeyup(string keyname) {
	if (!isStunned) {
		if (keyname == "Left" && movingLeft) {
			movingLeft = false;
			untilt();
		}
		else if (keyname == "Right" && movingRight) {
			movingRight = false;
			untilt();
		}

		if (keyname == "Up" && movingForward) {
			movingForward = false;
		}
		else if (keyname == "Down" && movingBack) {
			movingBack = false;
		}
	}
}

void Boat::untilt() {
	//start recentering rotation
	startRot = rotation;
	goalRot.y = 0;
	goalRot.z = 0;
	goalRotTimer = 0;
}

void Boat::tiltLeft() {
	//start leftward rotation
	startRot = rotation;
	goalRot.y = 15;
	goalRot.z = -10;
	goalRotTimer = 0;
}

void Boat::tiltRight() {
	//start rightward rotation
	startRot = rotation;
	goalRot.y = -15;
	goalRot.z = 10;
	goalRotTimer = 0;
}

void Boat::testWaveCollision(glm::vec3 wavePos) {
	glm::vec2 boatCollisionPos = glm::vec2(position.x, position.z);
	glm::vec2 waveCollisionPos = glm::vec2(wavePos.x * 4 * 8, wavePos.z * 8); //move wave into boat space
	//float dist = glm::distance(boatCollisionPos, waveCollisionPos);
	float distX = std::abs(waveCollisionPos.x - boatCollisionPos.x);
	float distZ = std::abs(waveCollisionPos.y - boatCollisionPos.y);
	
	if (distX < wavePos.y * 10 * 3 && distZ < wavePos.y * 10) { //stupid magic numbers because of all the distortions I caused
		
		if (!isStunned) {

			isStunned = true;
			stunTimer = 0;

			movingLeft = false;
			movingRight = false;
			movingForward = false;
			movingBack = false;

			untilt();

			//apply a force
			glm::vec2 force = glm::normalize(boatCollisionPos - waveCollisionPos) * 10.0f * wavePos.y;
			velocity = glm::vec3(force.x, 0, force.y);
		}
	}
}

void Boat::draw() {
	
	//bind model --- abstract into function?
	glBindVertexArray(Boat::model.vao);
	glUseProgram(Boat::model.shader.program); //only the shader is necessary for uniforms

	//wave effect
	glUniform1f(uniWave, totalTime * 0.5f);

	//update camera
	glUniformMatrix4fv(Boat::uniView, 1, GL_FALSE, glm::value_ptr(camera.view));
	glUniformMatrix4fv(Boat::uniProj, 1, GL_FALSE, glm::value_ptr(camera.proj));

	//update transform
	glUniformMatrix4fv(Boat::uniModel, 1, GL_FALSE, glm::value_ptr(transform));

	//draw
	drawModel(Boat::model);
}