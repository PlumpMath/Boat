#pragma once

//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//SDL open gl
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

//GLM
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//standard libs
#include <string>
using std::string;
#include <vector>
using std::vector;

struct glApp {
	SDL_Window* window;
	SDL_GLContext context;
};

SDL_Window* start_sdl();
SDL_GLContext start_gl(SDL_Window* window);
glApp start();
void quit(glApp app);

string readFile(const char* filePath);

struct glAttribF {
	string name;
	int size;
};

struct glShaderData {
	GLuint vert;
	GLuint frag;
	GLuint program;
};

struct glMeshData {
	GLuint vbo;
	bool hasElements;
	GLuint ebo;
};

struct glModelData {
	GLuint vao;
	glShaderData shader;
	glMeshData mesh;
	int triCount;
};

glShaderData constructShaderProgram(string vertexShaderStr, string fragmentShaderStr);
glShaderData readAndConstructShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath);
void destroyShaderProgram(glShaderData shader);

void setAttrib(glAttribF attribData, int length, int offset, GLuint shaderProgram);
void initAttribs(vector<glAttribF> attribArray, GLuint shaderProgram);

GLuint setVertexBuffer(GLfloat vertices[], int count);
GLuint setElementBuffer(GLuint elements[], int count);

glMeshData setMesh(GLfloat vertices[], int vertCount, GLuint elements[], int elemCount);
glMeshData setMesh(GLuint vertices[], int vertCount);
void destroyMesh(glMeshData mesh);

glModelData buildModel(const char* vertexShaderPath, const char* fragmentShaderPath, GLfloat vertices[], int vertCount, GLuint elements[], int elemCount);
void destroyModel(glModelData model);