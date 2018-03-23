#include "Escena.h"

#include "Personaje.h"
#include "Bala.h"
#include "Daga.h"
#include "CalaveraBase.h"
#include "CalaveraBaseII.h"
#include "Gema.h"

using namespace std;

Escena::Escena() {
	balas = vector<shared_ptr<Bala>>();
	dagas = vector<shared_ptr<Daga>>();
	calaveras = vector<shared_ptr<CalaveraBase>>();
}

void Escena::add(shared_ptr<Bala> b) {
	balas.push_back(b);
}

void Escena::add(shared_ptr<CalaveraBase> cb) {
	calaveras.push_back(cb);
}

void Escena::add(shared_ptr<CalaveraBaseII> cb) {
	calaverasII.push_back(cb);
}
void Escena::add(shared_ptr<Daga> d) {
	dagas.push_back(d);
}

void Escena::add(shared_ptr<Personaje> p) {
	per = p;
}

void Escena::add(shared_ptr<Gema> g) {
	gemas.push_back(g);
}

void Escena::anyadirGema(GLfloat x, GLfloat y, GLfloat z) {
	shared_ptr<Personaje> a = getPer();
	shared_ptr<Gema> sg = make_shared<Gema>(Gema(x, y, z, this, a->window, 0));
	add(sg);

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


vector<shared_ptr<Daga>> * Escena::getDagas() {
	return &dagas;
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
			GLfloat x, y, z;
			x = calaverasII.at(i)->pos[0];
			y = calaverasII.at(i)->pos[1];
			z = calaverasII.at(i)->pos[2];
			calaverasII.erase(calaverasII.begin() + i);
			anyadirGema(x, y, z);
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
	bool perso = per->renderizar();
}