#ifndef BALA3D_H_
#define BALA3D_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>

#include <Windows.h>
#include <mmsystem.h>

#include "Render3D.h"

#include <ctime>

using namespace std;

class Camara;

class Bala3D : public Render3D {
	public:
		//Propiedades de la bala
		float velocidad = 20;

		int danyo = 5;
		clock_t inicio;

		int tiempoBala = 3;

		bool muerto = false;

		GLfloat limite = 2;

		Bala3D(glm::vec3 pos, glm::vec3 dir, GLFWwindow* window, Camara * c, Obj3D obj, int nivel);

		Bala3D operator=(const Bala3D& b);

		void avanzar();

		void morir();

		void mover();

		//void sonido(Bala& b);
};


#endif 
