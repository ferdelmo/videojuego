#ifndef PARTIDA3D_H_
#define PARTIDA3D_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <ctime>
#include <random>
#include <thread>
#include "Render3D.h"

using namespace std;

class Escena3D;

class Partida3D {
	private:
		GLfloat daga1sec = 3.8;
		GLfloat daga2sec = 39;
		GLfloat daga3sec = 244;
		Escena3D * es;
		bool fin = false;
		float limite = 15;
		const GLfloat tam = 2;
		const GLfloat numPosis = 8;
		glm::vec3 posiposibles[8] = { 
			{limite,5,limite }, {-limite,5,limite } , { limite,5,-limite },
			{-limite,5,-limite }, {0,5,limite },{ -limite,5,0 } ,{ 0,5,-limite },{ -limite,5, 0 }
		};
		bool daga1p = true;
		bool daga2p = true;
		bool daga3p = true;

		GLuint shader;
		GLuint sha;
		clock_t daga1, daga2, daga3;

		//para generar numeros aleatorios
		mt19937 gen;
		uniform_int_distribution<> distribution;

		Obj3D * modelo;

	public:
		Partida3D(Escena3D * es);

		void start();

		void stop();
		
		void actualizar();

		void generarDaga1();

		void generarDaga2();

		void generarDaga3();
};

#endif
