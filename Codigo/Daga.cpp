#include "Daga.h"

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
#include "LoadShader.h"
#include "Escena.h"
#include "Gema.h"
#include <SOIL.h>

using namespace std;

Daga::Daga(GLfloat posi[],Escena * es, int numGemas, GLFWwindow* window, Camara * c)
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

void Daga::GenerarCalaveras(int n) {
	for (int i = 0; i < n; i++) {
		float x = distribution(gen);
		float y = distribution(gen);
		es->add(make_shared<CalaveraBase>(CalaveraBase(pos[0] + x * tam, pos[1] + y * tam, pos[2],es,window,cam)));
	}
}

bool Daga::sigueVivo() {
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

void Daga::mover() {
	sigue = sigueVivo();
}