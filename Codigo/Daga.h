#ifndef DAGA_H_
#define DAGA_H_

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <SOIL.h>
#include <random>
#include <cmath>
#include <ctime>
#include <vector>
#include <memory>


using namespace std;

class CalaveraBase;

class Daga {

	const double pi = atan(1) * 4;
	const GLfloat velocidad = 1;
	const GLfloat tam = 0.05f;
	GLfloat pos[3] = { 0.9f,0.9f,0 };
	GLfloat orientacion = pi / 2;
	//Para renderizar
	GLuint shaderProgram;
	GLuint VAO;
	GLuint EBO;
	GLuint points_VBO;
	GLuint colors_VBO;

	int vida = 20;
	bool muerto = false;

	//direccion para seguir al personaje
	GLfloat dir[3] = { 0,0,0 };
	//textura
	GLfloat colors[12] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f
	};
	GLfloat texCoords[8] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f

	};
	GLuint texCoords_VBO;
	GLuint texture;
	GLfloat vertices[12] = {
		0.5f, 0.5f, 0.0f, // Arriba dcha
		0.5f, -0.5f, 0.0f, // Abajo dcha
		-0.5f, -0.5f, 0.0f, // Abajo izqda
		-0.5f, 0.5f, 0.0f // Arriba izqda
	};
	GLuint indices[6] = {
		0, 1, 2, // Triángulo #1
		1, 3, 2 // Triángulo #2
	};

	int texWid, texHei, texChan;
	unsigned char* texImage = SOIL_load_image("../DevilDaggers/videojuego/Codigo/daga.png", &texWid,
		&texHei, &texChan, SOIL_LOAD_RGB);
	//Rota el punto "punto" sobre centro "angulo" grados(RAD) y lo guarda en rot
	void rotatePoint(GLfloat centro[], GLfloat punto[], GLfloat angulo, GLfloat rot[]) {
		rot[0] = cos(angulo)*(punto[0] - centro[0]) - sin(angulo)*(punto[1] - centro[1]) + centro[0];
		rot[1] = sin(angulo)*(punto[0] - centro[0]) + cos(angulo)*(punto[1] - centro[1]) + centro[1];
		rot[2] = centro[2];
	}

	//GUARDA Y ACTUALIZA LAS CALAVERAS GENERADAS POR ESTA DAGA
	vector<shared_ptr<CalaveraBase>> cb;

	//para generar numeros aleatorios
	mt19937 gen;
	uniform_real_distribution<float> distribution;

	clock_t ultimaGen;
	GLfloat tiempoGen = 5;

	bool lanzar = true;
	
	public:
		Daga();

		void GenerarCalaveras(int n);

		bool renderizar();
};

#endif