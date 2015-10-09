//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

//GLM
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//standard libs
#include <chrono>
#include <iostream>

//my stuff
#include "utils.h"
#include "Camera.h"

glApp app;

Camera camera;

GLuint vao;
GLuint vbo;
GLuint ebo;
glShaderData shader;

GLint uniProj;
GLint uniView;
GLint uniModel;
GLint uniWave;

int verticesPerSide = 30;

float totalTime = 0;

void generateOceanMesh() {
	
	float meshSize = 5.0f;

	int verticesTotal = verticesPerSide * verticesPerSide;
	int trianglesTotal = (verticesPerSide - 1) * (verticesPerSide - 1) * 2;

	GLfloat vertices[verticesTotal * 3];
	GLuint elements[trianglesTotal * 3];

	//create vertices
	for (int r = 0; r < verticesPerSide; r++) {
		for (int c = 0; c < verticesPerSide; c++) {

			//calculate start index for vertex
			int i = (r * verticesPerSide * 3) + (c * 3); 

			//calc position
			float x = (r * (meshSize / verticesPerSide)) - (meshSize / 2.0f);
			float y = 0.0f;
			float z = (c * (meshSize / verticesPerSide)) - (meshSize / 2.0f);
			//set position
			vertices[i + 0] = x;
			vertices[i + 1] = y;
			vertices[i + 2] = z;

			//std::cout << i << std::endl;
			//std::cout << x << " " << y << " " << z << std::endl;
		}
	}

	//create triangles
	for (int r = 0; r < (verticesPerSide - 1); r++) {
		for (int c = 0; c < (verticesPerSide - 1); c++) {

			int t1 = (r * (verticesPerSide - 1) * 3 * 2) + (c * 3 * 2); //two triangles per vertex
			int t2 = t1 + 3;

			//triangle 1
			elements[t1 + 0] = (r * verticesPerSide) + c;
			elements[t1 + 1] = (r * verticesPerSide) + (c + 1);
			elements[t1 + 2] = ((r + 1) * verticesPerSide) + (c + 1);

			//std::cout << t1 << std::endl;
			//std::cout << elements[t1 + 0] << " " << elements[t1 + 1] << " " << elements[t1 + 2] << std::endl;

			//triangle 2
			elements[t2 + 1] = (r * verticesPerSide) + c;
			elements[t2 + 2] = ((r + 1) * verticesPerSide) + (c + 1);;
			elements[t2 + 0] = ((r + 1) * verticesPerSide) + c;

			//std::cout << t2 << std::endl;
			//std::cout << elements[t2 + 0] << " " << elements[t2 + 1] << " " << elements[t2 + 2] << std::endl;
		}
	}

	vbo = setVertexBuffer(vertices, sizeof(vertices));
	ebo = setElementBuffer(elements, sizeof(elements));
}

void ready() {

	camera = Camera();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	shader = readAndConstructShaderProgram("ocean_vert.glsl", "ocean_frag.glsl");
	glUseProgram(shader.program);

	generateOceanMesh();

	//glAttribF a = { "position", 3 };
	setAttrib({ "position", 3 }, 3, 0, shader.program);

	//get uniforms for the various transformation matrices
	uniProj = glGetUniformLocation(shader.program, "proj");
	uniView = glGetUniformLocation(shader.program, "view");
	uniModel = glGetUniformLocation(shader.program, "model");
	uniWave = glGetUniformLocation(shader.program, "waveOffset");
}

void update(float dt) {

	totalTime += dt;

	
	camera.view = glm::rotate(
			camera.view,
			-1.0f * 0.05f * dt * glm::radians(180.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

	glBindVertexArray(vao);

	//update camera
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(camera.view));
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(camera.proj));

	//update model transform
	glm::mat4 model;
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	glUniform1f(uniWave, totalTime * 0.2f);
	
	//draw
	glDrawElements(GL_TRIANGLES, (verticesPerSide - 1) * (verticesPerSide - 1) * 2 * 3, GL_UNSIGNED_INT, 0);
}

void on_quit() {

}

//main game loop boilerplate
void loop() {
	//start time
	auto then = std::chrono::high_resolution_clock::now();

	//event loop
	SDL_Event windowEvent;
	while (true) {

		//calc time
		auto now = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration_cast<std::chrono::duration<float>>(now - then).count();

		if (SDL_PollEvent(&windowEvent)) {
			if (windowEvent.type == SDL_QUIT) break;
		}

		// Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //depth buffer needs to be cleared for depth testing

		//update & draw everything
		update(dt);

		//update the window
		SDL_GL_SwapWindow(app.window);

		then = now;
	}
}

int main(int argc, char *argv[])
{
	app = start();
	ready();

	loop();

	on_quit();
	quit(app);

	return 0;
}