#ifndef BALA_H_
#define BALA_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>

#include "Renderizable.h"

#include <ctime>

using namespace std;

class Bala : public Renderizable {
	public:
		//Propiedades de la bala
		float velocidad = 7.5;

		int danyo = 5;
		clock_t inicio;

		int tiempoBala = 3;

		bool muerto = false;

		Bala(GLfloat inicio[], float o, GLFWwindow* window);

		Bala operator=(const Bala& b);

		void avanzar();

		void morir();

		void mover();
};


#endif 
