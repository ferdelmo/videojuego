#ifndef CALAVERA_BASE3D_H_
#define CALAVERA_BASE3D_H_

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>

#include "Render3D.h"
#include "Escena3D.h"
#include <random>
#include <cmath>
#include <thread>

#include <al.h>
#include <alc.h>

using namespace std;

class Escena3D;

class Camara;

class Bala3D;

class Gema3D;

class SistemaParticulas;

class CalaveraBase3D : public Render3D {
	public:
		GLfloat velocidad = 4;
		GLfloat velRot = pi / 10;
		bool llegar = false;
		int vida = 35;
		bool muerto = false;
		int nivel = 0;
		clock_t tiempecito = clock();
		
		clock_t tiempoSonido = clock();
		bool haSonado = false;
		float intervalo = 10.0;
		bool pillada = false;
		double tiempoSeguir = abs(distribution(gen))*2.0;
		//direccion para seguir al personaje
		glm::vec3 dir = { 0,0,0 };
		GLfloat botarY;
		bool botecito = false;
		glm::vec3 posAux;
		//para generar numeros aleatorios
		mt19937 gen;
		uniform_real_distribution<float> distribution;
		uniform_real_distribution<float> distribution2;
		uniform_real_distribution<float> distribution3;
		uniform_real_distribution<float> distribution4;
		uniform_real_distribution<float> distributionXLejos;
		uniform_real_distribution<float> distributionYLejos;
		Escena3D * es;
		SistemaParticulas * sp;
		thread * spFin;

		ALuint buffer;
		ALsizei size;
		ALsizei frequency;
		ALenum format;
		int Sonido = -1;
	public:
		CalaveraBase3D(glm::vec3 pos, glm::vec3 dir, Escena3D * es, GLFWwindow* window, Camara * c, Obj3D obj, int nivel);

		//CalaveraBase3D(glm::vec3 pos, glm::vec3 dir, GLFWwindow* window, Camara * c, Obj3D obj, int nivel, GLuint sha);

		void seguir();

		bool vivo();

		void mover();

		CalaveraBase3D operator=(const CalaveraBase3D& b);

		void fisicas();
};

#endif;