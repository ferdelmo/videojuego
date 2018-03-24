#include "Partida.h"
#include "Daga.h" 

#include "Escena.h"
#include "Personaje.h"
#include "Daga.h"
#include "Camara.h"
#include "LoadShader.h"

using namespace std;

Partida::Partida(Escena * es) {
	this->es = es;
	//numeros aleatorios
	distribution = uniform_int_distribution<>(0, numPosis -1);
	random_device rd;
	// Initialize Mersenne Twister pseudo-random number generator
	gen = mt19937(rd());
	shader = LoadShaders("../DevilDaggers/videojuego/Codigo/Shaders/daga.vert", "../DevilDaggers/videojuego/Codigo/Shaders/daga.frag");
}

void Partida::start() {
	daga1 = clock();
	daga2 = clock();
	daga3 = clock();
}

void Partida::stop() {

}

void Partida::actualizar() {
	generarDaga1();
	generarDaga2();
	generarDaga3();
}

void Partida::generarDaga1() {
	if ((clock() - daga1) / (CLOCKS_PER_SEC / 1000)>=(daga1sec*1000)) {
		int p = distribution(gen);
		es->add(make_shared<Daga>(Daga(posiposibles[p], es, 1, es->getPer()->window, es->getPer()->cam, shader)));
		daga1sec = 10;
		daga1 = clock();
	}
}

void Partida::generarDaga2() {
	if ((clock() - daga2) / (CLOCKS_PER_SEC / 1000)>= (daga2sec * 1000)) {
		int p = distribution(gen);
		es->add(make_shared<Daga>(Daga(posiposibles[p], es, 2, es->getPer()->window, es->getPer()->cam, shader)));
		daga2sec = 10;
		daga2 = clock();
	}
}

void Partida::generarDaga3() {
	if ((clock() - daga3) / (CLOCKS_PER_SEC / 1000)>= (daga3sec * 1000)) {
		int p = distribution(gen);
		es->add(make_shared<Daga>(Daga(posiposibles[p], es, 3, es->getPer()->window, es->getPer()->cam, shader)));
		daga3sec = 10;
		daga3 = clock();
	}
}
