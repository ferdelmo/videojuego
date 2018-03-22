#ifndef PERSONAJE_H_
#define PERSONAJE_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>

#include <random>
#include <cmath>
#include <ctime>
#include <vector>
#include <stdlib.h>

#include "Renderizable.h"


using namespace std;

class Escena;

class Personaje : public Renderizable{
	public:
		const GLfloat velocidad = 3.75;
		GLuint numGemas = 0;

		//para disparar
		clock_t ultimaBala;
		float cadencia=50;
		float margenBala = 50;
		//para generar numeros aleatorios
		mt19937 gen;
		uniform_real_distribution<float> distribution;

		bool escopeta = true;
		bool vivo = true;


		Escena * es;

	public:
		//CONSTRUCTOR EN UNA POSICION ESPECIFICA
		Personaje(GLfloat x, GLfloat y, GLfloat z, Escena * es, GLFWwindow * window);

		void getPosition(GLfloat posi[]);
		
		void addGema();
		//FUNCION AUXILIAR CONTROLES POR INTERRUPCION TECLAS
		void controlesP(GLFWwindow* window, int key, int scancode, int action, int mods);
		//FUNCION AUXILIAR CONTROLES POR INTERRUPCION raton
		void mouseP(GLFWwindow* window, int button, int action, int mods);
		//dispara un escopetazo
		void escopetazo();

		//dispara una bala
		void lanzarBala();

		void setWindow(GLFWwindow* window);
		
		bool getPulsado();

		void controlesInFrame();
		
		bool sigueVivo();

		GLfloat distancia(GLfloat x, GLfloat y, GLfloat xp, GLfloat yp);

		void mover();

};

#endif