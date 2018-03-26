#include "Gema.h"

#include <iostream>
#include <random>
#include <cmath>
#include <ctime>
#include <vector>
#include <stdlib.h>

#include "LoadShader.h"
#include "Personaje.h"
#include "Bala.h"
#include "Escena.h"
#include "Camara.h"

using namespace std;

//CONSTRUCTOR EN UNA POSICION ESPECIFICA
Gema::Gema(GLfloat x, GLfloat y, GLfloat z, Escena * es, GLFWwindow* window, Camara * c) 
	: Renderizable(window, "../DevilDaggers/videojuego/Codigo/Gema.png", "../DevilDaggers/videojuego/Codigo/Shaders/gema.vert", "../DevilDaggers/videojuego/Codigo/Shaders/gema.frag",0.025f, c) {
	pos[0] = x; pos[1] = y; pos[2] = z;
	this->es = es;
	//numeros aleatorios
	distribution = uniform_real_distribution<float>(0, 1);
	random_device rd;
	// Initialize Mersenne Twister pseudo-random number generator
	gen = mt19937(rd());

	int nivelPer = log10(es->getPer()->numGemas);
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

bool Gema::colisionBala() {
	vector<shared_ptr<Bala>> * b = es->getBalas();
	int i = 0;
	while (i < b->size()) {
		//cout << distancia(pos[0], pos[1], b[i].pos[0], b[i].pos[1]) << endl;
		if (distancia(pos[0], pos[1], b->at(i)->pos[0], b->at(i)->pos[1]) <=  tam * tam + b->at(i)->tam * b->at(i)->tam) {
			vida -= b->at(i)->danyo;
			b->erase(b->begin() + i);
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

void Gema::seguirPersonaje() {
	shared_ptr<Personaje> a = es->getPer();
	orientacion += velRot;
	if (!a->getPulsado()) {
		GLfloat posP[] = { 0,0,0 };
		a->getPosition(posP);
		GLfloat dirx = posP[0] - pos[0], diry = posP[1] - pos[1];
		GLfloat moduloDir = sqrt(dirx*dirx + diry * diry);
		GLfloat dirxNorm = dirx / moduloDir;
		GLfloat diryNorm = diry / moduloDir;
		pos[0] += (0.005f * velocidad)*dirxNorm;
		pos[1] += (0.005f * velocidad)*diryNorm;
	}
}

void Gema::setPos(GLfloat nuevaX, GLfloat nuevaY, GLfloat nuevaZ) {
	pos[0] = nuevaX;
	pos[1] = nuevaY;
	pos[2] = nuevaZ;
}

void Gema::mover() {
	shared_ptr<Personaje> a = es->getPer();
	GLfloat posP[] = { 0,0,0 };
	a->getPosition(posP);

	if (distancia(pos[0], pos[1], posP[0], posP[1]) <= a->tam*a->tam && vida <= 0) {
		a->addGema();
		cout << "El personaje tiene: " << a->numGemas << " gemas." << endl;
		sigue= false;
		noCogida = false;
	}
	if (vida <= 0) {
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