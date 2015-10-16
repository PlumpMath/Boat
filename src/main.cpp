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
#include <time.h>  //chrono vs time?
#include <iostream>
#include <string>
using std::string;
#include <stdlib.h> 

//my stuff
#include "utils.h"
#include "globals.h"
#include "Camera.h"
#include "Boat.h"


/*
TO DO:
Interaction
- waves collide w/ boat
- collision effect
- wave drama controller

Boat
- smoke
- wake?

Lighthouse

Storm
- lightning
- rain
- lightning shader(s)
- wave intensity shader
- storm shader(s)

SFX
- waves
- boat engine
- storm
- etc???

Other
- underwater quad
- jumping fish
*/

/*
What waves could do to you
- carry you backwards
- knock you sideways
- cause you to spin
- cause you to bob
- cause you to keel over
- cause you to get stuck in place
- toss you up in the air

what they will do:
- side hit = knocked sideways, and spin (speed & distance determined by wave size)
- direct hit = knocked upside down for a bit (length determined by wave size, but always remains visible)
*/

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
GLint uniBigWave;

glm::vec3 wavePos = glm::vec3(2,1,-3);
glm::vec3 waveSpeed = glm::vec3(-1,0,1);
bool didWaveHitPlayer = false;
float waveDissapearTimer;
float waveStartHeight;

float totalTime = 0;

Boat boat;


glm::vec3 randomWaveStartingPosition(float waveHeight) {
	float rad = glm::radians( (rand() % 360) * 1.0f );
	std::cout << rad << std::endl;
	glm::vec3 unit = glm::vec3( cos(rad), 0, sin(rad) );
	std::cout << unit.x << " " << unit.z << std::endl;
	glm::vec3 pos = glm::vec3( unit.x * 2, 0, unit.z * 3 );//unit * 3.0f;//5.0f;
	pos += glm::vec3(unit.x * waveHeight * 3, 0, unit.z * waveHeight); //unit * waveHeight;

	return pos;
}

glm::vec3 wavePosToWorldSpace(glm::vec3 pos) {
	return glm::vec3(pos.x * 3, pos.y, pos.z);
}

glm::vec3 boatPosToWorldSpace(Boat b) {
	return glm::vec3(b.position.x * b.scale.x, b.position.y * b.scale.y, b.position.z * b.scale.z);
}

glm::vec3 waveToBoatVec3(glm::vec3 waveStart, Boat b) {
	glm::vec3 boatPos = boatPosToWorldSpace(b);
	boatPos.y = 0;

	glm::vec3 wavePos = wavePosToWorldSpace(waveStart);
	wavePos.y = 0;

	glm::vec3 toBoat = boatPos - wavePos;
	toBoat.x /= 3;
	toBoat = glm::normalize(toBoat);

	return toBoat;
}

int verticesPerSide = 50;
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
			//float x = (r * ((meshSize * 3) / verticesPerSide)) - ((meshSize * 3) / 2.0f);
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
	std::cout << "VAO " << vao << std::endl;

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
	uniBigWave = glGetUniformLocation(shader.program, "bigWavePos");

	glBindVertexArray(0);

	Boat::InitModel();

	//random wave
	wavePos = randomWaveStartingPosition(1);
	waveSpeed = waveToBoatVec3(wavePos, boat);
	wavePos.y = 1;
}

void update(float dt) {

	totalTime += dt;

	/*
	camera.view = glm::rotate(
			camera.view,
			-1.0f * 0.05f * dt * glm::radians(180.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);
	*/
	//check for waves out of bounds
	if (wavePos.x - (wavePos.y * 3) > 2 || 
		wavePos.x + (wavePos.y * 3) < -2 ||
		wavePos.z - (wavePos.y) > 3 || 
		wavePos.z + (wavePos.y) < -3) 
	{
		//new wave!
		wavePos = randomWaveStartingPosition(1);
		waveSpeed = waveToBoatVec3(wavePos, boat);
		wavePos.y = 1;
	}

	if (didWaveHitPlayer) {
		waveDissapearTimer += dt;

		if (waveDissapearTimer > 1) {
			//new wave!
			wavePos = randomWaveStartingPosition(1);
			waveSpeed = waveToBoatVec3(wavePos, boat);
			wavePos.y = 1;
			didWaveHitPlayer = false;
		}
		else {
			wavePos.y = waveStartHeight * (1 - waveDissapearTimer);
		}
	}
	else {
		didWaveHitPlayer = boat.testWaveCollision(wavePos);

		if (didWaveHitPlayer) {
			waveDissapearTimer = 0;
			waveStartHeight = wavePos.y;
		}
	}
	
	boat.update(dt);
	boat.draw();

	//std::cout << wavePos.x << " " << wavePos.z << std::endl;

	glBindVertexArray(vao);
	glUseProgram(shader.program);

	//update camera
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(camera.view));
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(camera.proj));

	//update model transform
	glm::mat4 model;
	model = glm::scale(model, glm::vec3(2.5,1,1));
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	glUniform1f(uniWave, totalTime * 0.2f);

	wavePos += waveSpeed * dt;
	glUniform3fv(uniBigWave, 1, glm::value_ptr(wavePos));
	
	//draw
	glDrawElements(GL_TRIANGLES, (verticesPerSide - 1) * (verticesPerSide - 1) * 2 * 3, GL_UNSIGNED_INT, 0);
}

void on_quit() {
	Boat::DestroyModel();
}

void OnKeyDown(SDL_KeyboardEvent* key) {
	string keyname = SDL_GetKeyName( key->keysym.sym );
	std::cout << "down " << keyname << std::endl;

	boat.onkeydown(keyname);

	if (keyname == "Space") {
		wavePos = randomWaveStartingPosition(1);
		waveSpeed = waveToBoatVec3(wavePos, boat);
		wavePos.y = 1;
	}
}

void OnKeyUp(SDL_KeyboardEvent* key) {
	string keyname = SDL_GetKeyName( key->keysym.sym );
	std::cout << "up " << keyname << std::endl;

	boat.onkeyup(keyname);
}

//main game loop boilerplate
void loop() {
	//start time
	auto then = std::chrono::high_resolution_clock::now();
	//std::cout << "RAND" << rand() << std::endl;

	//event loop
	SDL_Event event;
	bool run = true;
	while (run) {

		//calc time
		auto now = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration_cast<std::chrono::duration<float>>(now - then).count();

		while (SDL_PollEvent(&event)) {
			switch( event.type ){

				case SDL_QUIT:
					run = false;
					break;

				case SDL_KEYDOWN:
					OnKeyDown(&event.key);
					break;

				case SDL_KEYUP:
					OnKeyUp(&event.key);
					break;

				default:
					break;

			}
		}

		// Clear the screen to black
        glClearColor(0.3f, 0.7f, 1.0f, 1.0f);
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
	srand( time(NULL) ); //initialize random numbers

	app = start();
	ready();

	loop();

	on_quit();
	quit(app);

	return 0;
}