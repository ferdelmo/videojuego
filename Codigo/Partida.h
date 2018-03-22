#ifndef PARTIDA_H_
#define PARTIDA_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <ctime>

using namespace std;

class Escena;

class Partida {
	private:
		GLfloat daga1sec;
		GLfloat daga2sec;
		GLfloat daga3sec;
		Escena * es;
	public:
		Partida(Escena * es);

		void start();

		void generarDaga1();

		void generarDaga2();

		void generarDaga3();
};

#endif
