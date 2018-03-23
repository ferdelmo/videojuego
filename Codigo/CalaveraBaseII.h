#ifndef CALAVERA_BASE_II_H_
#define CALAVERA_BASE_II_H_

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>

#include "Renderizable.h"
#include <random>
#include <cmath>

using namespace std;

class Escena;

class Camara;

class CalaveraBaseII : public Renderizable {
	public:
		GLfloat velocidad =3;
		GLfloat velRot = pi / 10;

		int vida = 25;
		bool muerto = false;

		//direccion para seguir al personaje
		GLfloat dir[3] = { 0,0,0 };

		//para generar numeros aleatorios
		mt19937 gen;
		uniform_real_distribution<float> distribution;

		Escena * es;

	public:
		CalaveraBaseII(GLfloat x, GLfloat y, GLfloat z, Escena * es, GLFWwindow* window, Camara * c);

		void seguir();

		GLfloat distancia(GLfloat x, GLfloat y, GLfloat xp, GLfloat yp);

		bool vivo();

		void mover();

		CalaveraBaseII operator=(const CalaveraBaseII& b);
};

#endif;