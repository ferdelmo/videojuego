#include "Partida3D.h"
#include "Daga3D.h" 

#include "Escena3D.h"
#include "Personaje3D.h"
#include "Daga3D.h"
#include "CalaveraBase3D.h"
#include "../Camara.h"
#include "../LoadShader.h"

using namespace std;

Partida3D::Partida3D(Escena3D * es) {
	this->es = es;
	//numeros aleatorios
	distribution = uniform_int_distribution<>(0, numPosis -1);
	random_device rd;
	// Initialize Mersenne Twister pseudo-random number generator
	gen = mt19937(rd());
	modelo = new Obj3D;
	Render3D::loadOBJ("../DevilDaggers/videojuego/Codigo/3D/daga.obj", modelo->vertices, modelo->uvs, modelo->normals);
	cout << "MEGA LOL " << modelo->vertices.size() << endl;
}

void Partida3D::start() {
	daga1 = clock();
	daga2 = clock();
	daga3 = clock();
}

void Partida3D::stop() {

}

void Partida3D::actualizar() {
	if (true) {
		generarDaga1();
	}
	generarDaga2();
	generarDaga3();
}

void Partida3D::generarDaga1() {
	if (es->per->vivo || es->per->modoDios) {
		if ((clock() - daga1) / (CLOCKS_PER_SEC / 1000) >= (daga1sec * 1000)) {
			int p = distribution(gen);
			cout << "MEGA LOL " << modelo->vertices.size() << endl;
			es->add(make_shared<Daga3D>(Daga3D(posiposibles[p], { 0,0,0 }, es, es->per->window, es->per->cam, *modelo, 1)));
			if (daga1p) {
				daga1p = false;
				daga1sec = 20;
			}
			else if (daga1sec > 20) {
				daga1sec = daga1sec - 0.15;
			}
			cout << "Genera cada: " << daga1sec << endl;;
			daga1 = clock();
			una = false;
		}
	}
}

void Partida3D::generarDaga2() {
	if (es->per->vivo || es->per->modoDios) {
		if ((clock() - daga2) / (CLOCKS_PER_SEC / 1000) >= (daga2sec * 1000)) {
			int p = distribution(gen);
			es->add(make_shared<Daga3D>(Daga3D(posiposibles[p], { 0,0,0 }, es, es->per->window, es->per->cam, *modelo, 2)));
			if (daga2p) {
				daga2p = false;
				daga2sec = 30;
			}
			else if (daga2sec > 30) {
				daga2sec = daga2sec - 0.2;
			}
			cout << "Genera cada: " << daga2sec << endl;
			daga2 = clock();
		}
	}
}

void Partida3D::generarDaga3() {
	if (es->per->vivo || es->per->modoDios) {
		if ((clock() - daga3) / (CLOCKS_PER_SEC / 1000) >= (daga3sec * 1000)) {
			int p = distribution(gen);
			es->add(make_shared<Daga3D>(Daga3D(posiposibles[p], { 0,0,0 }, es, es->per->window, es->per->cam, *modelo, 3)));
			if (daga3p) {
				daga3p = false;
				daga3sec = 40;
			}
			else if (daga3sec > 40) {
				daga3sec = daga3sec - 0.25;
			}
			cout << "Genera cada: " << daga3sec << endl;;
			daga3 = clock();
		}
	}
}
