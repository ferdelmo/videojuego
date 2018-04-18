#include "CalaveraBase3D.h"

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

#include "Personaje3D.h"
#include "Bala3D.h"
#include "Gema3D.h"
#include "../LoadShader.h"
#include "../Camara.h"

using namespace std;

CalaveraBase3D::CalaveraBase3D(glm::vec3 pos, glm::vec3 dir, Escena3D * es, GLFWwindow* window, Camara * c, Obj3D obj, int nivel)
	: Render3D(window, "../DevilDaggers/videojuego/Codigo/Shaders/3D.vert", "../DevilDaggers/videojuego/Codigo/Shaders/3D.frag", c, obj, { 1,0,0 }, 0.5){
	this->pos = pos;
	this->es = es;
	GLfloat texCoords[8] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f

	};
	distribution = uniform_real_distribution<float>(-10, 10);
	distribution2 = uniform_real_distribution<float>(tam/2.0, es->per->tam*1.5);
	random_device rd;
	// Initialize Mersenne Twister pseudo-random number generator
	gen = mt19937(rd());

	dir[0] = distribution(gen);
	dir[1] = distribution(gen);
	dir[2] = abs(distribution(gen));
	direccion = dir;
	/*int nivelPer = log10(es->getPer()->numGemas);
	if (nivelPer + 1 <= 1) {
		vida = 25;
	}
	else if (nivelPer + 1 == 2) {
		vida = 50;
	}
	else {
		vida = 100;
	}*/
	this->nivel = nivel;
	if (nivel <= 1) {
		for (int i = 0; i < vertices.size(); i++) {
			colors[i] = {1,0,0};
		}
		vida = 25;
	}
	else if (nivel == 2) {
		for (int i = 0; i < vertices.size(); i++) {
			colors[i] = { 0,1,0 };
		}
		vida = 50;
	}
	else {
		for (int i = 0; i < vertices.size(); i++) {
			colors[i] = { 0,0,1 };
		}
		vida = 100;
	}
}

void CalaveraBase3D::seguir() {
	glm::vec3 posP = es->per->pos;
	if (vida <= 25) {
		if (double(clock() - tiempecito) / CLOCKS_PER_SEC >= tiempoSeguir || llegar) {
			dir = posP;
			tiempecito = clock();
			tiempoSeguir = abs(distribution(gen))*2.0;
			llegar = false;
		}
		glm::vec3 vecDir = dir - pos; // vector movimiento
		vecDir = vecDir / glm::length(vecDir); //normalizar vector
		pos += (0.005f * velocidad) * vecDir;
		if (glm::length(pos - dir) <= tam) {
			llegar = true;
		}
		direccion = vecDir;
	}
	else if (vida > 25 && vida <= 50) {
		glm::vec3 vecDir = posP - pos; // vector movimiento
		vecDir = vecDir / glm::length(vecDir); //normalizar vector
		pos += (0.005f * velocidad) * vecDir;
		direccion = vecDir;
	}
	else{
		if (llegar) {
			llegar = false;
			dir[0] = distribution(gen);
			dir[1] = distribution2(gen);
			dir[2] = distribution(gen);
		}
		glm::vec3 vecDir = dir - pos;
		vecDir = vecDir / glm::length(vecDir); //normalizar vector
		pos += (0.005f * velocidad) * vecDir;
		if (glm::length(pos - dir) <= tam) {
			llegar = true;
		}
		direccion = vecDir;
	}
	if (glm::length(pos - posP) <= tam + es->per->tam && !es->per->modoDios) {
		//cout << "MUERTO MATAO" << endl;
	}
}

bool CalaveraBase3D::vivo() {
	vector<shared_ptr<Bala3D>> * b = es->getBalas();
	int i = 0;
	while (i < b->size()) {
		if (glm::length(pos - b->at(i)->pos) < tam + b->at(i)->tam) {
			vida -= b->at(i)->danyo;
			b->erase(b->begin() + i);
		}
		else {
			i++;
		}
	}
	if (vida <= 0 && nivel > 1) {
		//mciSendString("play ../DevilDaggers/videojuego/Codigo/Musica/calaveraI.mp3", NULL, 0, NULL);
		Obj3D cubo;
		Escena3D es3D;
		Render3D::loadOBJ("../DevilDaggers/videojuego/Codigo/3D/cubo.obj", cubo.vertices, cubo.uvs, cubo.normals);
		shared_ptr<Gema3D> sg = make_shared<Gema3D>(Gema3D(pos, { 1,1,0 }, es, window, cam, cubo));
		sg->vida = 0;
		sg->tiempecito = clock();
		es->add(sg);
	}
	return vida>0;
}
CalaveraBase3D CalaveraBase3D::operator=(const CalaveraBase3D& b) {
	if (this != &b) { // self-assignment check expected
		this->velocidad = b.velocidad;
		this->velRot = b.velRot;
		this->vida = b.vida;
		this->muerto = b.muerto;
		for (int i = 0; i < 3; i++) {
			this->dir[i] = b.dir[i];
		}

		this->gen = b.gen;
		this->distribution = b.distribution;
		this->es = b.es;
	}
	return *this;
}

void CalaveraBase3D::mover() {
	seguir();
	sigue = vivo();
}

void CalaveraBase3D::fisicas() {
	sigue = vivo();
}
