#include "CalaveraBase3D.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>

#include <random>
#include <cmath>
#include <ctime>
#include <vector>
#include <stdlib.h>

#include "Personaje3D.h"
#include "Bala3D.h"
#include "Gema3D.h"
#include "../LoadShader.h"
#include "../Camara.h"
#include "Particulas.h"
#include "Sonidos.h"

using namespace std;

CalaveraBase3D::CalaveraBase3D(glm::vec3 pos, glm::vec3 dir, Escena3D * es, GLFWwindow* window, Camara * c, Obj3D obj, int nivel)
	: Render3D(window, "../DevilDaggers/videojuego/Codigo/Shaders/3D.vert", "../DevilDaggers/videojuego/Codigo/Shaders/3D.frag", c, obj, { 1,0,0 }, 0.15){
	this->pos = pos;
	this->es = es;
	GLfloat texCoords[8] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f

	};
	
	distribution = uniform_real_distribution<float>(-1, 1);
	distribution2 = uniform_real_distribution<float>(2,3);
	random_device rd;
	// Initialize Mersenne Twister pseudo-random number generator
	gen = mt19937(rd());
	GLfloat auxY = pos.y;
	/*posAux = es->per->pos;
	posAux.y = auxY + distribution(gen);*(¡/

	/*dir[0] = distribution(gen);
	dir[1] = distribution(gen);
	dir[2] = abs(distribution(gen));*/
	direccion = dir;
	/*int nivelPer = log10(es->getPer()->numGemas);
	if (nivelPer + 1 <= 1) {
		vida = 25;
	}
	else if (nivelPer + 1 == 2) {
		vida = 50;
	}
	else {
		vida = 100;
	}*/
	this->nivel = nivel;
	if (nivel <= 1) {
		cambiarColor({ 1,0,0 });
		vida = 25;
	}
	else if (nivel == 2) {
		cambiarColor({ 0,1,0 });
		vida = 50;
	}
	else {
		cambiarColor({ 0,0,1 });
		vida = 50;
	}

	velRot = 0.085+distribution(gen)*0.005;
	velocidad = velocidad + distribution(gen)*0.5;
	direccion = { distribution(gen),abs(distribution(gen)),distribution(gen) };
	//cout << "{ " << direccion.x << ", " << direccion.y << ", " << direccion.z << " }" << endl;
	direccion = direccion / length(direccion);
}

void CalaveraBase3D::seguir() {
	glm::vec3 posP = es->per->pos;
	//cout << "{ " << direccion.x << ", " << direccion.y << ", " << direccion.z << " }" << endl;
	if (nivel == 1) { //NORMALES
		glm::vec3 vecDir = posP - pos; // vector movimiento


		vecDir = vecDir / glm::length(vecDir);
		vecDir = glm::mix(vecDir, direccion, 0.9 + velRot);
		if (pos.y < 0.5 && vecDir.y < 0) {
			vecDir.y = abs(vecDir.y);
		}
		vecDir = vecDir / glm::length(vecDir); //normalizar vector
		pos += (0.01f * velocidad) * vecDir;
		direccion = vecDir;
	}
	else if (nivel == 2) { //SIGUEN PERFECTO
		glm::vec3 vecDir = posP - pos; // vector movimiento
		vecDir = vecDir / glm::length(vecDir); //normalizar vector
		pos += (0.01f * velocidad) * vecDir;
		direccion = vecDir;
	}
	else{
		glm::vec3 mira = es->per->direccion;
		glm::vec3 dirAux = posP - pos;
		mira.y = 0; dirAux.y = 0;
		mira = mira / glm::length(mira);
		dirAux = dirAux / glm::length(dirAux);
		double alpha = glm::dot(mira,dirAux) / (glm::length(mira)*glm::length(dirAux));
		alpha = acos(alpha);
		if (glm::cross(mira, dirAux).y > 0) {
			alpha = -alpha;
		}
		alpha = alpha / 2;
		glm::vec3 vecDir = { 0,0,0 }; // vector movimient
		vecDir.x = dirAux.x * cos(alpha) - dirAux.z*sin(alpha);
		vecDir.z = dirAux.x * sin(alpha) + dirAux.z*cos(alpha);
		vecDir = vecDir / glm::length(vecDir); //normalizar vector
		pos += (0.01f * velocidad) * vecDir;
		direccion = vecDir;
		/*cout << "MIRA: " << "{ " << mira.x << ", " << mira.y << ", " << mira.z << " }" << endl;
		cout << "DIRAUX: " << "{ " << dirAux.x << ", " << dirAux.y << ", " << dirAux.z << " }" << endl;
		cout << "DIRFINAL: " << "{ " << direccion.x << ", " << direccion.y << ", " << direccion.z << " }" << endl;
		*/
		if (pos.y > 0.5f) {
			pos += glm::vec3({ 0,-velocidad /2,0 })*0.01f;
		}
	}
	if (glm::length(pos - posP) <= 2*tam && !es->per->modoDios) {
		//cout << "MUERTO MATAO" << endl;
		es->per->morir();
	}
}

bool CalaveraBase3D::vivo() {
	vector<shared_ptr<Bala3D>> * b = es->getBalas();
	int i = 0;
	while (i < b->size()) {
		if (glm::length(pos - b->at(i)->pos) < tam + 0.25f + b->at(i)->tam) {
			vida -= b->at(i)->danyo;
			b->erase(b->begin() + i);
		}
		else {
			i++;
		}
	}
	if (vida <= 0 && nivel > 1) {
		//mciSendString("play ../DevilDaggers/videojuego/Codigo/Musica/calaveraI.mp3", NULL, 0, NULL);
		Obj3D cubo;
		Escena3D es3D;
		Render3D::loadOBJ("../DevilDaggers/videojuego/Codigo/3D/cubo.obj", cubo.vertices, cubo.uvs, cubo.normals);
		shared_ptr<Gema3D> sg = make_shared<Gema3D>(Gema3D(pos, { 1,1,0 }, es, window, cam, cubo));
		sg->vida = 0;
		sg->tiempecito = clock();
		es->add(sg);
	}
	return vida>0;
}
CalaveraBase3D CalaveraBase3D::operator=(const CalaveraBase3D& b) {
	if (this != &b) { // self-assignment check expected
		this->velocidad = b.velocidad;
		this->velRot = b.velRot;
		this->vida = b.vida;
		this->muerto = b.muerto;
		for (int i = 0; i < 3; i++) {
			this->dir[i] = b.dir[i];
		}

		this->gen = b.gen;
		this->distribution = b.distribution;
		this->es = b.es;
	}
	return *this;
}

void CalaveraBase3D::mover() {
	if (!muerto) {
		seguir();
		muerto = !vivo();
		if (muerto) {
			sp = new SistemaParticulas(pos, window, cam, colors[0], 20);
			//thread t1(&SistemaParticulas::recogeLosThreads, sp);
			//spFin = &t1;
			tam = 0.001f;
		}
	}
	else {
		for (int i = 0; i < sp->parts.size(); i++) {
			sp->parts[i].lightPos = es->per->lightPos;
		}
		sigue = sp->renderizar();
	}
}

void CalaveraBase3D::fisicas() {
	sigue = vivo();
}
