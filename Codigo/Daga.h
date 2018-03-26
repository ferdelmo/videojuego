#ifndef DAGA_H_
#define DAGA_H_

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

class Bala;

class Daga : public Renderizable {

	GLfloat velocidad = 0.75f, velRot = 0.005f;
	bool generadaPos = false;
	bool muerto = false;
	clock_t tiempecito = 0;
	int generadas = -1;
	GLfloat posFinal[3] = { 0,0,0 };
	//direccion para seguir al personaje
	GLfloat dir[3] = { 0,0,0 };
	int nivel = 0;
	//Escena
	Escena * es;

	//para generar numeros aleatorios
	mt19937 gen;
	uniform_real_distribution<float> distribution;

	clock_t ultimaGen;
	int tiempoGen = 15;

	bool lanzar = true;
	
	GLuint sha,sha2,sha3;

	//cuando tenga 0 muere la torre
	vector<shared_ptr<Gema>> gemas;

	public:
		Daga(GLfloat posi[],Escena * es, int numGemas, GLFWwindow* window, Camara * c);

		Daga(GLfloat posi[], Escena * es, int numGemas, GLFWwindow* window, Camara * c, GLuint sha);

		bool sigueVivo();

		void GenerarCalaveras(int n);

		void GenerarCalaverasII(int n);

		void GenerarCalaverasIII(int n);

		void mover();

		void fisicas();
};

#endif