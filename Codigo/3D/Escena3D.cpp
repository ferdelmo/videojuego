#include "Escena3D.h"

#include "Personaje3D.h"
#include "Bala3D.h"

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

void Escena3D::add(vector<shared_ptr<Bala3D>> bs) {
	balas.insert(balas.end(), bs.begin(), bs.end());
}

void Escena3D::renderizar() {
	int i = 0;
	clock_t tieempo = clock();
	while (i < balas.size()) {
		bool seguir = balas[i]->renderizar();
		if (!seguir) {
			balas.erase(balas.begin() + i);
		}
		else {
			i++;
		}
	}
	//if (per->vivo || per->modoDios) {
		bool perso = per->renderizar();
	//}
}

void Escena3D::moverObjetos() {
	for (int i = 0; i < balas.size(); i++) {
		balas[i]->mover();
	}
	per->mover();
}