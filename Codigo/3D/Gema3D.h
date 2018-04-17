#ifndef GEMA3D_H_
#define GEMA3D_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>

#include "Render3D.h"

#include <random>
#include <ctime>
#include <cmath>

using namespace std;

class Escena3D;

class Camara;

class Gema3D : public Render3D {
public:
	// propiedades del personaje
	const GLfloat velocidad = 7.5;
	GLfloat velRot = pi / 30;
	clock_t tiempecito = 0;
	
	//para generar numeros aleatorios
	mt19937 gen;
	uniform_real_distribution<float> distribution;

	Escena3D * es;

	int vida = 20;
	bool noCogida = true;

public:

	//CONSTRUCTOR EN UNA POSICION ESPECIFICA
	Gema3D(glm::vec3 posicion, glm::vec3 dir, Escena3D * es, GLFWwindow* window, Camara * c, Obj3D obj);

	void mover();
	
	void setPos(glm::vec3 nuevaPos);

	void seguirPersonaje();

	bool colisionBala();
};

#endif
