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
#include "Bala3D.h"
#include "Escena3D.h"
#include "../CalaveraBase.h"
#include "../Camara.h"
#include "../Opciones.h"

using namespace std;

//CONSTRUCTOR POR DEFECTO
Personaje3D::Personaje3D(glm::vec3 pos, Escena3D * es, GLFWwindow * window, Camara * c, Obj3D obj)
	: Render3D(window, "../DevilDaggers/videojuego/Codigo/Shaders/3D.vert", "../DevilDaggers/videojuego/Codigo/Shaders/3D.frag", c,obj, { 0,1,1 },1) {
	this->es = es;
	this->pos = pos;
	//prepara las balas
	ultimaBala = clock();
	//numeros aleatorios
	distribution = uniform_real_distribution<float>(-1, 1);
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
		glm::radians(a.FoV), // El campo de visi�n vertical, en radi�n: la cantidad de "zoom". Piensa en el lente de la c�mara. Usualmente est� entre 90� (extra ancho) y 30� (zoom aumentado)
		px*1.0f / py,       // Proporci�n. Depende del tama�o de tu ventana 4/3 == 800/600 == 1280/960, Parece familiar?
		0.1f,              // Plano de corte cercano. Tan grande como sea posible o tendr�s problemas de precisi�n.
		100.0f             // Plano de corte lejano. Tan peque�o como se pueda.
	);
	camaras[0] = a;
	Render3D::loadOBJ("../DevilDaggers/videojuego/Codigo/3D/bala.obj", bala.vertices, bala.uvs, bala.normals);
}

void Personaje3D::getPosition(GLfloat posi[]) {
	posi[0] = pos[0];
	posi[1] = pos[1];
	posi[2] = pos[2];
}

void Personaje3D::addGema() {
	//mciSendString("play ../DevilDaggers/videojuego/Codigo/Musica/gema.wav", NULL, 0, NULL);
	this->numGemas++;
}

void Personaje3D::morir() {
	cout << "muerto" << endl;
	vivo = false;
	mciSendString("play ../DevilDaggers/videojuego/Codigo/Musica/muerte.wav", NULL, 0, NULL);
}
//dispara un escopetazo
void Personaje3D::escopetazo() {
	int n = 8;
	//mciSendString("play ../DevilDaggers/videojuego/Codigo/Musica/disparo.wav", NULL, 0, NULL);
	//cout << "ESCOPETAZO " << endl;
	vector<shared_ptr<Bala3D>> bs = vector<shared_ptr<Bala3D>>(n);
	for (int i = 0; i < n; i++) {
		glm::vec3 auxDir = { distribution(gen), distribution(gen), distribution(gen)};
		auxDir = direccion + auxDir * 0.2f;
		auxDir = auxDir / glm::length(auxDir);
		//rotatePoint(pos, punto1, orientacion, punto1a);
		bs[i] = make_shared<Bala3D>(Bala3D(pos + direccion * 0.5f, auxDir, window, cam, bala, log10(numGemas) + 1));
	}
	es->add(bs);
}

//dispara una bala
void Personaje3D::lanzarBala() {
	glm::vec3 auxDir = { distribution(gen), distribution(gen), distribution(gen) };
	auxDir = direccion + auxDir * 0.05f;
	auxDir = auxDir / glm::length(auxDir);
	es->add(make_shared<Bala3D>(Bala3D(pos + direccion * 0.5f , auxDir, window, cam, bala, log10(numGemas) + 1)));
}

void Personaje3D::setWindow(GLFWwindow* window) {
	this->window = window;
}

bool Personaje3D::getPulsado() {
	return disparando;
}

void Personaje3D::controlesInFrame() {
	escopetaGema = false;

	// DIRECCION
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	int px, py;
	glfwGetWindowSize(window, &px, &py);

	//calula la posicion relativa de la raton en la pantalla
	/*x = (x / px - 0.5f) * 2;
	y = (abs(y - py) / py - 0.5f) * 2;
	y *= py * 1.0f / px;*/

	angHoriz += (x - lastXpos) * sensibilidad * 0.01;
	angVert += (lastYpos - y) * sensibilidad * 0.01;

	glm::vec3 dirAux = { cos(glm::radians(angHoriz)) * cos(glm::radians(angVert)),
		sin(glm::radians(angVert)),
		sin(glm::radians(angHoriz)) * cos(glm::radians(angVert)) };
	direccion = glm::normalize(dirAux);
	lastXpos = x;
	lastYpos = y;

	int state = glfwGetKey(window, up);
	glm::vec3 dirEntr = { 0,0,0 };
	if (state == GLFW_PRESS) {
		pos += direccion * velocidad * 0.01f;
	}
	state = glfwGetKey(window, left);
	if (state == GLFW_PRESS) {
		pos += glm::normalize(glm::cross(direccion,
			{ 0,1,0 })) * -velocidad * 0.01f;
	}
	state = glfwGetKey(window, down);
	if (state == GLFW_PRESS) {
		pos += direccion * -velocidad * 0.01f;
	}
	state = glfwGetKey(window, right);
	if (state == GLFW_PRESS) {
		pos += glm::normalize(glm::cross(direccion,
			{ 0,1,0 })) * velocidad * 0.01f;
	}
	/*state = glfwGetKey(window, GLFW_KEY_Q);
	if (state == GLFW_PRESS) {
		orientacion += pi / 60 / 2;
	}
	state = glfwGetKey(window, GLFW_KEY_E);
	if (state == GLFW_PRESS) {
		orientacion -= pi/60/2;
	}*/

	//CONTROL CAMARA

	pos.y = 1;
	Camara a;
	a.FoV = 60;
	a.View = glm::lookAt(
		pos, // Camera is at (4,3,3), in World Space
		pos + direccion, // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	a.Projection = glm::perspective(
		glm::radians(a.FoV), // El campo de visi�n vertical, en radi�n: la cantidad de "zoom". Piensa en el lente de la c�mara. Usualmente est� entre 90� (extra ancho) y 30� (zoom aumentado)
		px*1.0f / py,       // Proporci�n. Depende del tama�o de tu ventana 4/3 == 800/600 == 1280/960, Parece familiar?
		0.1f,              // Plano de corte cercano. Tan grande como sea posible o tendr�s problemas de precisi�n.
		100.0f             // Plano de corte lejano. Tan peque�o como se pueda.
	);
	camaras[0] = a;

	pos.y = 0.5f;
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
	}
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
	/*if ((pos[0] > es->getLimites() || pos[0] < -es->getLimites()) || (pos[1] > es->getLimites() || pos[1] < -es->getLimites())) {
		if (!modoDios) {
			morir();
		}
	}*/
}