#include "CalaveraBase.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>
#include <Windows.h>
#include <mmsystem.h>
#include <random>
#include <cmath>
#include <ctime>
#include <vector>
#include <stdlib.h>

#include "LoadShader.h"
#include "Personaje.h"
#include "Bala.h"
#include "Gema.h"
#include "Escena.h"
#include "Camara.h"

using namespace std;

CalaveraBase::CalaveraBase(GLfloat x, GLfloat y, GLfloat z, Escena * es, GLFWwindow* window, Camara * c) 
	: Renderizable(window, "../DevilDaggers/videojuego/Codigo/skull.png", "../DevilDaggers/videojuego/Codigo/Shaders/calavera.vert", "../DevilDaggers/videojuego/Codigo/Shaders/calavera.frag",0.05f,c) {
	pos[0] = x; pos[1] = y; pos[2] = z;
	this->es = es;
	GLfloat texCoords[8] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f

	};
	for (int i = 0; i < 8; i++) {
		this->texCoords[i] = texCoords[i];
	}
	//numeros aleatorios
	distribution = uniform_real_distribution<float>(-1, 1);
	random_device rd;
	// Initialize Mersenne Twister pseudo-random number generator
	gen = mt19937(rd());

	dir[0] = distribution(gen);
	dir[1] = distribution(gen);
}

CalaveraBase::CalaveraBase(GLfloat x, GLfloat y, GLfloat z, Escena * es, GLFWwindow* window, Camara * c, GLuint sha)
	: Renderizable(window, "../DevilDaggers/videojuego/Codigo/skull.png", 0.05f, c, sha) {
	pos[0] = x; pos[1] = y; pos[2] = z;
	this->es = es;
	GLfloat texCoords[8] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f

	};
	for (int i = 0; i < 8; i++) {
		this->texCoords[i] = texCoords[i];
	}
	//numeros aleatorios
	distribution = uniform_real_distribution<float>(-1, 1);
	random_device rd;
	// Initialize Mersenne Twister pseudo-random number generator
	gen = mt19937(rd());

	dir[0] = distribution(gen);
	dir[1] = distribution(gen);
}

void CalaveraBase::seguir() {
	/*shared_ptr<Personaje> a= es->getPer();
	GLfloat posP[] = { 0,0,0 };
	a->getPosition(posP);
	GLfloat dirx = posP[0] - pos[0], diry = posP[1] - pos[1];
	GLfloat antigua = orientacion;
	orientacion = atan2(diry, dirx);
	float alpha = distribution(gen);
	//orientacion += alpha * pi / 6;
	if (orientacion > antigua) {
		orientacion = antigua + velRot;
	}
	else {
		orientacion = antigua - velRot;
	}
	pos[0] += (0.005 * velocidad)*cos(orientacion);
	pos[1] += (0.005 * velocidad)*sin(orientacion);
	if (distancia(pos[0], pos[1], posP[0], posP[1]) <= tam * tam + a->tam * a->tam) {
		a->morir();
	}*/
	shared_ptr<Personaje> a = es->getPer();
	GLfloat posP[] = { 0,0,0 };
	a->getPosition(posP);
	//distribution = uniform_real_distribution<float>(-es->getLimites(), es->getLimites()); // se puede usar otras distribuciones
	//random_device rd;
	// Initialize Mersenne Twister pseudo-random number generator
	//gen = mt19937(rd());
	if (double(clock() - tiempecito) / CLOCKS_PER_SEC >= tiempoSeguir || llegar) {
		dir[0] = posP[0];
		dir[1] = posP[1];
		tiempecito = clock();
		tiempoSeguir = abs(distribution(gen))*2.0;
		llegar = false;
	}
	GLfloat dirx = dir[0] - pos[0], diry = dir[1] - pos[1];
	orientacion = atan2(diry, dirx);
	float alpha = distribution(gen);
	//orientacion += alpha * pi / 6;
	GLfloat moduloDir = sqrt(dirx*dirx + diry * diry);
	GLfloat dirxNorm = dirx / moduloDir;
	GLfloat diryNorm = diry / moduloDir;
	pos[0] += (0.005f * velocidad)*dirxNorm;
	pos[1] += (0.005f * velocidad)*diryNorm;
	if (abs(pos[0] - dir[0]) <= tam && abs(pos[1] - dir[1]) <= tam) {
		llegar = true;
	}
	if (distancia(pos[0], pos[1], posP[0], posP[1]) <= tam * tam + a->tam * a->tam && !a->modoDios) {
		a->morir();
	}
}
GLfloat CalaveraBase::distancia(GLfloat x, GLfloat y, GLfloat xp, GLfloat yp) {
	return (x - xp)*(x - xp) + (y - yp)*(y - yp);
}
bool CalaveraBase::vivo() {
	vector<shared_ptr<Bala>> * b = es->getBalas();
	int i = 0;
	while (i < b->size()) {
		if (distancia(pos[0], pos[1], b->at(i)->pos[0], b->at(i)->pos[1]) <= tam * tam + b->at(i)->tam * b->at(i)->tam) {
			vida -= b->at(i)->danyo;
			b->erase(b->begin() + i);
		}
		else {
			i++;
		}
	}
	if (vida <= 0 && es->getPer()->opciones->sonido == 1) {
		mciSendString("play ../DevilDaggers/videojuego/Codigo/Musica/calaveraI.mp3", NULL, 0, NULL);
	}
	return vida>0;
}
CalaveraBase CalaveraBase::operator=(const CalaveraBase& b) {
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

void CalaveraBase::mover() {
	seguir();
}


void CalaveraBase::fisicas() {
	//colision con balas
	sigue = vivo();
}
