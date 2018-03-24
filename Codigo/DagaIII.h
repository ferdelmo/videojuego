#ifndef DAGAIII_H_
#define DAGAIII_H_

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Renderizable.h"

#include <SOIL.h>
#include <random>
#include <cmath>
#include <ctime>
#include <vector>
#include <memory>


using namespace std;

class Escena;

class Gema;

class Camara;

class Personaje;

class DagaIII : public Renderizable {

	GLfloat velocidad = 1, velRot = 0.005f;

	GLfloat vida = 20/8;
	bool muerto = false;
	clock_t tiempecito = 0;
	int generadas = -1;
	//direccion para seguir al personaje
	GLfloat dir[3] = { 0,0,0 };

	//Escena
	Escena * es;

	//para generar numeros aleatorios
	mt19937 gen;
	uniform_real_distribution<float> distribution;

	clock_t ultimaGen;
	GLfloat tiempoGen = 5;

	bool lanzar = true;
	
	//cuando tenga 0 muere la torre
	vector<shared_ptr<Gema>> gemas;

	public:
		DagaIII(GLfloat posi[],Escena * es, int numGemas, GLFWwindow* window, Camara * c);

		bool sigueVivo();

		void GenerarCalaveras(int n);

		void mover();
};

#endif