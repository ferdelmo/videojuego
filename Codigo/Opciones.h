#ifndef OPCIONES_H
#define OPCIONES_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>

using namespace std;

class Opciones {

	int mode;

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
	unsigned char* texImage = SOIL_load_image("../DevilDaggers/videojuego/Codigo/Opciones.jpg", &texWid,
		&texHei, &texChan, SOIL_LOAD_RGB);

	GLFWwindow *window;

public:

	Opciones(GLFWwindow *window);

	void controlesInFrame();

	int renderizar();

};

#endif
