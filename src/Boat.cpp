#include "Boat.h"

//static functions
void Boat::InitModel() {
	GLfloat vertices[] = {
		//x, y, z,	r, g, b,	nx, ny, nz
		//all vertices start at the front and go clockwise around

		//HULL
		//layer 1 = 0 - 6
		0, 0, 8,		0, 0, 0,	0, 0, 0,
		2.5, 0, 7,		0, 0, 0, 	0, 0, 0,
		2, 0, -6,		0, 0, 0,	0, 0, 0,
		1.5, 0, -8,		0, 0, 0,	0, 0, 0,
		-1.5, 0, -8,	0, 0, 0,	0, 0, 0,
		-2, 0, -6,		0, 0, 0,	0, 0, 0,
		-2.5, 0, 7,		0, 0, 0,	0, 0, 0, 

		//layer 2 = 7 - 13
		0, 3, 9,		0, 0, 0,	0, 0, 0,
		3, 2.5, 7,		0, 0, 0, 	0, 0, 0,
		2.5, 2, -6,		0, 0, 0,	0, 0, 0,
		2, 2.5, -9,		0, 0, 0,	0, 0, 0,
		-2, 2.5, -9,	0, 0, 0,	0, 0, 0,
		-2.5, 2, -6,	0, 0, 0,	0, 0, 0,
		-3, 2.5, 7,		0, 0, 0,	0, 0, 0, 

		//LOWER DECK

		//TOP DECK

		//SMOKESTACK
	};

	GLuint elements[] = {
		//HULL
		//sides = 0 - 12
		0, 7, 1,
		1, 7, 8,
		1, 8, 2,
		2, 8, 9,
		2, 9, 3,
		3, 9, 10,
		3, 10, 4,
		4, 10, 11,
		4, 11, 5,
		5, 11, 12,
		5, 12, 6,
		6, 12, 13,
		6, 13, 0,
		//top

		//LOWER DECK

		//TOP DECK

		//SMOKESTACK
	};
}

void Boat::DestroyModel() {

}

//methods
Boat::Boat() {

}

void Boat::update(float dt) {

}

void Boat::draw() {

}