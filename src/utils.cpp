#include "utils.h"

#include <iostream>
#include <fstream>

SDL_Window* start_sdl() {
	SDL_Window* window;
	//start SDL with video module	
	SDL_Init(SDL_INIT_VIDEO);

	//set OpenGL version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	//create window
	window = SDL_CreateWindow("OpenGL", 100, 100, 800, 600, SDL_WINDOW_OPENGL);

	return window;
}

SDL_GLContext start_gl(SDL_Window* window) {
	SDL_GLContext context;
	//create OpenGL context
	context = SDL_GL_CreateContext(window);

	//use glew to find experimental/modern OpenGl function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	//enable depth testing
	glEnable(GL_DEPTH_TEST);

	return context;
}

glApp start() {
	//open window & create context
	SDL_Window* window = start_sdl();
	SDL_GLContext context = start_gl( window );

	glApp app = {
		window, 
		context
	};
	return app;
}

void quit(glApp app) {
	//close window & delete context
	SDL_GL_DeleteContext(app.context);
	SDL_Quit();
}

string readFile(const char* filePath) {
    string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

glShaderData constructShaderProgram(string vertexShaderStr, string fragmentShaderStr) {
	//std::cout << vertexShaderStr << std::endl;
	//std::cout << fragmentShaderStr << std::endl;

	//get source from shader strings
	const char* vertexShaderSrc = vertexShaderStr.c_str();
	const char* fragmentShaderSrc = fragmentShaderStr.c_str();
	
	//make vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
	glCompileShader(vertexShader);

	//make fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fragmentShader);

	//make shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor"); //should I parameterize this later?
	//link the program to make it "compile"
	glLinkProgram(shaderProgram);

	glShaderData data = {
		vertexShader,
		fragmentShader,
		shaderProgram
	};
	return data; //NOTE: remember to USE THE FUCKING SHADER
	//return shaderProgram; //return shader program reference so you can use it later
}

glShaderData readAndConstructShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath) {
	return constructShaderProgram(readFile(vertexShaderPath), readFile(fragmentShaderPath));
}

void destroyShaderProgram(glShaderData shader) {
	glDeleteProgram(shader.program);
    glDeleteShader(shader.vert);
    glDeleteShader(shader.frag);
}

void setAttrib(glAttribF attribData, int length, int offset, GLuint shaderProgram) {
	GLint attrib = glGetAttribLocation(shaderProgram, attribData.name.c_str());
	glEnableVertexAttribArray(attrib);
	glVertexAttribPointer(attrib, attribData.size, GL_FLOAT, GL_FALSE, length * sizeof(float), (void*) (offset * sizeof(float)));
}

void initAttribs(vector<glAttribF> attribArray, GLuint shaderProgram) {
	int length = 0;
	int offset = 0;
	int i = 0;
	for (i = 0; i < attribArray.size(); i++) {
		length += attribArray[i].size;
	}
	std::cout << "length " << length << std::endl;
	for (i = 0; i < attribArray.size(); i++) {
		std::cout << attribArray[i].name << std::endl;
		std::cout << "offset " << offset << std::endl;
		setAttrib(attribArray[i], length, offset, shaderProgram);
		offset += attribArray[i].size;
	}
}

GLuint setVertexBuffer(GLfloat vertices[], int count) {

	GLuint vbo;

	//create vertex buffer
	glGenBuffers(1, &vbo);
	//make buffer the active object
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//send vertex data to vbo
	glBufferData(GL_ARRAY_BUFFER, count, vertices, GL_STATIC_DRAW);

	return vbo;
}

GLuint setElementBuffer(GLuint elements[], int count) {

	GLuint ebo;

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count, elements, GL_STATIC_DRAW);

	return ebo;
}

glMeshData setMesh(GLfloat vertices[], int vertCount, GLuint elements[], int elemCount) {
	glMeshData data = {
		setVertexBuffer(vertices, vertCount),
		true,
		setElementBuffer(elements, elemCount)
	};
	return data;
}

glMeshData setMesh(GLfloat vertices[], int vertCount) {
	glMeshData data = {
		setVertexBuffer(vertices, vertCount),
		false,
		0
	};
	return data;
}

void destroyMesh(glMeshData mesh) {
	glDeleteBuffers(1, &(mesh.vbo));
	if (mesh.hasElements) glDeleteBuffers(1, &(mesh.ebo));
}

glModelData buildModel(const char* vertexShaderPath, const char* fragmentShaderPath, GLfloat vertices[], int vertCount, GLuint elements[], int elemCount) {
	//init and bind Vertex Array Object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glModelData model = {
		vao,
		readAndConstructShaderProgram(vertexShaderPath, fragmentShaderPath),
		setMesh(vertices, vertCount, elements, elemCount),
		elemCount / 3
	};

	//should I put this in the shader wrapper too?
	glUseProgram(model.shader.program);

	return model;
}

void destroyModel(glModelData model) {
	destroyShaderProgram(model.shader);
	destroyMesh(model.mesh);
	glDeleteVertexArrays(1, &(model.vao));
}