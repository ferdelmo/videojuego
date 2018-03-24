#include "Escena.h"

#include "Personaje.h"
#include "Bala.h"
#include "Daga.h"
#include "DagaII.h"
#include "DagaIII.h"
#include "CalaveraBase.h"
#include "CalaveraBaseII.h"
#include "Gema.h"
#include "CalaveraBaseIII.h"
#include "Fondo.h"

using namespace std;

Escena::Escena() {
	balas = vector<shared_ptr<Bala>>();
	gemas = vector<shared_ptr<Gema>>();
	dagas = vector<shared_ptr<Daga>>();
	dagasII = vector<shared_ptr<DagaII>>();
	dagasIII = vector<shared_ptr<DagaIII>>();
	calaveras = vector<shared_ptr<CalaveraBase>>();
	calaverasII = vector<shared_ptr<CalaveraBaseII>>();
	calaverasIII = vector<shared_ptr<CalaveraBaseIII>>();
}

void Escena::add(shared_ptr<Bala> b) {
	balas.push_back(b);
}

void Escena::add(vector<shared_ptr<Bala>> bs) {
	balas.insert(balas.end(),bs.begin(),bs.end());
}

void Escena::add(shared_ptr<CalaveraBase> cb) {
	calaveras.push_back(cb);
}

void Escena::add(shared_ptr<CalaveraBaseII> cb) {
	calaverasII.push_back(cb);
}

void Escena::add(shared_ptr<CalaveraBaseIII> cb) {
	calaverasIII.push_back(cb);
}

void Escena::add(shared_ptr<Daga> d) {
	dagas.push_back(d);
}

void Escena::add(shared_ptr<DagaII> d) {
	dagasII.push_back(d);
}

void Escena::add(shared_ptr<DagaIII> d) {
	dagasIII.push_back(d);
}

void Escena::add(shared_ptr<Personaje> p) {
	per = p;
}

void Escena::add(shared_ptr<Gema> g) {
	gemas.push_back(g);
}

void Escena::setFondo(shared_ptr<Fondo> f) {
	fon = f;
}

GLfloat Escena::getLimites() {
	return fon->tam;
}

vector<shared_ptr<Bala>> * Escena::getBalas() {
	return &balas;
}

vector<shared_ptr<CalaveraBase>> * Escena::getCalaveras() {
	return &calaveras;
}

vector<shared_ptr<CalaveraBaseII>> * Escena::getCalaverasII() {
	return &calaverasII;
}

vector<shared_ptr<CalaveraBaseIII>> * Escena::getCalaverasIII() {
	return &calaverasIII;
}

vector<shared_ptr<Daga>> * Escena::getDagas() {
	return &dagas;
}

vector<shared_ptr<DagaII>> * Escena::getDagasII() {
	return &dagasII;
}

vector<shared_ptr<DagaIII>> * Escena::getDagasIII() {
	return &dagasIII;
}
vector<shared_ptr<Gema>> * Escena::getGemas() {
	return &gemas;
}

shared_ptr<Personaje> Escena::getPer() {
	return per;
}

void Escena::renderizar() {
	int i = 0;
	while (i<balas.size()) {
		bool seguir = balas[i]->renderizar();
		if (!seguir) {
			balas.erase(balas.begin() + i);
		}
		else {
			i++;
		}
	}
	i = 0;
	while (i<calaveras.size()) {
		bool seguir = calaveras[i]->renderizar();
		if (!seguir) {
			calaveras.erase(calaveras.begin() + i);
		}
		else {
			i++;
		}
	}
	i = 0;
	while (i<calaverasII.size()) {
		bool seguir = calaverasII[i]->renderizar();
		if (!seguir) {
			calaverasII.erase(calaverasII.begin() + i);
		}
		else {
			i++;
		}
	}
	i = 0;
	while (i<calaverasIII.size()) {
		bool seguir = calaverasIII[i]->renderizar();
		if (!seguir) {
			calaverasIII.erase(calaverasIII.begin() + i);
		}
		else {
			i++;
		}
	}
	i = 0;
	while (i<dagas.size()) {
		bool seguir = dagas[i]->renderizar();
		if (!seguir) {
			dagas.erase(dagas.begin() + i);
		}
		else {
			i++;
		}
	}
	i = 0;
	while (i<gemas.size()) {
		bool seguir = gemas[i]->renderizar();
		if (!seguir) {
			gemas.erase(gemas.begin() + i);
		}
		else {
			i++;
		}
		//	cout << "Hay :" << gemas.size() << " gemas " << endl;
	}
	if (per->vivo) {
		bool perso = per->renderizar();
	}
}