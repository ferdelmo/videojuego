#ifndef CALAVERA_BASE_H_
#define CALAVERA_BASE_H_

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>

#include <random>
#include <cmath>

using namespace std;

class Escena;

class CalaveraBase {
	private:

		const double pi = atan(1) * 4;
		//PROPIEDADES
		GLfloat velocidad =3;
		GLfloat velRot = pi / 10;
		const GLfloat tam = 0.05f;
		GLfloat pos[3] = { 1,1,0 };
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
		unsigned char* texImage = SOIL_load_image("../DevilDaggers/videojuego/Codigo/skull.png", &texWid,
			&texHei, &texChan, SOIL_LOAD_RGB);
		//Rota el punto "punto" sobre centro "angulo" grados(RAD) y lo guarda en rot
		void rotatePoint(GLfloat centro[], GLfloat punto[], GLfloat angulo, GLfloat rot[]) {
			rot[0] = cos(angulo)*(punto[0] - centro[0]) - sin(angulo)*(punto[1] - centro[1]) + centro[0];
			rot[1] = sin(angulo)*(punto[0] - centro[0]) + cos(angulo)*(punto[1] - centro[1]) + centro[1];
			rot[2] = centro[2];
		}

		//para generar numeros aleatorios
		mt19937 gen;
		uniform_real_distribution<float> distribution;

		Escena * es;

	public:
		CalaveraBase(Escena * es);

		CalaveraBase(GLfloat x, GLfloat y, GLfloat z, Escena * es);

		void seguir();

		GLfloat distancia(GLfloat x, GLfloat y, GLfloat xp, GLfloat yp);

		bool vivo();

		CalaveraBase operator=(const CalaveraBase& b);

		bool renderizar();
};

#endif;