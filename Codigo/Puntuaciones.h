#ifndef PUNTUACIONES_H
#define PUNTUACIONES_

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>

#include "Fondo.h"
#include "Camara.h"

#include <string>
#include <fstream>

using namespace std;

class Puntuaciones {

	int mode;

	string topPunt[10];

	int oldState;
	bool pulsado;

	//Para renderizar
	GLuint shaderProgram;
	GLuint VAO;
	GLuint EBO;
	GLuint points_VBO;
	GLuint colors_VBO;

	// textura
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

	GLfloat vertices[12] = {
		-1.0f, 1.0f, 0.0f, // Arriba izqda
		1.0f, 1.0f, 0.0f, // Arriba dcha
		-1.0f, -1.0f, 0.0f, // Abajo izda
		1.0f, -1.0f, 0.0f // Abajo dcha
	};

	GLuint indices[6] = {
		0, 2, 3, // Triángulo #1
		1, 0, 3 // Triángulo #2

	};

	GLuint texCoords_VBO;
	GLuint texture;

	int texWid, texHei, texChan;
	unsigned char* texImage = SOIL_load_image("../DevilDaggers/videojuego/Codigo/MejoresPuntuaciones.jpg", &texWid,
		&texHei, &texChan, SOIL_LOAD_RGB);

	GLFWwindow *window;
	Camara * c;


public:
	const string fich = "puntuaciones.txt";

	void displayText(float x, float y, int r, int g, int b, string str) {
		glUseProgram(0);

		glLoadIdentity();
		glRasterPos2i(x, y); glDisable(GL_LIGHTING);
		glColor3f(r, g, b);
		/*float posXcorrected = 0;
		for (int i = 0; i < str.size() / 2; i++) {
		posXcorrected += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, str[i]);
		}*/
		glRasterPos3f(x, y, 0);

		glDisable(GL_TEXTURE_2D);
		for (int i = 0; i < str.size(); i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
		}
		glEnable(GL_TEXTURE_2D);
	}

	Puntuaciones(GLFWwindow *window, Camara * c);

	void LeerFichero();

	void controlesInFrame();

	int renderizar();

};

#endif
