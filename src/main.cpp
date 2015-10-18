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
- make 4 difficulty levels
- tweak boat control feel
- tweak boat knockback feel
- drama controller
	- test timing
- storm effects
	- lightning
	- rain
	- lightning shaders
	- wave intensity
	- storm color shaders
- boat polish
	- smoke
	- wake
- lighthouse model
- SFX
- underwater quad
- jumping fish?
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

glm::vec3 wavePos = glm::vec3(0,1,3);
glm::vec3 waveSpeed = glm::vec3(0,0,-1);
bool didWaveHitPlayer = false;
float waveDissapearTimer;
float waveStartHeight;

struct DifficultyLevel {
	float minSize;
	float maxSize;
	float minSpeed;
	float maxSpeed;
	float timeBetweenWaves;
};

DifficultyLevel easy = {
	0.7, 1,
	0.3, 1,
	3
};

DifficultyLevel hard = {
	1, 1.5,
	1, 1.5,
	3
};

DifficultyLevel impossible = {
	1, 1.5,
	1, 1.5,
	3
};

DifficultyLevel curDifficulty = easy;

bool betweenWaves = false;
float betweenWavesTimer = 0;
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

void startNewWave(DifficultyLevel difficulty) {
	std::cout << "start new wave" << std::endl;

	float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	float height = difficulty.minSize + (r1 * (difficulty.maxSize - difficulty.minSize));
	float speed = difficulty.minSpeed + (r2 * (difficulty.maxSpeed - difficulty.minSpeed));

	wavePos = randomWaveStartingPosition(height);
	waveSpeed = waveToBoatVec3(wavePos, boat) * speed;
	wavePos.y = height;

	didWaveHitPlayer = false;
}

void waitForNextWave() {
	wavePos = glm::vec3(0,0,100); //hide the dumb wave
	waveSpeed = glm::vec3(0,0,0);

	betweenWavesTimer = 0;
	betweenWaves = true;
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
	startNewWave(curDifficulty);
}

void waveUpdate(float dt) {


	if (betweenWaves) {

		betweenWavesTimer += dt;
		//std::cout <<  betweenWavesTimer << std::endl;

		if (betweenWavesTimer > curDifficulty.timeBetweenWaves) {
			//std::cout << "wave timer is done!" << std::endl;
			startNewWave(curDifficulty);
			betweenWaves = false;
		}
	}
	else {

		//move the wave
		wavePos += waveSpeed * dt;	

		//check for waves out of bounds
		if (wavePos.x - (wavePos.y * 3) > 2 || 
			wavePos.x + (wavePos.y * 3) < -2 ||
			wavePos.z - (wavePos.y) > 3 || 
			wavePos.z + (wavePos.y) < -3) 
		{
			waitForNextWave();
		}

		if (didWaveHitPlayer) { //waves that have hit the player dissapear
			float dissapearTimeEnd = 1.5f;
			waveDissapearTimer += dt;

			if (waveDissapearTimer > dissapearTimeEnd) {
				waitForNextWave();
			}
			else {
				wavePos.y = waveStartHeight * ((dissapearTimeEnd - waveDissapearTimer) / dissapearTimeEnd);
			}
		}
		else { //check for wave collisions
			didWaveHitPlayer = boat.testWaveCollision(wavePos);

			if (didWaveHitPlayer) {
				waveDissapearTimer = 0;
				waveStartHeight = wavePos.y;
			}
		}
	}
	
}

void drawOcean() {
	glBindVertexArray(vao);
	glUseProgram(shader.program);

	//update camera
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(camera.view));
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(camera.proj));

	//update model transform
	glm::mat4 model;
	model = glm::scale(model, glm::vec3(2.52,1,1));
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	glUniform1f(uniWave, totalTime * 0.2f);

	glUniform3fv(uniBigWave, 1, glm::value_ptr(wavePos));
	
	//draw
	glDrawElements(GL_TRIANGLES, (verticesPerSide - 1) * (verticesPerSide - 1) * 2 * 3, GL_UNSIGNED_INT, 0);	
}

void rotateCameraForTesting(float dt) {
	camera.view = glm::rotate(
			camera.view,
			-1.0f * 0.05f * dt * glm::radians(180.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);
}

void update(float dt) {

	totalTime += dt;

	waveUpdate(dt);
	boat.update(dt);
	
	boat.draw();
	drawOcean();
}

void on_quit() {
	Boat::DestroyModel();
}

void OnKeyDown(SDL_KeyboardEvent* key) {
	string keyname = SDL_GetKeyName( key->keysym.sym );
	//std::cout << "down " << keyname << std::endl;

	boat.onkeydown(keyname);

	/*
	if (keyname == "Space") {
		wavePos = randomWaveStartingPosition(1);
		waveSpeed = waveToBoatVec3(wavePos, boat);
		wavePos.y = 1;
	}
	*/
}

void OnKeyUp(SDL_KeyboardEvent* key) {
	string keyname = SDL_GetKeyName( key->keysym.sym );
	//std::cout << "up " << keyname << std::endl;

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
        glClearColor(0.3f, 0.7f, 1.0f, 1.0f); //light blue

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