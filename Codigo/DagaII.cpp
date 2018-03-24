#include "DagaII.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <random>
#include <cmath>
#include <ctime>
#include <vector>
#include <stdlib.h>

#include "CalaveraBase.h"
#include "CalaveraBaseII.h"
#include "CalaveraBaseIII.h"
#include "LoadShader.h"
#include "Personaje.h"
#include "Escena.h"
#include "Gema.h"
#include <SOIL.h>

using namespace std;

DagaII::DagaII(GLfloat posi[],Escena * es, int numGemas, GLFWwindow* window, Camara * c)
	: Renderizable(window, "../DevilDaggers/videojuego/Codigo/daga.png", "../DevilDaggers/videojuego/Codigo/Shaders/daga.vert", "../DevilDaggers/videojuego/Codigo/Shaders/daga.frag", 0.075f,c){
	this->es = es;
	for (int i = 0; i < 3; i++) {
		this->pos[i] = posi[i];
	}
	gemas = vector<shared_ptr<Gema>>();
	//añadir las gemas necesarias
	GLfloat div = 2 * pi / numGemas; //para colocarlas alrededor de la torre
	for (int i = 0; i < numGemas; i++) {
		GLfloat posIni[3] = { pos[0]+tam*cos(i*div), pos[1]+tam*sin(i*div), pos[2] };
		shared_ptr<Gema> sg = make_shared<Gema>(Gema(posIni[0], posIni[1], posIni[2],es,window,c));
		shared_ptr<Gema> sg1(sg);
		gemas.push_back(sg);
		es->add(sg1);
	}

	//numeros aleatorios
	distribution = uniform_real_distribution<float>(-1, 1);
	random_device rd;
	// Initialize Mersenne Twister pseudo-random number generator
	gen = mt19937(rd());
	
	ultimaGen = clock();
}

void DagaII::GenerarCalaveras(int n) {
	for (int i = 0; i < n; i++) {
		float x = distribution(gen);
		float y = distribution(gen);
		es->add(make_shared<CalaveraBase>(CalaveraBase(pos[0] + x * tam, pos[1] + y * tam, pos[2],es,window,cam)));
	}
	float x = distribution(gen);
	float y = distribution(gen);
	es->add(make_shared<CalaveraBaseII>(CalaveraBaseII(pos[0] + x * tam, pos[1] + y * tam, pos[2], es, window, cam)));
}

bool DagaII::sigueVivo() {
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

void DagaII::mover() {
	shared_ptr<Personaje> a = es->getPer();
	GLfloat posPer[] = { 0,0,0 };
	a->getPosition(posPer);
	GLfloat posP[] = { 0,0,0 };
	sigue = sigueVivo();
	GLfloat nuevaX, nuevaY, nuevaZ;
	GLfloat dirx = posP[0] - pos[0], diry = posP[1] - pos[1];
	GLfloat moduloDir = sqrt(dirx*dirx + diry * diry);
	GLfloat dirxNorm = dirx / moduloDir;
	GLfloat diryNorm = diry / moduloDir;
	if (distancia(pos[0], pos[1], 0, 0) > tam/2.5) { 
		pos[0] += (0.005f * velocidad)*dirxNorm;
		pos[1] += (0.005f * velocidad)*diryNorm;
	}
	orientacion += velRot;
	GLfloat div = 2 * pi / gemas.size(); //para colocarlas alrededor de la torre
	for (int i = 0; i < gemas.size(); i++) {
		nuevaX = pos[0] + (tam * cos(i*div+orientacion));
		nuevaY = pos[1] + (tam * sin(i*div+orientacion));

		gemas.at(i)->setPos(nuevaX, nuevaY, gemas.at(i)->pos[2]);
	}
	//cout << int(clock() - tiempecito) / CLOCKS_PER_SEC << endl;
	if ((int(clock() - tiempecito) / CLOCKS_PER_SEC) %10 == 0 && generadas != int(clock() - tiempecito) / CLOCKS_PER_SEC) {
		GenerarCalaveras(4);
		//cout << "GENERANDOOOO" << endl;
		generadas = int(clock() - tiempecito) / CLOCKS_PER_SEC;
	}
	if (distancia(pos[0], pos[1], posPer[0], posPer[1]) <= tam * tam + a->tam * a->tam) {
		a->morir();
	}
}