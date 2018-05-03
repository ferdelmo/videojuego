#include "Gema3D.h"

#include <iostream>
#include <random>
#include <cmath>
#include <ctime>
#include <vector>
#include <stdlib.h>

#include "Personaje3D.h"
#include "Bala3D.h"
#include "Escena3D.h"
//#include "Gema3D.h"
#include "Sonidos.h"
#include "../LoadShader.h"
#include "../Camara.h"

using namespace std;

//CONSTRUCTOR EN UNA POSICION ESPECIFICA
Gema3D::Gema3D(glm::vec3 posicion, glm::vec3 dir, Escena3D * es, GLFWwindow* window, Camara * c, Obj3D obj)
	: Render3D(window, "../DevilDaggers/videojuego/Codigo/Shaders/3D.vert", "../DevilDaggers/videojuego/Codigo/Shaders/3D.frag", c, obj, { 1,0,0 }, 0.1){
	this->pos = posicion;
	this->es = es;
	//numeros aleatorios
	distribution = uniform_real_distribution<float>(0, 1);
	random_device rd;
	// Initialize Mersenne Twister pseudo-random number generator
	gen = mt19937(rd());
	this->direccion = dir;
	int nivelPer = log10(es->per->numGemas);
	if (nivelPer + 1 <= 1) {
		vida = 25;
	}
	else if (nivelPer + 1 == 2) {
		vida = 50;
	}
	else {
		vida = 100;
	}
}

bool Gema3D::colisionBala() {
	vector<shared_ptr<Bala3D>> * b = es->getBalas();
	int i = 0;
	while (i < b->size()) {
		//cout << distancia(pos[0], pos[1], b[i].pos[0], b[i].pos[1]) << endl;
		if (glm::length(pos - b->at(i)->pos) < 1.25*tam + b->at(i)->tam) {
			vida -= b->at(i)->danyo;
			b->erase(b->begin() + i);
			cout << "colision " << endl;
		}
		else {
			i++;
		}
	}
	if (vida <= 0) {
		tiempecito = clock();
	}
	return vida > 0;
}

void Gema3D::seguirPersonaje() {
	shared_ptr<Personaje3D> a = es->per;
	this->direccion = direccion;
	if (!a->getPulsado()) {
		glm::vec3 posP = es->per->pos;
		glm::vec3 vecDir = posP - pos; // vector movimiento
		vecDir = vecDir / glm::length(vecDir); //normalizar vector
		pos += (0.01f * velocidad) * vecDir;
	}
}

void Gema3D::setPos(glm::vec3 posicion) {
	this->pos = posicion;
}

void Gema3D::mover() {
	shared_ptr<Personaje3D> a = es->per;
	glm::vec3 posP = es->per->pos;

	if (glm::length(pos - a->pos) <= tam + a->tam && vida <= 0) {
		a->addGema();
		cout << "El personaje tiene: " << a->numGemas << " gemas." << endl;
		/*thread t(&Sonidos::play, &Sonido, buffer, pos, "../DevilDaggers/videojuego/Codigo/Musica/gema3D.wav");
		t.detach();*/
		sigue = false;
		noCogida = false;
	}
	if (vida <= 0) {
		if (tam > 0.1) {
			tam -= 0.005f;
		}
		if (pos.y > 0.5f) {
			pos += glm::vec3({ 0,-velocidad/5,0 })*0.01f;
		}
		//cout << "seguir al personaje " << endl;
		seguirPersonaje();
	}
	if (double(clock() - tiempecito) / CLOCKS_PER_SEC >= 10.0 && vida <= 0) {
		sigue = false;
	}
	else {
		sigue = vida>0 || noCogida;
	}
}