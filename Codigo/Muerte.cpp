#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>

#include "Muerte.h"
#include "Fondo.h"
#include "LoadShader.h"


using namespace std;

Muerte::Muerte(GLFWwindow *window, Camara *c, Fondo * f) {

	this->window = window;
	this->c = c;

	mode = 6;
	fondo = f;

	oldState = GLFW_RELEASE;
	pulsado = false;
}

void Muerte::controlesInFrame() {

	mode = 6;

	// Posición del cursor
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	int px, py;
	glfwGetWindowSize(window, &px, &py);

	//calula la posicion relativa de la raton en la pantalla
	x = (x / px - 0.5f) * 2;
	y = (abs(y - py) / py - 0.5f) * 2;

	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);

	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

	if (state == GLFW_RELEASE && oldState == GLFW_PRESS) {
		pulsado = false;
	}

	if (!pulsado) { // Control una pulsación
		if (state == GLFW_PRESS) {
			//cout << "x: " << x << " ;; y: " << y << endl;
			/*if (x >= 0.5 && x <= 0.96&& y >= -0.89 && y <= -0.74) { // BOTÓN MENÚ PRINCIPAL
				pulsado = true;
				mode = 1;
			}*/
		}
	}

	oldState = state;

}

int Muerte::renderizar() {
	fondo->renderizar();

	//controlesInFrame();

	return 6;
}
