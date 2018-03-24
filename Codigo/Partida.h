#ifndef PARTIDA_H_
#define PARTIDA_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <ctime>
#include <random>
#include <thread>

using namespace std;

class Escena;

class Partida {
	private:
		GLfloat daga1sec = 3.8;
		GLfloat daga2sec = 39;
		GLfloat daga3sec = 244;
		Escena * es;
		bool fin = false;
		const GLfloat tam = 4;
		const GLfloat numPosis = 4;
		GLfloat posiposibles[4][3] = { 
			{tam,tam,0}, {-tam,tam,0} , {tam,-tam,0}, {-tam,-tam,0} 
		};
		
		GLuint shader;
		clock_t daga1, daga2, daga3;

		//para generar numeros aleatorios
		mt19937 gen;
		uniform_int_distribution<> distribution;

	public:
		Partida(Escena * es);

		void start();

		void stop();
		
		void actualizar();

		void generarDaga1();

		void generarDaga2();

		void generarDaga3();
};

#endif
