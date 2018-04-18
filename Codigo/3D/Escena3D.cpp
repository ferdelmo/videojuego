#include "Escena3D.h"

#include "Personaje3D.h"
#include "CalaveraBase3D.h"
#include "Bala3D.h"
#include "Gema3D.h"
#include "Daga3D.h"

using namespace std;

Escena3D::Escena3D() {
	balas = vector<shared_ptr<Bala3D>>();
}

void Escena3D::add(shared_ptr<Personaje3D> p) {
	per = p;
}

void Escena3D::add(shared_ptr<Bala3D> b) {
	balas.push_back(b);
}

void Escena3D::add(shared_ptr<CalaveraBase3D> c) {
	calaveras.push_back(c);
}

void Escena3D::add(shared_ptr<Gema3D> g) {
	gemas.push_back(g);
}

void Escena3D::add(shared_ptr<Daga3D> d) {
	dagas.push_back(d);

}
void Escena3D::add(vector<shared_ptr<Bala3D>> bs) {
	balas.insert(balas.end(), bs.begin(), bs.end());
}

vector<shared_ptr<Bala3D>>* Escena3D::getBalas() {
	return &balas;
}

vector<shared_ptr<Gema3D>>* Escena3D::getGemas() {
	return &gemas;
}

vector<shared_ptr<Daga3D>>* Escena3D::getDagas() {
	return &dagas;

}
void Escena3D::renderizar() {
	int i = 0;
	clock_t tieempo = clock();
	glm::vec3 light = { 0,4,0 };
	light = light + per->pos;
	while (i < balas.size()) {
		balas[i]->lightPos = light;
		bool seguir = balas[i]->renderizar();
		if (!seguir) {
			balas.erase(balas.begin() + i);
		}
		else {
			i++;
		}
	}
	i = 0;
	while (i < calaveras.size()) {
		calaveras[i]->lightPos = light;
		bool seguir = calaveras[i]->renderizar();
		if (!seguir) {
			calaveras.erase(calaveras.begin() + i);
		}
		else {
			i++;
		}
	}
	i = 0;
	while (i < gemas.size()) {
		gemas[i]->lightPos = light;
		bool seguir = gemas[i]->renderizar();
		if (!seguir) {
			gemas.erase(gemas.begin() + i);
		}
		else {
			i++;
		}
	}
	i = 0;
	while (i < dagas.size()) {
		dagas[i]->lightPos = light;
		bool seguir = dagas[i]->renderizar();
		if (!seguir) {
			dagas.erase(dagas.begin() + i);
		}
		else {
			i++;
		}
	}
	//if (per->vivo || per->modoDios) {
	per->lightPos = light;
		bool perso = per->renderizar();
	//}
}

void Escena3D::moverObjetos() {
	for (int i = 0; i < balas.size(); i++) {
		balas[i]->mover();
	}
	for (int i = 0; i < calaveras.size(); i++) {
		calaveras[i]->mover();
	}
	for (int i = 0; i < gemas.size(); i++) {
		gemas[i]->mover();
	}
	for (int i = 0; i < dagas.size(); i++) {
		dagas[i]->mover();
	}
	per->mover();
}