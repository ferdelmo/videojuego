#include "Bala.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <cmath>
#include <ctime>
#include <string>

#include "LoadShader.h"
#include "Camara.h"

using namespace std;

//Genera una bala en la posicion inicio con orientacion o
Bala::Bala(GLfloat inicio[], float o, GLFWwindow* window, Camara * c)
	: Renderizable(window, "../DevilDaggers/videojuego/Codigo/bala.png", "../DevilDaggers/videojuego/Codigo/Shaders/gema.vert", "../DevilDaggers/videojuego/Codigo/Shaders/gema.frag", 0.01f, c){
	this->window = window;
	for (int i = 0; i < 3; i++) {
		pos[i] = inicio[i];
	}
	orientacion = o;
	this->inicio = clock();
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
	sigue = muerto || ((clock() - inicio) / CLOCKS_PER_SEC) < tiempoBala;
}