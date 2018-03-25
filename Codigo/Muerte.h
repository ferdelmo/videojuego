#ifndef MUERTE_H
#define MUERTE_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>

#include <string>
#include <fstream>

using namespace std;

class Camara;

class Fondo;

class Muerte {

	int mode;

	int oldState;
	bool pulsado;

	GLFWwindow *window;
	Camara *c;

	Fondo * fondo;


public:

	Muerte(GLFWwindow *window, Camara *c, Fondo * f);

	void controlesInFrame();

	int renderizar();

};

#endif
