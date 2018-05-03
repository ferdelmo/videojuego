#include "Bala3D.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <cmath>
#include <ctime>
#include <string>
#include <thread>

#include "../LoadShader.h"
#include "../Camara.h"

using namespace std;

//Genera una bala en la posicion inicio con orientacion o
Bala3D::Bala3D(glm::vec3 pos, glm::vec3 dir, GLFWwindow* window, Camara * c, Obj3D obj, int nivel)
	: Render3D(window,"../DevilDaggers/videojuego/Codigo/Shaders/3D.vert", "../DevilDaggers/videojuego/Codigo/Shaders/3D.frag", c, obj, { 0.3f,0.1f,0.1f }, 0.1){
	this->window = window;
	this->pos = pos;
	direccion = dir;
	this->inicio = clock();
	if (nivel <= 1) {
		danyo = 5;
		cambiarColor({ 1,0,0 });
	}
	else if (nivel == 2) {
		danyo = 10;
		cambiarColor({0,1,1 });
	}
	else {
		danyo = 25;
		cambiarColor({ 1,0,1 });
	}
}

// necesario para usar la clase vector, asignacion
Bala3D Bala3D::operator=(const Bala3D& b) {
	if (this != &b) { // self-assignment check expected
		this->pos = b.pos;
		this->direccion = b.direccion;
		this->velocidad = b.velocidad;
		this->inicio = b.inicio;
		this->tiempoBala = b.tiempoBala;
	}
	return *this;
}

//Avanza la bala en la direccion de su orientacion a velocidad v
void Bala3D::avanzar() {
	pos = pos + velocidad * 0.01f*direccion;
}

void Bala3D::morir() {
	muerto = true;
}

void Bala3D::mover() {
	avanzar();
	bool sale = (pos[0]>limite || pos[0]<-limite || pos[1]>limite || pos[1]<-limite);
	sale = false;
	sigue = !sale && !muerto && ((clock() - inicio) / CLOCKS_PER_SEC) < tiempoBala;
}