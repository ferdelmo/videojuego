#include "Escena3D.h"

#include "Personaje3D.h"
#include "CalaveraBase3D.h"
#include "Bala3D.h"
#include "Gema3D.h"
#include "Daga3D.h"
#include "Sonidos.h"
#include "Network.hpp"

#pragma warning(disable : 4996)

using namespace std;

Escena3D::Escena3D() {
	balas = std::vector<shared_ptr<Bala3D>>();
	
}

void Escena3D::entrenarRed() {
	std::vector<int> layers;
	layers.push_back(1); //capas de entrada
	layers.push_back(3); //capas intermedias
	layers.push_back(1); //capas de salida

	redCalavs = new Network(layers, 0.001);
	cout << *redCalavs << endl;
	matrix<double> X(360, 1), Y(360, 1);
	for (int i = -180; i < 180; i++) {
		X(i + 180, 0) = i * 1.0 / 180.0;
		Y(i + 180, 0) = i * 1.0 / 2 / 180.0;
	}
	redCalavs->fit(X, Y, 200);
	for (int i = -180; i <= 180; i = i + 10) {
		boost::numeric::ublas::vector<double> aux(1);
		aux[0] = i/180.0;
		cout << "ANG: " << aux[0] * 180.0 << " PREDICT: " << (*(redCalavs->predict(aux)))[0]*180.0 << endl;	}

}

void Escena3D::add(shared_ptr<Personaje3D> p) {
	per = p;
	fondo = make_shared<Render3D>(Render3D(p->window,p->cam,"../DevilDaggers/videojuego/Codigo/suelo.png",limite*1,limite));
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
void Escena3D::add(std::vector<shared_ptr<Bala3D>> bs) {
	balas.insert(balas.end(), bs.begin(), bs.end());
}

std::vector<shared_ptr<Bala3D>>* Escena3D::getBalas() {
	return &balas;
}

std::vector<shared_ptr<Gema3D>>* Escena3D::getGemas() {
	return &gemas;
}

std::vector<shared_ptr<Daga3D>>* Escena3D::getDagas() {
	return &dagas;

}
void Escena3D::renderizar() {
	int i = 0;
	clock_t tieempo = clock();
	glm::vec3 light = { 0,4,0 };
	light = light + per->pos;
	fondo->lightPos = light;
	fondo->renderizar();
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
			/*thread t(&Sonidos::play, &Sonido, buffer, calaveras[i]->pos, "../DevilDaggers/videojuego/Codigo/Musica/muerteCalavera.wav");
			t.detach();*/
			calaveras.erase(calaveras.begin() + i);
			calavsMatadas++;
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
	if (!espectador) {
		bool perso = per->renderizar();
	}
	//}
}

void Escena3D::moverObjetos() {
	if (!espectador) {
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
			dagas[i]->fisicas();
		}
	}
	if (per->vivo || per->modoDios) {
		per->mover();
	}
	
}

void Escena3D::reset() {
	calaveras.clear();
	dagas.clear();
	gemas.clear();
	per.reset();
	balas.clear();
	//fon.reset();
	fin = false;
	calavsMatadas = 0;
}