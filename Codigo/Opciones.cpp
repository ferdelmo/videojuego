#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>

#include "Opciones.h"
#include "LoadShader.h"
#include <fstream>
#include <string>
#include <sstream>

using namespace std;



Opciones::Opciones() {
	mode = 3;
	ifstream myfile;
	myfile.open(fich);
	if (myfile.is_open()) {
		myfile >> px >> py >> resolucion;
		myfile >> Fullscreen;
		myfile >> up >> down >> right >> left;
		myfile.close();
	}
	else {
		//generamos el opciones por defecto
		ofstream myfile;
		myfile.open(fich);
		px = resoluciones[resolucion][0]; py = resoluciones[resolucion][1];
		myfile << px << " " << py << " " << resolucion << '\n';
		myfile << Fullscreen << '\n';
		myfile << up << " " << down << " " << right << " " << left << '\n';
		myfile.close();
	}
}

void Opciones::setFlechas(Fondo * izq, Fondo * der) {
	this->izq = izq;
	this->der = der;
	GLfloat texCoords[8] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

	};
	for (int i = 0; i < 8; i++) {
		this->izq->texCoords[i] = texCoords[i];
	}
	GLfloat texCoords2[8] = {
		1.0f, 0.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

	};
	for (int i = 0; i < 8; i++) {
		this->der->texCoords[i] = texCoords2[i];
	}
}

void Opciones::setBotones(Fondo * atras, Fondo * guardar) {
	this->atras = atras;
	this->guardar = guardar;
	GLfloat texCoords[8] = {
		1.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,

	};
	for (int i = 0; i < 8; i++) {
		this->atras->texCoords[i] = texCoords[i];
	}
	GLfloat texCoords2[8] = {
		1.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,

	};
	for (int i = 0; i < 8; i++) {
		this->guardar->texCoords[i] = texCoords2[i];
	}
}

void Opciones::setWindow(GLFWwindow * window) {
	this->window = window;
}

void Opciones::controles() {
	mode = 3;
	if (!cambiando) {
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		x = (x / (px*1.0f) - 0.5f) * 2;
		y = (abs(y - py) / (py*1.0f) - 0.5f) * 2;

		if (x > -0.25 && x < 0.3 && y>0.2 && y < 0.3 && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			cout << "PULSADO TECLA DE ARRIBA" << endl;
			cout << "_______________________" << endl;
			cambiando = true;
			aCambiar = 0;
		}
		else if (x > -0.25 && x < 0.3 && y>0.1 && y < 0.2 && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			cout << "PULSADO TECLA DE ABAJO" << endl;
			cout << "_______________________" << endl;
			cambiando = true;
			aCambiar = 1;
		}
		else if (x > -0.25 && x < 0.3 && y>0.0 && y < 0.1 && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			cout << "PULSADO TECLA DE IZQUIERDA" << endl;
			cout << "_______________________" << endl;
			cambiando = true;
			aCambiar = 2;
		}
		else if (x > -0.25 && x < 0.3 && y>-0.1 && y < 0.0 && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			cout << "PULSADO TECLA DE DERECHA" << endl;
			cout << "_______________________" << endl;
			cambiando = true;
			aCambiar = 3;
		}
		else if (x > -0.2 && x < -0.05 && y>0.3 && y < 0.5 && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			cout << "PULSADO FLECHA IZQ" << endl;
			cout << "_______________________" << endl;
			if (resolucion > 0 && !pulsado) {
				resolucion--;
			}
			pulsado = true;
		}
		else if (x > 0.05 && x < 0.2 && y>0.3 && y < 0.5 && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			cout << "PULSADO FLECHA DER" << endl;
			cout << "_______________________" << endl;
			if (resolucion < 4 && !pulsado) {
				resolucion++;
			}
			pulsado = true;
		}
		else if (x > -0.2 && x < -0.05 && y>0.3 && y < 0.5 && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			cout << "PULSADO FLECHA IZQ" << endl;
			cout << "_______________________" << endl;
			if (resolucion > 0 && !pulsado) {
				resolucion--;
			}
			pulsado = true;
		}
		else if (x > 0.05 && x < 0.2 && y>0.3 && y < 0.5 && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			cout << "PULSADO FLECHA DER" << endl;
			cout << "_______________________" << endl;
			if (resolucion < 4 && !pulsado) {
				resolucion++;
			}
			pulsado = true;
		}
		else if (x > -0.4 && x < -0.1 && y>-1 && y < -0.5 && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			cout << "GUARDAR" << endl;
			cout << "_______________________" << endl;
			mode = 1;
			//EScribe los datitos
			px = resoluciones[resolucion][0]; py = resoluciones[resolucion][1];
			ofstream myfile(fich);
			myfile << px << " " << py << " " << resolucion << '\n';
			myfile << Fullscreen << '\n';
			myfile << up << " " << down << " " << right << " " << left << '\n';
			myfile.close();
		}
		else if (x > 0.1 && x < 0.4 && y>-1 && y < -0.5 && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			cout << "ATRAS" << endl;
			cout << "_______________________" << endl;
			mode = 1;
			cargar();
		}
		if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			pulsado = false;
		}
	}
	else {
		cambiando = !cogerTecla();
	}
}

int Opciones::renderizar() {
	controles();
	if (!cambiando) {
		displayText(-0.05f, 0.8f, 1, 1, 1, "OPCIONES");
		displayText(-0.05f, 0.45f, 1, 1, 1, to_string(resoluciones[resolucion][0]) + "x" + to_string(resoluciones[resolucion][1]));
		izq->renderizar();
		der->renderizar();
		atras->renderizar();
		guardar->renderizar();
		stringstream ssa;
		string sa;
		char a = up;
		ssa << a;
		ssa >> sa;
		displayText(-0.2f, 0.2f, 1, 1, 1, "TECLA MOVERSE ARRIBA (click para cambiar): " + sa);
		stringstream ssb;
		string sb;
		char b = down;
		ssb << b;
		ssb >> sb;
		displayText(-0.2f, 0.1f, 1, 1, 1, "TECLA MOVERSE ABAJO (click para cambiar): " + sb);
		stringstream ssc;
		string sc;
		char c = left;
		ssc << c;
		ssc >> sc;
		displayText(-0.2f, 0.0f, 1, 1, 1, "TECLA MOVERSE IZQUIERDA (click para cambiar): " + sc);
		stringstream ssd;
		string sd;
		char d = right;
		ssd << d;
		ssd >> sd;
		displayText(-0.2f, -0.1f, 1, 1, 1, "TECLA MOVERSE DERECHA (click para cambiar): " + sd);
	}
	else {
		displayText(-0.01f, 0.0f, 1, 1, 1, "PULSE LA NUEVA TECLA");
	}

	return mode;
}

void Opciones::cargar() {
	ifstream myfile;
	myfile.open(fich);
	if (myfile.is_open()) {
		myfile >> px >> py >> resolucion;
		myfile >> Fullscreen;
		myfile >> up >> down >> right >> left;
		myfile.close();
	}
}