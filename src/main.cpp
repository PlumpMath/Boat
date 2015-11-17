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
#include <math.h> 
#define PI 3.14159265
#include <vector>

//my stuff
#include "utils.h"
#include "globals.h"

#include "Camera.h"
#include "Boat.h"
#include "Flag.h"
#include "lighthouse.h"
#include "Lightbeam.h"
#include "ParticleSystem.h"
#include "RainParticles.h"
#include "SmokeParticles.h"
#include "Sailor.h"


/*
TODO:
- sailors
- package shaders with .app
- sound
- cross platform distribution
- credits & title
- make flag hit into camera on its way out
- jumping fish

new ideas:
- narration
- rocks?
- others?

for next gl project:
- make uniforms more easy / automatic
- build a "game object base clas"
- build a "3D model base class"
- expand on particle system class
- - better control of particle draw order (alpha particle problem)
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
GLint uniStorminess;
GLint uniLightning;

glm::vec3 wavePos = glm::vec3(0,1,3);
glm::vec3 waveSpeed = glm::vec3(0,0,-1);
bool didWaveHitPlayer = false;
float waveDissapearTimer;
float waveStartHeight;
bool isTheBigOne = false;

bool hasGameStarted = false;

struct DifficultyLevel {
	float minSize; float maxSize;
	float minSpeed; float maxSpeed;
	float timeBetweenWaves;
};

DifficultyLevel calm = {
	0, 0,
	0, 0,
	0
};

DifficultyLevel easy = {
	0.7, 1,
	0.3, 1,
	3
};

DifficultyLevel medium = {
	1, 1.3,
	1, 1.5,
	2.5
};

DifficultyLevel hard = {
	1.2, 1.4,
	1.5, 2.5,
	2
};

DifficultyLevel impossible = {
	1.3, 1.6,
	3, 4,
	0
};

struct DramaUnit {
	DifficultyLevel difficulty;
	float time;
};

DramaUnit dramaticArc[] = 
{
	{calm, 10},
	{easy, 40},
	{calm, 15},
	{medium, 40}, //too hard sometimes? 
	{hard, 30},
	{impossible, 20},
	{calm, 20}
};

int dramaIndex = 0;
float dramaTimer = 0;
 
DifficultyLevel curDifficulty = dramaticArc[dramaIndex].difficulty;

bool betweenWaves = false;
float betweenWavesTimer = 0;
float totalTime = 0;
float totalPlayTime = 0;

float storminess = 0;
float lightningTimer = 8; //time since last lightning strike
int lightningStrikeCounter = 0;
float timeBetweenLightning;
bool doesLightningStrikeOnWaveCollision = false;
float waveTimer = 0;

int lighthouseMoveCounter = 0;

Boat boat;
Flag flag;
Lighthouse lighthouse;

RainParticles rain;
SmokeParticles smoke;

//const int testSailorNum = 50;
//Sailor testSailors[testSailorNum];
vector<Sailor> sailors;

glm::vec3 randomWaveStartingPosition(float waveHeight) {
	float rad = glm::radians( (rand() % 360) * 1.0f );
	//std::cout << rad << std::endl;
	glm::vec3 unit = glm::vec3( cos(rad), 0, sin(rad) );
	//std::cout << unit.x << " " << unit.z << std::endl;
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

void startBigWave() {
	wavePos = glm::vec3(0, 2.5, 5.5);
	waveSpeed = glm::vec3(0, 0, -1);
	didWaveHitPlayer = false;
	isTheBigOne = true;
}

void startNewWave(DifficultyLevel difficulty) {
	//std::cout << "start new wave" << std::endl;

	float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	float height = difficulty.minSize + (r1 * (difficulty.maxSize - difficulty.minSize));
	float speed = difficulty.minSpeed + (r2 * (difficulty.maxSpeed - difficulty.minSpeed));

	wavePos = randomWaveStartingPosition(height);
	glm::vec3 toBoat = waveToBoatVec3(wavePos, boat);
	waveSpeed = toBoat * speed;
	wavePos.y = height;

	flag.setRotYGoal( (atan2(toBoat.x, toBoat.z) * (180 / PI)) + 180 );
	//rain.gust(toBoat);

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
	uniStorminess = glGetUniformLocation(shader.program, "storminess");
	uniLightning = glGetUniformLocation(shader.program, "lightningTimer");

	glBindVertexArray(0);

	Boat::InitModel();
	Flag::InitModel();
	Lighthouse::InitModel();
	Lightbeam::InitModel();
	Sailor::InitGeometry();

	rain.init();
	smoke.init();

	//random wave
	startNewWave(curDifficulty);
	flag.setRotYGoal(0);

	//debug
	/*
	for (int i = 0; i < 10; i++) {
		Sailor s;
		sailors.push_back(s);
	}
	*/
}

void dramaUpdate(float dt) {
	dramaTimer += dt;
	totalPlayTime += dt;
	//std::cout << dramaTimer << std::endl;

	float startOfStormTime = (dramaticArc[0].time + dramaticArc[1].time + dramaticArc[2].time);
	float hardModeTime = (dramaticArc[0].time + dramaticArc[1].time + dramaticArc[2].time + 
							dramaticArc[3].time);
	float impossibleModeTime = (dramaticArc[0].time + dramaticArc[1].time + dramaticArc[2].time + 
							dramaticArc[3].time + dramaticArc[4].time);
	float fakeCalmTime = (dramaticArc[0].time + dramaticArc[1].time + dramaticArc[2].time + 
							dramaticArc[3].time + dramaticArc[4].time + dramaticArc[5].time);
	float endOfStormTime = (dramaticArc[0].time + dramaticArc[1].time + dramaticArc[2].time + 
							dramaticArc[3].time + dramaticArc[4].time + dramaticArc[5].time + 
							dramaticArc[6].time);

	//std::cout << dramaTimer << std::endl;

	//if (dramaTimer > dramaticArc[dramaIndex].time && dramaIndex < 5) {
	if (dramaTimer > dramaticArc[dramaIndex].time && dramaIndex < 6) {
		//std::cout << "next difficulty!!!" << std::endl;
		dramaIndex++;
		curDifficulty = dramaticArc[dramaIndex].difficulty;
		dramaTimer = 0;

		//make the current wave dissapear
		didWaveHitPlayer = true;
		waveDissapearTimer = 0;
		waveStartHeight = wavePos.y;
	}

	//send the flag flying away
	if (totalPlayTime > (endOfStormTime - 10) && flag.isAttachedToBoat) {
		flag.setRotYGoal( 270 );
		flag.isAttachedToBoat = false;
	}

	if (!isTheBigOne && totalPlayTime > (endOfStormTime - 5)) {
		//std::cout << "HERE IT COMES " << std::endl;
		startBigWave();
	}
	
	//lighthouse events
	if (totalPlayTime > 2 && lighthouseMoveCounter < 1) {
		lighthouse.moveTo(glm::vec3(6, -3.7, 10), startOfStormTime - 2);
		lighthouseMoveCounter++;
	}
	else if (totalPlayTime > (endOfStormTime + 30 + 3) && lighthouseMoveCounter < 2) {
		//lighthouse.moveTo(glm::vec3(1.8, -0.4, 2.7), 15);
		lighthouse.moveTo(glm::vec3(2, -0.4, 2), 30);
		lighthouseMoveCounter++;
	}

	//storminess
	if (totalPlayTime < startOfStormTime - 5) {
		storminess = 0;
	}
	else if (totalPlayTime < startOfStormTime) {
		storminess = (totalPlayTime - (startOfStormTime - 5)) / 5;
	}
	else if (totalPlayTime < (endOfStormTime + 15)) {
		storminess = 1;
	}
	else if (totalPlayTime < endOfStormTime + 20) {
		storminess = (5 - (totalPlayTime - (endOfStormTime + 15))) / 5;
	}
	else {
		storminess = 0;
	}


	//lightning
	lightningTimer += dt;
	//if (totalPlayTime > (startOfStormTime - 2.51) && totalPlayTime < startOfStormTime) { //start of storm
	if (totalPlayTime > startOfStormTime && totalPlayTime < (startOfStormTime + 1)) {
		if (lightningStrikeCounter < 1) {
			lightningTimer = 0;
			lightningStrikeCounter++;
		}
	}
	else if (totalPlayTime > hardModeTime && totalPlayTime < (hardModeTime + 5)) { //start of second wave
		if (lightningStrikeCounter < 2) {
			lightningTimer = 0;
			lightningStrikeCounter++;
		}
	}
	//else if (totalPlayTime > impossibleModeTime && totalPlayTime < (impossibleModeTime + 5)) { //start of third wave
	else if (totalPlayTime > (impossibleModeTime - 1) && totalPlayTime < impossibleModeTime) { //start of third wave
		if (lightningStrikeCounter < 3) {
			lightningTimer = 0;
			lightningStrikeCounter++;
			//timeBetweenLightning = (2 + (rand() % 5)) * 1.0f;
		}
	}
	else if (totalPlayTime > impossibleModeTime && totalPlayTime < (fakeCalmTime - 5)) { //third wave
		doesLightningStrikeOnWaveCollision = true;
		lightningTimer += dt; //double the speed of lightning in this period
	}
	else if (totalPlayTime > fakeCalmTime && totalPlayTime < (fakeCalmTime + 5)) { //end of third wave
		doesLightningStrikeOnWaveCollision = false;
		if (lightningStrikeCounter < 4) {
			lightningTimer = 0;
			lightningStrikeCounter++;
		}
	}
	//else if (totalPlayTime > (endOfStormTime + 20 - 2.51) && totalPlayTime < (endOfStormTime + 25)) { //end of storm
	else if (totalPlayTime > (endOfStormTime + 20 - 5) && totalPlayTime < (endOfStormTime + 25)) { //end of storm
		if (lightningStrikeCounter < 5) {
			lightningTimer = 0;
			lightningStrikeCounter++;
		}
	}	
}

void waveUpdate(float dt) {
	//waveTimer = (waveTimer + dt);
	//if (waveTimer > 5) waveTimer -= 5;

	if (betweenWaves || wavePos.y == 0) { //second statement is a hack to move past calm periods

		betweenWavesTimer += dt;
		//std::cout <<  betweenWavesTimer << std::endl;

		if (betweenWavesTimer > curDifficulty.timeBetweenWaves) {
			//std::cout << "wave timer is done!" << std::endl;
			if (curDifficulty.minSize > 0 ) startNewWave(curDifficulty); //more hacks for calm period
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
			if (isTheBigOne) {
				boat.testBigWaveCollision(wavePos);
			}
			else {
				didWaveHitPlayer = boat.testWaveCollision(wavePos);

				if (didWaveHitPlayer) {
					waveDissapearTimer = 0;
					waveStartHeight = wavePos.y;

					if (doesLightningStrikeOnWaveCollision) {
						lightningTimer = 0;
					}

					//
					glm::vec3 waveWorldPos = wavePosToWorldSpace(wavePos);
					waveWorldPos.y = 0;
					glm::vec3 fromBoat = glm::normalize(waveWorldPos - boat.worldPosition());
					Sailor s(boat.worldPosition(), fromBoat);
					sailors.push_back(s);
				}
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
	//glUniform1f(uniWave, waveTimer * 0.2f);

	glUniform3fv(uniBigWave, 1, glm::value_ptr(wavePos));

	glUniform1f(uniStorminess, storminess);
	glUniform1f(uniLightning, lightningTimer);
	
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
	//lightningTimer += dt; //debug only

	//rotateCameraForTesting(dt);

	//make the camera bob
	glm::mat4 tmpView = camera.view;
	camera.view = glm::translate(camera.view, glm::vec3(0, 0.2 + (sin(totalTime) * 0.2), 0));

	if (hasGameStarted) {
		dramaUpdate(dt);	
	}
	
	waveUpdate(dt);
	boat.update(dt);
	flag.update(dt, boat.flagAnchorPoint(), boat.rotation);	
	
	flag.draw();
	boat.draw();
	drawOcean();

	lighthouse.update(dt);
	lighthouse.draw();

	rain.update(dt);
	rain.draw();


	if (boat.hasBoatSunk && !smoke.isPaused) {
		smoke.pause();
	}
	else if (smoke.isPaused && !boat.hasBoatSunk) {
		smoke.unpause();
	}

	for (int i = 0; i < sailors.size(); i++) {
		sailors[i].update(dt);
		sailors[i].draw();

		if (sailors[i].collisionWithBoat(boat.worldPosition())) {
			if (!boat.stunned()) {
				sailors[i].rescue();
				break;
			}
		}
		else if (sailors[i].rescued()) {
			sailors.erase(sailors.begin() + i);
			break;
		}
		else if (sailors[i].goneOffScreen()) {
			sailors.erase(sailors.begin() + i);
			break;
		}
	}

	smoke.position = boat.smokeAnchorPoint();
	smoke.update(dt);
	smoke.draw();

	camera.view = tmpView; //hack to keep base camera view stored
}

void on_quit() {
	Boat::DestroyModel();
	Flag::DestroyModel();
	Lighthouse::DestroyModel();
	Lightbeam::DestroyModel();
	Sailor::DestroyGeometry();
	rain.cleanup();
	smoke.cleanup();
	//destroy the ocean model???
}

void OnKeyDown(SDL_KeyboardEvent* key) {
	string keyname = SDL_GetKeyName( key->keysym.sym );
	//std::cout << "down " << keyname << std::endl;

	/*
	if (keyname == "Space") {
		lightningTimer = 0;
	}
	*/

	if (hasGameStarted) {
		boat.onkeydown(keyname);
	}
	else {

		std::cout << "test" <<std::endl;

		hasGameStarted = true;
		boat.startEntranceAnimation();
		lighthouse.moveTo(glm::vec3(15, -13.5, 30), 2);
	}

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

		// Clear the screen (move this into update?)
		glm::vec3 skyColor = glm::vec3(0.3, 0.7, 1.0);
		glm::vec3 stormColor = glm::vec3(0.1, 0.15, 0.25);
		glm::vec3 curColor = skyColor + (storminess * (stormColor - skyColor));
		if (lightningTimer < 0.3) {
			curColor = glm::vec3(1,1,1);	
		}
		else if (lightningTimer < 3) {
			glm::vec3 lightningColor = glm::vec3(0,0,0);
			if (lightningTimer < 1) {
				float lightningDelta = (lightningTimer - 0.3) / 0.7;
				curColor = glm::vec3(1,1,1) + ((lightningColor - glm::vec3(1,1,1)) * lightningDelta);
			}
			else if (lightningTimer > 2) {
				float lightningDelta = 1 - ((lightningTimer - 2) / 1);
				curColor = curColor + ((lightningColor - curColor) * lightningDelta);
			}
			else {
				curColor = lightningColor;
			}
		}
        glClearColor(curColor.x, curColor.y, curColor.z, 1);

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