#include "Personaje.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>

#include <random>
#include <cmath>
#include <ctime>
#include <vector>
#include <stdlib.h>

#include "LoadShader.h"
#include "Bala.h"
#include "Escena.h"
#include "CalaveraBase.h"

using namespace std;

//CONSTRUCTOR POR DEFECTO
Personaje::Personaje(GLfloat x, GLfloat y, GLfloat z, Escena * es, GLFWwindow * window) 
	: Renderizable(window,"../DevilDaggers/videojuego/Codigo/CARETO.jpg","../DevilDaggers/videojuego/Codigo/Shaders/tri.vert","../DevilDaggers/videojuego/Codigo/Shaders/tri.frag", 0.1f) {
	this->es = es;
	//prepara las balas
	ultimaBala = clock();
	//numeros aleatorios
	distribution = uniform_real_distribution<float>(0, 1);
	random_device rd;
	// Initialize Mersenne Twister pseudo-random number generator
	gen = mt19937(rd());
}

void Personaje::getPosition(GLfloat posi[]) {
	posi[0] = pos[0];
	posi[1] = pos[1];
	posi[2] = pos[2];
}

void Personaje::addGema() {
	this->numGemas++;
}
//FUNCION AUXILIAR CONTROLES POR INTERRUPCION TECLAS
void Personaje::controlesP(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	/*else if (key == GLFW_KEY_W && state==GLFW_PRESS) {
	pos[1] += velocidad*0.01f;
	}
	else if (key == GLFW_KEY_A && state == GLFW_PRESS) {
	pos[0] -= velocidad*0.01f;
	}
	else if (key == GLFW_KEY_S && state == GLFW_PRESS) {
	pos[1] -= velocidad* 0.01f;
	}
	else if (key == GLFW_KEY_D && state == GLFW_PRESS) {
	pos[0] += velocidad* 0.01f;
	}*/

}
//FUNCION AUXILIAR CONTROLES POR INTERRUPCION raton
void Personaje::mouseP(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		escopetazo();
	}
}
//dispara un escopetazo
void Personaje::escopetazo() {
	//cout << "ESCOPETAZO " << endl;
	for (int i = 0; i < 6; i++) {
		float ale = distribution(gen);
		float angle = (ale*pi / 6) - pi / 12;
		GLfloat punto1[3] = { pos[0],pos[1] + tam,pos[2] + ale / 10 };
		GLfloat punto1a[3] = { 0,0,0 };
		int px, py;
		glfwGetWindowSize(window, &px, &py);
		punto1a[0] *= py * 1.0f / px;
		rotatePoint(pos, punto1, orientacion, punto1a);
		es->add(make_shared<Bala>(Bala(punto1a, orientacion + pi / 2 + angle,window)));
		cout << "BALITA " << angle << endl;
	}
}
//dispara una bala
void Personaje::lanzarBala() {
	GLfloat punto1[3] = { pos[0],pos[1] + tam,pos[2] };
	GLfloat punto1a[3] = { 0,0,0 };
	rotatePoint(pos, punto1, orientacion, punto1a);
	es->add(make_shared<Bala>(Bala(punto1a, orientacion + pi / 2,window)));
}

void Personaje::setWindow(GLFWwindow* window) {
	this->window = window;
}

bool Personaje::getPulsado() {
	return GLFW_PRESS == glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) || GLFW_PRESS == glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
}

void Personaje::controlesInFrame() {
	int state = glfwGetKey(window, GLFW_KEY_W);
	if (state == GLFW_PRESS) {
		pos[1] += velocidad * 0.005;
	}
	state = glfwGetKey(window, GLFW_KEY_A);
	if (state == GLFW_PRESS) {
		pos[0] -= velocidad * 0.005;
	}
	state = glfwGetKey(window, GLFW_KEY_S);
	if (state == GLFW_PRESS) {
		pos[1] -= velocidad * 0.005;
	}
	state = glfwGetKey(window, GLFW_KEY_D);
	if (state == GLFW_PRESS) {
		pos[0] += velocidad * 0.005;
	}
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	int px, py;
	glfwGetWindowSize(window, &px, &py);

	//calula la posicion relativa de la raton en la pantalla
	x = (x / px - 0.5f) * 2;
	y = (abs(y - py) / py - 0.5f) * 2;
	//calcula la orientacion para que mire al raton
	float dirx = x - pos[0];
	float diry = y - pos[1];
	orientacion = atan2(-dirx, diry);
	//cout << orientacion << endl;
	//mira si se ha disparado y ha pasado el tiempo de cadencia
	state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	int stateR = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	if (state == GLFW_PRESS && (clock() - ultimaBala) / (CLOCKS_PER_SEC / 1000) > cadencia) {
		lanzarBala();
		ultimaBala = clock();
		escopeta = false;
	}
	else if (state == GLFW_PRESS) {
		escopeta = false;
	}
	else if (stateR == GLFW_PRESS && escopeta) {
		escopetazo();
		escopeta = false;
	}
	else if (stateR == GLFW_RELEASE) {
		escopeta = true;
	}
	
}

GLfloat Personaje::distancia(GLfloat x, GLfloat y, GLfloat xp, GLfloat yp) {
	return (x - xp)*(x - xp) + (y - yp)*(y - yp);
}

bool Personaje::sigueVivo() {
	int i = 0;
	vector<shared_ptr<CalaveraBase>> * cb = es->getCalaveras();
	while (i < cb->size()) {
		//cout << distancia(pos[0], pos[1], b[i].pos[0], b[i].pos[1]) << endl;
		if (distancia(pos[0], pos[1], cb->at(i)->pos[0], cb->at(i)->pos[1]) <= 3 * tam * tam) {
			vivo = false;
			i++;
		}
		else {
			i++;
		}
	}
	return vivo;
}

void Personaje::mover() {
	controlesInFrame();
	//sigue = sigueVivo();
}
