#include "Bala.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <cmath>
#include <ctime>
#include <string>
#include <thread>

#include "LoadShader.h"
#include "Camara.h"

using namespace std;

//Genera una bala en la posicion inicio con orientacion o
Bala::Bala(GLfloat inicio[], float o, GLFWwindow* window, Camara * c, int nivel)
	: Renderizable(window, "../DevilDaggers/videojuego/Codigo/bala"+to_string(nivel)+".png", "../DevilDaggers/videojuego/Codigo/Shaders/bala.vert", "../DevilDaggers/videojuego/Codigo/Shaders/bala.frag", 0.015f, c){
	this->window = window;
	for (int i = 0; i < 3; i++) {
		pos[i] = inicio[i];
	}
	orientacion = o;
	this->inicio = clock();
	if (nivel <= 1) {
		danyo = 5;
	}
	else if (nivel == 2) {
		danyo = 10;
	}
	else {
		danyo = 20;
	}
}

Bala::Bala(GLfloat inicio[], float o, GLFWwindow* window, Camara * c, GLuint sha, int nivel)
	: Renderizable (window, "../DevilDaggers/videojuego/Codigo/bala" + to_string(nivel) + ".png", 0.015f, c, sha) {
	this->window = window;
	for (int i = 0; i < 3; i++) {
		pos[i] = inicio[i];
	}
	orientacion = o;
	this->inicio = clock();
	if (nivel <= 1) {
		danyo = 5;
	}
	else if (nivel == 2) {
		danyo = 10;
	}
	else {
		danyo = 20;
	}
}

// necesario para usar la clase vector, asignacion
Bala Bala::operator=(const Bala& b) {
	if (this != &b) { // self-assignment check expected
		for (int i = 0; i < 3; i++) {
			this->pos[i] = b.pos[i];
		}
		this->orientacion = b.orientacion;
		this->velocidad = b.velocidad;
		this->inicio = b.inicio;
		this->tiempoBala = b.tiempoBala;
	}
	return *this;
}

//Avanza la bala en la direccion de su orientacion a velocidad v
void Bala::avanzar() {
	pos[0] += (velocidad * 0.005)*cos(orientacion);
	pos[1] += (velocidad * 0.005)*sin(orientacion);
}

void Bala::morir() {
	muerto = true;
}

void Bala::mover() {
	avanzar();
	bool sale = (pos[0]>limite || pos[0]<-limite || pos[1]>limite || pos[1]<-limite);
	sigue = !sale && !muerto && ((clock() - inicio) / CLOCKS_PER_SEC) < tiempoBala;
}