#ifndef DAGA3D_H_
#define DAGA3D_H_

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Render3D.h"

#include <SOIL.h>
#include <random>
#include <cmath>
#include <ctime>
#include <vector>
#include <memory>


using namespace std;

class Escena3D;

class Gema3D;

class CalaveraBase3D;

class Camara;

class Personaje3D;

class Bala3D;

class Daga3D : public Render3D {

	GLfloat velocidad = 2, velRot = 0.1f;
	bool generadaPos = false;
	bool muerto = false;
	clock_t tiempecito = 0;
	int generadas = -1;
	glm::vec3 posFinal = { 0,0,0 };
	//direccion para seguir al personaje
	glm::vec3 dir = { 0,0,0 };
	int nivel = 0;
	//Escena
	Escena3D * es;

	//para generar numeros aleatorios
	mt19937 gen;
	uniform_real_distribution<float> distribution;
	uniform_real_distribution<float> distribution2;

	clock_t ultimaGen;
	int tiempoGen = 15;

	bool lanzar = true;
	
	GLuint sha,sha2,sha3;

	//cuando tenga 0 muere la torre
	vector<shared_ptr<Gema3D>> gemas;

	public:
		Daga3D(glm::vec3 pos, glm::vec3 dir, Escena3D * es, GLFWwindow* window, Camara * c, Obj3D obj, int nivel);


		bool sigueVivo();

		void GenerarCalaveras(int n);

		void mover();

		void fisicas();
};

#endif