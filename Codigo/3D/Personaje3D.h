#ifndef PERSONAJE3D_H_
#define PERSONAJE3D_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>

#include <random>
#include <cmath>
#include <thread>
#include <ctime>
#include <Windows.h>
#include <mmsystem.h>
#include <vector>
#include <stdlib.h>
#include "Render3D.h"
#include "../Camara.h"

using namespace std;

class Escena;

class Personaje3D : public Render3D {
public:
	Obj3D plano;
	const GLfloat velocidad = 2;
	GLuint numGemas = 0;

	//para disparar
	clock_t ultimaBala;
	float cadencia = 25;
	//para generar numeros aleatorios
	mt19937 gen;
	uniform_real_distribution<float> distribution;

	bool escopeta = true;
	bool vivo = true;
	bool escopetaGema = false;
	bool disparando = false;

	GLuint shaderProgramBala;

	Escena * es;
	bool modoDios = false;

	int up, down, left, right;

	Camara camaras[2];
	int camaraActual = 0;

	GLfloat lastXpos=0, lastYpos=0;
public:
	//CONSTRUCTOR EN UNA POSICION ESPECIFICA
	Personaje3D(glm::vec3 pos, Escena * es, GLFWwindow * window, Camara * c, Obj3D obj);

	void getPosition(GLfloat posi[]);

	void addGema();
	//FUNCION AUXILIAR CONTROLES POR INTERRUPCION TECLAS
	void controlesP(GLFWwindow* window, int key, int scancode, int action, int mods);
	//FUNCION AUXILIAR CONTROLES POR INTERRUPCION raton
	void mouseP(GLFWwindow* window, int button, int action, int mods);
	void morir();
	//dispara un escopetazo
	void escopetazo();

	//dispara una bala
	void lanzarBala();

	void setWindow(GLFWwindow* window);

	bool getPulsado();

	void controlesInFrame();

	GLfloat distancia(GLfloat x, GLfloat y, GLfloat xp, GLfloat yp);

	void mover();

	void fisicas();

};

#endif