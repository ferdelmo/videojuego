#include "Daga3D.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <random>
#include <cmath>
#include <ctime>
#include <vector>
#include <string>
#include <stdlib.h>

#include <Windows.h>
#include <mmsystem.h>

#include "CalaveraBase3D.h"
#include "../LoadShader.h"
#include "Personaje3D.h"
#include "Render3D.h"
#include "Escena3D.h"
#include "Bala3D.h"
#include "Gema3D.h"
#include <SOIL.h>
#include <string>

using namespace std;

Daga3D::Daga3D(glm::vec3 pos, glm::vec3 dir, Escena3D * es, GLFWwindow* window, Camara * c, Obj3D obj, int nivel)
	: Render3D(window, "../DevilDaggers/videojuego/Codigo/Shaders/3D.vert", "../DevilDaggers/videojuego/Codigo/Shaders/3D.frag", c, obj, { 1,0,0 }, 1) {
	this->es = es;
	this->nivel = nivel;
	distribution = uniform_real_distribution<float>(-0.5, 0.5);
	if (nivel <= 1) {
		for (int i = 0; i < vertices.size(); i++) {
			colors[i] = { 1,1,0 };
		}
	}
	else if (nivel == 2) {
		for (int i = 0; i < vertices.size(); i++) {
			colors[i] = { 0,1,1 };
		}
	}
	else {
		for (int i = 0; i < vertices.size(); i++) {
			colors[i] = { 1,0,1 };
		}
	}
	this->pos = pos;
	cout << window << " " << cam << " " << es << endl;
	cout << endl;
	gemas = vector<shared_ptr<Gema3D>>();
	//añadir las gemas necesarias
	GLfloat div = 2 * pi / nivel; //para colocarlas alrededor de la torre
	for (int i = 0; i < nivel; i++) {
		glm::vec3 posIni = { pos[0]+tam*cos(i*div), pos[1]+tam*sin(i*div), pos[2] };
		Obj3D cubo;
		Escena3D es3D;
		Render3D::loadOBJ("../DevilDaggers/videojuego/Codigo/3D/cubo.obj", cubo.vertices, cubo.uvs, cubo.normals);
		shared_ptr<Gema3D> sg = make_shared<Gema3D>(Gema3D(posIni, { 1,1,1 }, es, window, cam, cubo));
		es->add(sg);
		gemas.push_back(sg);
	}
	
	//numeros aleatorios
	
	distribution = uniform_real_distribution<float>(-0.5, 0.5);
	distribution2 = uniform_real_distribution<float>(tam/2, tam);
	random_device rd;
	// Initialize Mersenne Twister pseudo-random number generator
	gen = mt19937(rd());
	
	ultimaGen = clock();

	tiempecito = clock()+1;
}

void Daga3D::GenerarCalaveras(int n) {
	Obj3D cubo;
	Render3D::loadOBJ("../DevilDaggers/videojuego/Codigo/3D/CALAVERA.obj", cubo.vertices, cubo.uvs, cubo.normals);
	if (nivel == 1) {
		for (int i = 0; i < n; i++) {
			glm::vec3 posOffset = { distribution(gen), distribution(gen), distribution(gen)};
			posOffset = pos + posOffset;
			CalaveraBase3D cal1(posOffset, { 0, 0, 0 }, es, window, cam, cubo, 1);
			es->add(make_shared<CalaveraBase3D>(cal1));
		}
		glm::vec3 posOffset = { distribution(gen) , distribution(gen), distribution(gen) };
		posOffset = pos + posOffset;
		CalaveraBase3D cal3( posOffset , { 0, 0, 0 }, es, window, cam, cubo, 3);
		es->add(make_shared<CalaveraBase3D>(cal3));
	}
	else if (nivel == 2) {
		for (int i = 0; i < n; i++) {
			glm::vec3 posOffset = { distribution(gen) , distribution(gen), distribution(gen) };
			posOffset = pos + posOffset;
			CalaveraBase3D cal1(posOffset , { 0, 0, 0 }, es, window, cam, cubo, 1);
			es->add(make_shared<CalaveraBase3D>(cal1));
		}
		glm::vec3 posOffset = { distribution(gen) , distribution(gen), distribution(gen) };
		posOffset = pos + posOffset;
		CalaveraBase3D cal2(posOffset , { 0, 0, 0 }, es, window, cam, cubo, 2);
		es->add(make_shared<CalaveraBase3D>(cal2));
	}
	else {
		for (int i = 0; i < n; i++) {
			glm::vec3 posOffset = { distribution(gen) , distribution(gen), distribution(gen) };
			posOffset = pos + posOffset;
			CalaveraBase3D cal1(posOffset , { 0, 0, 0 }, es, window, cam, cubo, 1);
			es->add(make_shared<CalaveraBase3D>(cal1));
		}
		glm::vec3 posOffset = { distribution(gen) , distribution(gen), distribution(gen) };
		posOffset = pos + posOffset;
		CalaveraBase3D cal2(posOffset, { 0, 0, 0 }, es, window, cam, cubo, 2);
		es->add(make_shared<CalaveraBase3D>(cal2));
		posOffset = { distribution(gen) , distribution(gen), distribution(gen) };
		posOffset = pos + posOffset;
		CalaveraBase3D cal3(posOffset, { 0, 0, 0 }, es, window, cam, cubo, 3);
		es->add(make_shared<CalaveraBase3D>(cal3));
	}
}

bool Daga3D::sigueVivo() {
	int i = 0;
	while (i < gemas.size()) {
		bool siguenViva = gemas[i]->colisionBala();
		if (!siguenViva) {
			gemas.erase(gemas.begin() + i);
		}
		else {
			i++;
		}
	}
	return gemas.size() > 0;
}

void Daga3D::mover() {
	if(!generadaPos){
		generadaPos = true;
		posFinal = { distribution(gen), distribution2(gen), distribution(gen) };
		if (pos[0] < 0 && posFinal[0] > 0) {
			posFinal[0] = -posFinal[0];
		}
		else if (pos[0] > 0 && posFinal[0] < 0) {
			posFinal[0] = -posFinal[0];
		}
		if (pos[1] < 0 && posFinal[1] > 0) {
			posFinal[1] = -posFinal[1];
		}
		else if (pos[1] > 0 && posFinal[1] < 0) {
			posFinal[1] = -posFinal[1];
		}
	}
	
	glm::vec3 nuevaPos;
	glm::vec3 direccioncita = posFinal - pos;
	if (glm::length(pos - posFinal) > tam/2.5){ 
		pos += (0.005f * velocidad)*(direccioncita/glm::length(direccioncita));
	}
	GLfloat div = 2 * pi / gemas.size(); //para colocarlas alrededor de la torre
	for (int i = 0; i < gemas.size(); i++) {
		/*nuevaX = pos[0] + (tam * cos(orientacion+i*div));
		nuevaY = pos[1] + (tam * sin(orientacion + i*div));*/
		glm::vec3 pollas = { tam + gemas[i]->tam, 0 ,0 };
		nuevaPos = pos + pollas;
		gemas[i]->setPos(nuevaPos);
	}

	if ((int(clock() - tiempecito) / CLOCKS_PER_SEC) % tiempoGen == 0 && generadas != int(clock() - tiempecito) / CLOCKS_PER_SEC) {
		GenerarCalaveras(8);
		//cout << "GENERANDOOOO" << endl;
		generadas = int(clock() - tiempecito) / CLOCKS_PER_SEC;
	}
}

void Daga3D::fisicas() {
	//mira colision con personaje pa matarlo
	if (glm::length(pos - es->per->pos) <= tam + es->per->tam && !es->per->modoDios) {
		//a->morir();
	}
	//colisiona las gemas con las balas
	sigue = sigueVivo();

	//busca colisiones con las balas para eliminarlas
	vector<shared_ptr<Bala3D>> * b = es->getBalas();
	int i = 0;
	while (i < b->size()) {
		if (glm::length(pos - b->at(i)->pos) < tam + b->at(i)->tam) {
			b->erase(b->begin() + i);
		}
		else {
			i++;
		}
	}
}