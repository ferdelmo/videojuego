#ifndef CALAVERA_BASE_H_
#define CALAVERA_BASE_H_

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>

#include "Renderizable.h"
#include <random>
#include <cmath>
#include <memory>

using namespace std;

class Escena;

class Camara;

class CalaveraBase : public Renderizable{
	public:
		GLfloat velocidad =3;
		GLfloat velRot = pi / 10;

		int vida = 20;
		bool muerto = false;

		//direccion para seguir al personaje
		GLfloat dir[3] = { 0,0,0 };

		//para generar numeros aleatorios
		mt19937 gen;
		uniform_real_distribution<float> distribution;

		Escena * es;

	public:
		CalaveraBase(GLfloat x, GLfloat y, GLfloat z, Escena * es, GLFWwindow* window, Camara * c);

		CalaveraBase(GLfloat x, GLfloat y, GLfloat z, Escena * es, GLFWwindow* window, Camara * c, GLuint sha);

		void seguir();

		GLfloat distancia(GLfloat x, GLfloat y, GLfloat xp, GLfloat yp);

		bool vivo();

		void mover();

		CalaveraBase operator=(const CalaveraBase& b);

		void fisicas();
};

#endif;