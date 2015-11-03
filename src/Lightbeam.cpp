#include "Lightbeam.h"

#include <iostream>

glModelData Lightbeam::model;
GLint Lightbeam::uniProj;
GLint Lightbeam::uniView;
GLint Lightbeam::uniModel;
GLint Lightbeam::uniStorm;
GLint Lightbeam::uniLightning;

void Lightbeam::InitModel() {
	GLfloat vertices[] = {
		//x, y, z,	a
		0, 0.03, 0, 	1,
		0, -0.03, 0, 	1,
		0, 1, -10,		0,
		0, -1, -10, 	0,
	};

	GLuint elements[] = {
		0, 1, 2,
		1, 2, 3,
	};

	Lightbeam::model = buildModel(
		"lightbeam_vert.glsl", "lightbeam_frag.glsl",
		vertices, sizeof(vertices),
		elements, sizeof(elements),
		{
			{"position", 3},
			{"alpha", 1},
		}
	);

	Lightbeam::uniProj = glGetUniformLocation(Lightbeam::model.shader.program, "proj");
	Lightbeam::uniView = glGetUniformLocation(Lightbeam::model.shader.program, "view");
	Lightbeam::uniModel = glGetUniformLocation(Lightbeam::model.shader.program, "model");
	//Lightbeam::uniStorm = glGetUniformLocation(Lightbeam::model.shader.program, "storminess");
	//Lightbeam::uniLightning = glGetUniformLocation(Lightbeam::model.shader.program, "lightningTimer");

}

void Lightbeam::DestroyModel() {
	destroyModel(Lightbeam::model);
}

Lightbeam::Lightbeam() {
	//scale = glm::vec3(1, 0.05, 1);
	rotation.x = 15;
	scale = glm::vec3(1, 1, 1);
	//position = glm::vec3(3.25, 0.65, 5);
	position = glm::vec3(5.6, -0.3, 10);
	//position = glm::vec3(5, 0.3, 12);
}

void Lightbeam::update(float dt) {
	rotation.y += 45 * dt;

	//update transform
	transform = glm::mat4(); //return to identity matrix



	transform = glm::translate(transform, position); //position
	transform = glm::scale( transform, scale); //scale

	//rotation
	transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1,0,0));
	transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0,1,0));
	transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0,0,1));

}

void Lightbeam::draw() {
	glBindVertexArray(Lightbeam::model.vao);
	glUseProgram(Lightbeam::model.shader.program);

	//update camera
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(camera.view));
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(camera.proj));
	
	//update transform
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(transform));

	//special effects
	//glUniform1f(uniStorm, storminess);
	//glUniform1f(uniLightning, lightningTimer);

	drawModel(Lightbeam::model);
}