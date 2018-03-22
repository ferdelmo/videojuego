#ifndef GEMA_H_
#define GEMA_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>

#include "Renderizable.h"

#include <random>
#include <ctime>
#include <cmath>

using namespace std;

class Escena;

class Gema : public Renderizable {
public:
	// propiedades del personaje

	const GLfloat velocidad = 3.75/6;
	GLfloat velRot = pi / 10;
	clock_t tiempecito = 0;
	
	//para generar numeros aleatorios
	mt19937 gen;
	uniform_real_distribution<float> distribution;

	Escena * es;

	int vida = 20;
	bool noCogida = true;

public:

	//CONSTRUCTOR EN UNA POSICION ESPECIFICA
	Gema(GLfloat x, GLfloat y, GLfloat z, Escena * es, GLFWwindow* window);
	
	void mover();

	void seguirPersonaje();

	bool colisionBala();

	int getVida();
};

#endif
