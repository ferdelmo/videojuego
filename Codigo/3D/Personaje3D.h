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
#include <vector>
#include <stdlib.h>
#include "Render3D.h"
#include "../Camara.h"

#include <al.h>
#include <alc.h>

using namespace std;

class Escena3D;


class Personaje3D : public Render3D {
public:
	Obj3D plano;
	GLfloat velocidad = 5;
	GLfloat sensibilidad = 20;
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

	Escena3D * es;
	bool modoDios = true;

	int up, down, left, right;

	Camara camaras[2];
	int camaraActual = 0;

	double angHoriz=0, angVert=0;
	double lastXpos=0, lastYpos=0;

	Obj3D bala;
	glm::vec3 posCam = { 0,0,0 };

	bool cambioCam = true;


	ALuint buffer;
	ALsizei size;
	ALsizei frequency;
	ALenum format;
	int Sonido = -1;
public:
	//CONSTRUCTOR EN UNA POSICION ESPECIFICA
	Personaje3D(glm::vec3 pos, Escena3D * es, GLFWwindow * window, Camara * c, Obj3D obj);

	void addGema();

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