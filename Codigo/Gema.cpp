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
}

bool Gema::colisionBala() {
	vector<shared_ptr<Bala>> * b = es->getBalas();
	int i = 0;
	while (i < b->size()) {
		//cout << distancia(pos[0], pos[1], b[i].pos[0], b[i].pos[1]) << endl;
		if (distancia(pos[0], pos[1], b->at(i)->pos[0], b->at(i)->pos[1]) <= 3 * tam * tam) {
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

int Gema::getVida() {
	return vida;
}


void Gema::seguirPersonaje() {
	shared_ptr<Personaje> a = es->getPer();
	GLfloat posP[] = { 0,0,0 };
	a->getPosition(posP);
	GLfloat dirx = posP[0] - pos[0], diry = posP[1] - pos[1];
	GLfloat antigua = orientacion;
	orientacion = atan2(diry, dirx);
	
	if (!a->getPulsado()) {
		//cout << "A SEGUIIIIIR" << endl;
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
	}
	else {
		//cout << "NO SEGUIIIIIR" << endl;
	}
	
	//cout << "tiempo = " << tiempecito << endl;
	//cout << "DESPEGADA DE LA TORRE" << endl;
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
		seguirPersonaje();
	}
	if (double(clock() - tiempecito) / CLOCKS_PER_SEC >= 10.0 && vida <= 0) {
		sigue = false;
	}
	else {
		sigue = vida>0 || noCogida;
	}
}