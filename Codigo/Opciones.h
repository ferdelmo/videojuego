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
	const int resoluciones[5][2] = {
		{ 800,600 },{ 1024,768 },{ 1366,768 },{ 1920,1080 },{ 2560,1440 }
	};

	bool Fullscreen = false;
	int up = GLFW_KEY_W, down = GLFW_KEY_S, right = GLFW_KEY_D, left = GLFW_KEY_A;

	int px = 1920, py = 1080;

	Fondo * izq, *der;

	Fondo * atras, *guardar;

	bool cambiando = false;

	int aCambiar = 0; // 0,1,2,3 -> arriba, abajko, izq,der
	GLFWwindow * window;

	bool pulsado = true;

public:
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

	bool cogerTecla()
	{
		for (int i = 32; i < 256; i++) {
			int state = glfwGetKey(window, i);
			if (state == GLFW_PRESS) {
				if (aCambiar == 0) {
					up = i;
				}
				else if (aCambiar == 1) {
					down = i;
				}
				else if (aCambiar == 2) {
					left = i;
				}
				else if (aCambiar == 3) {
					right = i;
				}
				i = 256;
				return true;
			}
		}
		return false;
	}

	Opciones();

	void setFlechas(Fondo * izq, Fondo * der);

	void setBotones(Fondo * atras, Fondo * guardar);

	void setWindow(GLFWwindow * window);

	void controles();

	void cargar();

	int renderizar();

};

#endif