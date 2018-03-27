#ifndef CALAVERA_BASE_III_H_
#define CALAVERA_BASE_III_H_

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>
#include <Windows.h>
#include <mmsystem.h>

#include "Renderizable.h"
#include <random>
#include <cmath>

using namespace std;

class Escena;

class Camara;

class CalaveraBaseIII : public Renderizable {
	public:
		GLfloat velocidad = 3.5;
		GLfloat velRot = pi / 10;
		bool llegar = false;
		int vida = 35;
		bool muerto = false;

		//direccion para seguir al personaje
		GLfloat dir[3] = { 0,0,0 };

		//para generar numeros aleatorios
		mt19937 gen;
		uniform_real_distribution<float> distribution;

		Escena * es;

	public:
		CalaveraBaseIII(GLfloat x, GLfloat y, GLfloat z, Escena * es, GLFWwindow* window, Camara * c);

		CalaveraBaseIII(GLfloat x, GLfloat y, GLfloat z, Escena * es, GLFWwindow* window, Camara * c, GLuint sha);

		void seguir();

		bool vivo();

		void mover();

		CalaveraBaseIII operator=(const CalaveraBaseIII& b);

		void fisicas();
};

#endif;