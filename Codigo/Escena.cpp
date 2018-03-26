#include "Escena.h"

#include "Personaje.h"
#include "Bala.h"
#include "Daga.h"
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

vector<shared_ptr<Gema>> * Escena::getGemas() {
	return &gemas;
}

shared_ptr<Personaje> Escena::getPer() {
	return per;
}

void Escena::moverObjetos() {
	clock_t tieempo = clock();
	if (per->vivo) {
		int i = 0;
		while (i<calaveras.size()) {
			//cout << "renderiza calavera " << i << endl;
			calaveras[i]->mover();
			i++;
		}
		i = 0;
		while (i<calaverasII.size()) {
			calaverasII[i]->mover();
			i++;
		}
		i = 0;
		while (i<calaverasIII.size()) {

			calaverasIII[i]->mover();

			i++;
		}
		i = 0;
		while (i<dagas.size()) {

			dagas[i]->mover();
			i++;
		}
		i = 0;
		while (i<gemas.size()) {
			gemas[i]->mover();
			i++;
		}
		per->mover();
	}
	int i = 0;
	while (i<balas.size()) {
		balas[i]->mover();
		i++;
	}
	bool fin = false;
	iMover++;
	mMover += (clock() - tieempo);
	mMover = mMover / iMover;
	if ((clock() - tieempo) > tMover) {
		tMover = clock() - tieempo;
		cout << "MOVER: max = " << tMover << " media = " << mMover << endl;
	}
	tieempo = clock();
}

void Escena::renderizar() {
	fon->renderizar();
	int i = 0;
	clock_t tieempo = clock();
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
		//cout << "renderiza calavera " << i << endl;
		bool seguir = calaveras[i]->renderizar();
		if (!seguir) {
			calavsMatadas++;
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
			calavsMatadas++;
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
			calavsMatadas++;
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
	}
	if (per->vivo) {
		bool perso = per->renderizar();
	}
	bool fin = false;
	iRender++;
	mRender += (clock() - tieempo);
	mRender = mRender / iRender;
	if ((clock() - tieempo) > tRender) {
		tRender = clock() - tieempo;
		cout << "RENDERIZAR: max = " << tRender << " media = " << mRender << endl;
	}
	tieempo = clock();
}

void Escena::actualizarFisicas() {
	//while (!fin) {
	clock_t tieempo = clock();
	if (per->vivo) {
		per->fisicas();
		for (int i = 0; i < dagas.size(); i++) {
			//cout << "fisicas daga " << i << endl;
			dagas[i]->fisicas();
		}
		for (int i = 0; i < calaverasIII.size(); i++) {
			calaverasIII[i]->fisicas();
		}
		for (int i = 0; i < calaverasII.size(); i++) {
			calaverasII[i]->fisicas();
		}
		for (int i = 0; i < calaveras.size(); i++) {
			//cout << "fisicas calavera " << i << endl;
			calaveras[i]->fisicas();
		}
		//}
		per->fisicas();
		bool fin = false;
		iFisica++;
		mFisica += (clock() - tieempo);
		mFisica = mFisica / iFisica;
	}
	
	if ((clock() - tieempo) > tFisica) {
		tFisica = clock() - tieempo;
		cout << "FISICAS: max = " << tFisica << " media = " << mFisica << endl;
	}
	tieempo = clock();
}

void Escena::pararFisicas() {
	fin = true;
}