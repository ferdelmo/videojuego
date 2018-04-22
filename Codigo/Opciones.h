#ifndef OPCIONES_H
#define OPCIONES_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>
#include <GL/freeglut.h>
#include "Fondo.h"

using namespace std;

class Opciones {
public:
	int mode;

	const string fich = "opciones.txt";

	int resolucion = 1;
	int modo = 1, aux_modo = 1;
	int oldresolucion = 1;

	const int resoluciones[5][2] = {
		{ 800,600 },{ 1024,768 },{ 1366,768 },{ 1920,1080 },{ 2560,1440 }
	};

	bool Fullscreen = false;
	int up = GLFW_KEY_W, down = GLFW_KEY_S, right = GLFW_KEY_D, left = GLFW_KEY_A;
	int aux_up = GLFW_KEY_W, aux_down = GLFW_KEY_S, aux_right = GLFW_KEY_D, aux_left = GLFW_KEY_A;

	int px = 1920, py = 1080;

	bool cambiando = false;
	int aCambiar = 0; // 0,1,2,3 -> arriba, abajko, izq,der

	bool pulsado;
	int oldState;

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
	Camara * c;

public:

	Opciones();

	void setValue(GLFWwindow *window, Camara * c);

	void restaurarValores();

	bool cogerTecla();
	
	void controlesInFrame();

	void cargar();

	void mostrarTexto();

	int renderizar();

};

#endif