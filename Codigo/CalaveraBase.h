#ifndef CALAVERA_BASE_H_
#define CALAVERA_BASE_H_

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
#include <ctime>
#include <memory>

using namespace std;

class Escena;

class Camara;

class CalaveraBase : public Renderizable{
	public:
		GLfloat velocidad = 3;
		GLfloat velRot = pi / 10;
		bool llegar = false;
		clock_t tiempecito = clock();
		int vida = 20;
		bool muerto = false;
				//direccion para seguir al personaje
		GLfloat dir[3] = { 0,0,0 };

		double tiempoSeguir = abs(distribution(gen))*2.0;

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