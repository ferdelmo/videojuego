#include "CalaveraBaseII.h"

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
#include "Personaje.h"
#include "Bala.h"
#include "Gema.h"
#include "Escena.h"
#include "Camara.h"

using namespace std;

CalaveraBaseII::CalaveraBaseII(GLfloat x, GLfloat y, GLfloat z, Escena * es, GLFWwindow* window, Camara * c)
	: Renderizable(window, "../DevilDaggers/videojuego/Codigo/skull_II.png", "../DevilDaggers/videojuego/Codigo/Shaders/calaveraII.vert", "../DevilDaggers/videojuego/Codigo/Shaders/calaveraII.frag", 0.07f, c) {
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

void CalaveraBaseII::seguir() {
	shared_ptr<Personaje> a= es->getPer();
	GLfloat posP[] = { 0,0,0 };
	a->getPosition(posP);
	GLfloat dirx = posP[0] - pos[0], diry = posP[1] - pos[1];
	GLfloat antigua = orientacion;
	orientacion = atan2(diry, dirx);
	float alpha = distribution(gen);
	//orientacion += alpha * pi / 6;
	GLfloat moduloDir = sqrt(dirx*dirx + diry * diry);
	GLfloat dirxNorm = dirx / moduloDir;
	GLfloat diryNorm = diry / moduloDir;
	pos[0] += (0.005f * velocidad)*dirxNorm;
	pos[1] += (0.005f * velocidad)*diryNorm;
	if (distancia(pos[0], pos[1], posP[0], posP[1]) <= tam * tam + a->tam * a->tam) {
		a->morir();
	}
}

bool CalaveraBaseII::vivo() {
	vector<shared_ptr<Bala>> * b = es->getBalas();
	int i = 0;
	while (i < b->size()) {	
		//cout << distancia(pos[0], pos[1], b[i].pos[0], b[i].pos[1]) << endl;
		if (distancia(pos[0], pos[1], b->at(i)->pos[0], b->at(i)->pos[1]) <= 3 * tam * tam) {
			vida -= b->at(i)->danyo;
			b->erase(b->begin() + i);
			cout << vida << endl;
			//cout << "ELIMINADO " << b->size() << endl;
		}
		else {
			i++;
		}
	}
	if (vida <= 0) {
		cout << "GENERA GEMA" << endl;
		shared_ptr<Gema> sg = make_shared<Gema>(Gema(pos[0], pos[1], pos[2], es, window,cam));
		sg->vida = 0;
		sg->tiempecito = clock();
		es->add(sg);
	}
	return vida>0;
}
CalaveraBaseII CalaveraBaseII::operator=(const CalaveraBaseII& b) {
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

void CalaveraBaseII::mover() {
	seguir();
	sigue = vivo();
}