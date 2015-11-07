#include "Flag.h"

#include <iostream>
#include <algorithm> 

glModelData Flag::model;
GLint Flag::uniProj;
GLint Flag::uniView;
GLint Flag::uniModel;
GLint Flag::uniWave;
GLint Flag::uniStorm;
GLint Flag::uniLightning;

void Flag::InitModel() {
	GLfloat vertices[] = {
		//x, y, z,
		//top = 0 - 10
		0, 4, 0, 
		0, 3.6, -1, 
		0, 3.2, -2, 
		0, 2.8, -3, 
		0, 2.4, -4, 
		0, 2, -5, 
		0, 1.6, -6, 
		0, 1.2, -7, 
		0, 0.8, -8, 
		0, 0.4, -9, 
		0, 0, -10, 
		//bottom = 11 - 20
		0, -4, 0, 
		0, -3.6, -1, 
		0, -3.2, -2, 
		0, -2.8, -3, 
		0, -2.4, -4, 
		0, -2, -5, 
		0, -1.6, -6,
		0, -1.2, -7, 
		0, -0.8, -8, 
		0, -0.4, -9, 
	};

	GLuint elements[] = {
		
		0, 1, 11,

		1, 11, 12,
		1, 2, 12,

		2, 12, 13,
		2, 3, 13,

		3, 13, 14,
		3, 4, 14,

		4, 14, 15,
		4, 5, 15,

		5, 15, 16,
		5, 6, 16,

		6, 16, 17,
		6, 7, 17,
	
		7, 17, 18,
		7, 8, 18,
		
		8, 18, 19,
		8, 9, 19,
		
		9, 19, 20,
		9, 10, 20,
	};

	Flag::model = buildModel(
		"flag_vert.glsl", "flag_frag.glsl",
		vertices, sizeof(vertices),
		elements, sizeof(elements),
		{
			{"position", 3}
		}
	);

	Flag::uniProj = glGetUniformLocation(Flag::model.shader.program, "proj");
	Flag::uniView = glGetUniformLocation(Flag::model.shader.program, "view");
	Flag::uniModel = glGetUniformLocation(Flag::model.shader.program, "model");
	Flag::uniWave = glGetUniformLocation(Flag::model.shader.program, "waveOffset");
	Flag::uniStorm = glGetUniformLocation(Flag::model.shader.program, "storminess");
	Flag::uniLightning = glGetUniformLocation(Flag::model.shader.program, "lightningTimer");

	std::cout << "build flag!!!" << std::endl;
	std::cout << "uni proj " << Flag::uniProj << std::endl;
}

void Flag::DestroyModel() {
	destroyModel(Flag::model);	
	std::cout << "destroy flag!!!" << std::endl;
}

Flag::Flag() {
	std::cout << "init flag!!!" << std::endl;

	scale = glm::vec3(0.02 * 1.5, 0.01 * 1.5, 0.01 * 1.5);
}

void Flag::update(float dt, glm::vec3 anchorPos, glm::vec3 anchorRot) {
	totalTime += dt;

	if (rotTimer > 0) {
		rotTimer -= dt;

		stiffnessTimer += dt * 2; //extend the flag
		if (stiffnessTimer > 2) stiffnessTimer = 2; //keep things reasonable
	
		curRotY = prevRotY + ( (nextRotY - prevRotY) * (1 - rotTimer) );
	}
	else {
		curRotY = nextRotY;
		prevRotY = nextRotY;
	}

	if (isAttachedToBoat) {

		if (stiffnessTimer > 0 && rotTimer <= 0) {
			stiffnessTimer -= dt;
			if (stiffnessTimer <= 0) {
				stiffnessTimer = 0;
			}
		}

		float bobWithBoat = (0.06 * (1 + storminess) * sin(totalTime * 0.5 * 8) / 2); //lotsa magic numbers
		
		position = anchorPos;
		position.y += bobWithBoat;
		position.y += 0.05;

		rotation = anchorRot; //currently doesn't work for stun spins :(
		rotation.y += curRotY;
	}
	else {
		position += glm::vec3(2,0.5,-0.5) * dt;

		rotation = glm::vec3(0,0,0);
		rotation.y += curRotY;
	}

	//update transform
	transform = glm::mat4(); //return to identity matrix
	
	transform = glm::translate(transform, position); //position

	//rotation
	transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1,0,0));
	transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0,1,0));
	transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0,0,1));

	transform = glm::scale( transform, glm::vec3( scale.x, scale.y, scale.z * ( 1 + ((stiffnessTimer / 2)*2) ) )); //scale

	

}


void Flag::setRotYGoal(float rotY) {
	rotTimer = 1.0;
	nextRotY = rotY;
}

void Flag::draw() {
	//std::cout << "draw flag!!!" << std::endl;

	glBindVertexArray(Flag::model.vao);
	glUseProgram(Flag::model.shader.program);

	//update camera
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(camera.view));
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(camera.proj));
	
	//update transform
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(transform));

	//special effects
	glUniform1f(uniWave, totalTime);
	glUniform1f(uniStorm, storminess);
	glUniform1f(uniLightning, lightningTimer);

	drawModel(Flag::model);
}