#include "Personaje3D.h"

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

#include "../LoadShader.h"
#include "../Bala.h"
#include "../Escena.h"
#include "../CalaveraBase.h"
#include "../Camara.h"
#include "../Opciones.h"

using namespace std;

//CONSTRUCTOR POR DEFECTO
Personaje3D::Personaje3D(glm::vec3 pos, Escena * es, GLFWwindow * window, Camara * c, Obj3D obj)
	: Render3D(window, "../DevilDaggers/videojuego/Codigo/Shaders/3D.vert", "../DevilDaggers/videojuego/Codigo/Shaders/3D.frag", c,obj, { 0,1,1 },1) {
	this->es = es;
	this->pos = pos;
	//prepara las balas
	ultimaBala = clock();
	//numeros aleatorios
	distribution = uniform_real_distribution<float>(0, 1);
	random_device rd;
	// Initialize Mersenne Twister pseudo-random number generator
	gen = mt19937(rd());
	shaderProgramBala = LoadShaders("../DevilDaggers/videojuego/Codigo/Shaders/gema.vert", "../DevilDaggers/videojuego/Codigo/Shaders/gema.frag");

	Opciones o;
	up = o.up;
	down = o.down;
	right = o.right;
	left = o.left;

	numGemas = 1;
	Camara a;
	a.FoV = 60;
	a.View = glm::lookAt(
		pos, // Camera is at (4,3,3), in World Space
		pos+glm::vec3(1,0,0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	int px, py;
	glfwGetWindowSize(window, &px, &py);
	a.Projection = glm::perspective(
		glm::radians(a.FoV), // El campo de visión vertical, en radián: la cantidad de "zoom". Piensa en el lente de la cámara. Usualmente está entre 90° (extra ancho) y 30° (zoom aumentado)
		px*1.0f / py,       // Proporción. Depende del tamaño de tu ventana 4/3 == 800/600 == 1280/960, Parece familiar?
		0.1f,              // Plano de corte cercano. Tan grande como sea posible o tendrás problemas de precisión.
		100.0f             // Plano de corte lejano. Tan pequeño como se pueda.
	);
	camaras[0] = a;
}

void Personaje3D::getPosition(GLfloat posi[]) {
	posi[0] = pos[0];
	posi[1] = pos[1];
	posi[2] = pos[2];
}

void Personaje3D::addGema() {
	mciSendString("play ../DevilDaggers/videojuego/Codigo/Musica/gema.wav", NULL, 0, NULL);
	this->numGemas++;
}
//FUNCION AUXILIAR CONTROLES POR INTERRUPCION TECLAS
void Personaje3D::controlesP(GLFWwindow* window, int key, int scancode, int action, int mods) {
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
void Personaje3D::mouseP(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		escopetazo();
	}
}

void Personaje3D::morir() {
	cout << "muerto" << endl;
	vivo = false;
	mciSendString("play ../DevilDaggers/videojuego/Codigo/Musica/muerte.wav", NULL, 0, NULL);
}
//dispara un escopetazo
void Personaje3D::escopetazo() {
	int n = 3;
	//mciSendString("play ../DevilDaggers/videojuego/Codigo/Musica/disparo.wav", NULL, 0, NULL);
	//cout << "ESCOPETAZO " << endl;
	vector<shared_ptr<Bala>> bs = vector<shared_ptr<Bala>>(n);
	for (int i = 0; i < n; i++) {
		float ale = distribution(gen);
		float angle = (ale*pi / 6) - pi / 12;
		GLfloat punto1[3] = { pos[0],pos[1] + tam,pos[2] + ale / 10 };
		GLfloat punto1a[3] = { 0,0,0 };
		//rotatePoint(pos, punto1, orientacion, punto1a);
		bs[i] = make_shared<Bala>(Bala(punto1a, orientacion + pi / 2 + angle, window, cam, shaderProgramBala, log10(numGemas) + 1));
	}
	es->add(bs);
}

//dispara una bala
void Personaje3D::lanzarBala() {
	GLfloat punto1[3] = { pos[0] - tam / 2 , pos[1] + tam,pos[2] };
	GLfloat punto1a[3] = { 0,0,0 };
	//rotatePoint(pos, punto1, orientacion, punto1a);
	es->add(make_shared<Bala>(Bala(punto1a, orientacion + pi / 2, window, cam, shaderProgramBala, log10(numGemas) + 1)));
}

void Personaje3D::setWindow(GLFWwindow* window) {
	this->window = window;
}

bool Personaje3D::getPulsado() {
	return disparando;
}

void Personaje3D::controlesInFrame() {
	escopetaGema = false;
	int state = glfwGetKey(window, up);
	if (state == GLFW_PRESS) {
		pos[2] -= velocidad * 0.01;
	}
	state = glfwGetKey(window, left);
	if (state == GLFW_PRESS) {
		pos[0] -= velocidad * 0.01;
	}
	state = glfwGetKey(window, down);
	if (state == GLFW_PRESS) {
		pos[2] += velocidad * 0.01;
	}
	state = glfwGetKey(window, right);
	if (state == GLFW_PRESS) {
		pos[0] += velocidad * 0.01;
	}
	state = glfwGetKey(window, GLFW_KEY_Q);
	if (state == GLFW_PRESS) {
		orientacion += pi / 60 / 2;
	}
	state = glfwGetKey(window, GLFW_KEY_E);
	if (state == GLFW_PRESS) {
		orientacion -= pi/60/2;
	}

	//CONTROL CAMARA
	Camara a;
	a.FoV = 60;
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	a.View = glm::lookAt(
		pos, // Camera is at (4,3,3), in World Space
		pos + glm::vec3(0, 0, -1), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	int px, py;
	glfwGetWindowSize(window, &px, &py);
	a.Projection = glm::perspective(
		glm::radians(a.FoV), // El campo de visión vertical, en radián: la cantidad de "zoom". Piensa en el lente de la cámara. Usualmente está entre 90° (extra ancho) y 30° (zoom aumentado)
		px*1.0f / py,       // Proporción. Depende del tamaño de tu ventana 4/3 == 800/600 == 1280/960, Parece familiar?
		0.1f,              // Plano de corte cercano. Tan grande como sea posible o tendrás problemas de precisión.
		100.0f             // Plano de corte lejano. Tan pequeño como se pueda.
	);
	camaras[0] = a;
	/*
	int px, py;
	glfwGetWindowSize(window, &px, &py);

	//calula la posicion relativa de la raton en la pantalla
	x = (x / px - 0.5f) * 2;
	y = (abs(y - py) / py - 0.5f) * 2;
	y *= py * 1.0f / px;
	//calcula la orientacion para que mire al raton
	orientacion = atan2(-x, y);
	//cout << orientacion << endl;
	//mira si se ha disparado y ha pasado el tiempo de cadencia
	state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	int stateR = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);

	disparando = state == GLFW_PRESS;
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

		disparando = true;
		escopetaGema = true;
		escopeta = false;
	}
	else if (stateR == GLFW_RELEASE) {
		escopeta = true;
	}*/

}

GLfloat Personaje3D::distancia(GLfloat x, GLfloat y, GLfloat xp, GLfloat yp) {
	return (x - xp)*(x - xp) + (y - yp)*(y - yp);
}

void Personaje3D::mover() {
	/*cam->View = glm::lookAt(
		glm::vec3(pos[0], pos[1], pos[2]), // Camera is at (4,3,3), in World Space
		glm::vec3(pos[0]*cos(orientacion), pos[1], pos[2]*sin(orientacion)), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);*/
	cam->Projection = camaras[0].Projection;
	cam->View = camaras[0].View;
	cam->FoV = camaras[0].FoV;
	cam->actualizarMVP();
	controlesInFrame();
}

void Personaje3D::fisicas() {
	if ((pos[0] > es->getLimites() || pos[0] < -es->getLimites()) || (pos[1] > es->getLimites() || pos[1] < -es->getLimites())) {
		if (!modoDios) {
			morir();
		}
	}
}