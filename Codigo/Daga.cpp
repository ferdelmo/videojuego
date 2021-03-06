#include "Daga.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <Windows.h>
#include <mmsystem.h>
#include <random>
#include <cmath>
#include <ctime>
#include <vector>
#include <string>
#include <stdlib.h>

#include "CalaveraBase.h"
#include "CalaveraBaseII.h"
#include "CalaveraBaseIII.h"
#include "LoadShader.h"
#include "Personaje.h"
#include "Escena.h"
#include "Bala.h"
#include "Gema.h"
#include <SOIL.h>
#include <string>

using namespace std;

Daga::Daga(GLfloat posi[],Escena * es, int numGemas, GLFWwindow* window, Camara * c)
	: Renderizable(window, "../DevilDaggers/videojuego/Codigo/daga"+to_string(numGemas)+".png", "../DevilDaggers/videojuego/Codigo/Shaders/daga.vert", "../DevilDaggers/videojuego/Codigo/Shaders/daga.frag", 0.085f,c){
	this->es = es;
	this->nivel = numGemas;

	cout << "DAGA GENERADA EN : ";
	for (int i = 0; i < 3; i++) {
		this->pos[i] = posi[i];
		cout << pos[i] << ", ";
	}
	cout << window << " " << cam << " " << es << endl;
	cout << endl;
	gemas = vector<shared_ptr<Gema>>();
	//a�adir las gemas necesarias
	GLfloat div = 2 * pi / numGemas; //para colocarlas alrededor de la torre
	for (int i = 0; i < numGemas; i++) {
		GLfloat posIni[3] = { pos[0]+tam*cos(i*div), pos[1]+tam*sin(i*div), pos[2] };
		shared_ptr<Gema> sg = make_shared<Gema>(Gema(posIni[0], posIni[1], posIni[2],es,window,c));
		shared_ptr<Gema> sg1(sg);
		gemas.push_back(sg);
		es->add(sg1);
	}
	
	//numeros aleatorios
	
	distribution = uniform_real_distribution<float>(-0.5, 0.5);
	random_device rd;
	// Initialize Mersenne Twister pseudo-random number generator
	gen = mt19937(rd());
	
	ultimaGen = clock();

	tiempecito = clock();
	sha = LoadShaders("../DevilDaggers/videojuego/Codigo/Shaders/calavera.vert", "../DevilDaggers/videojuego/Codigo/Shaders/calavera.frag");
	sha2 = LoadShaders("../DevilDaggers/videojuego/Codigo/Shaders/calavera.vert", "../DevilDaggers/videojuego/Codigo/Shaders/calavera.frag");
	sha3 = LoadShaders("../DevilDaggers/videojuego/Codigo/Shaders/calavera.vert", "../DevilDaggers/videojuego/Codigo/Shaders/calavera.frag");
}

Daga::Daga(GLfloat posi[], Escena * es, int numGemas, GLFWwindow* window, Camara * c, GLuint sha)
	: Renderizable(window, "../DevilDaggers/videojuego/Codigo/daga" + to_string(numGemas) + ".png", 0.085f, c, sha) {
	this->es = es;
	this->nivel = numGemas;
	cout << "DAGA GENERADA EN : ";
	for (int i = 0; i < 3; i++) {

		this->pos[i] = posi[i];
		cout << pos[i] << ", ";
	}
	cout << window << " " << cam << " " << es << endl;
	cout << endl;
	gemas = vector<shared_ptr<Gema>>();
	//a�adir las gemas necesarias
	GLfloat div = 2 * pi / numGemas; //para colocarlas alrededor de la torre
	for (int i = 0; i < numGemas; i++) {
		GLfloat posIni[3] = { pos[0] + tam * cos(i*div), pos[1] + tam * sin(i*div), pos[2] };
		shared_ptr<Gema> sg = make_shared<Gema>(Gema(posIni[0], posIni[1], posIni[2], es, window, c));
		shared_ptr<Gema> sg1(sg);
		gemas.push_back(sg);
		es->add(sg1);
	}

	//numeros aleatorios
	distribution = uniform_real_distribution<float>(-0.5f, 0.5f);
	random_device rd;
	// Initialize Mersenne Twister pseudo-random number generator
	gen = mt19937(rd());

	tiempecito = clock();

	this->sha = LoadShaders("../DevilDaggers/videojuego/Codigo/Shaders/calavera.vert", "../DevilDaggers/videojuego/Codigo/Shaders/calavera.frag");
	sha2 = LoadShaders("../DevilDaggers/videojuego/Codigo/Shaders/calaveraII.vert", "../DevilDaggers/videojuego/Codigo/Shaders/calaveraII.frag");
	sha3 = LoadShaders("../DevilDaggers/videojuego/Codigo/Shaders/calaveraIII.vert", "../DevilDaggers/videojuego/Codigo/Shaders/calaveraIII.frag");
}

void Daga::GenerarCalaveras(int n) {
	for (int i = 0; i < n; i++) {
		float x = distribution(gen);
		float y = distribution(gen);
		es->add(make_shared<CalaveraBase>(CalaveraBase(pos[0] + x * tam, pos[1] + y * tam, pos[2],es,window,cam,sha)));
	}
	float x = distribution(gen);
	float y = distribution(gen);
	es->add(make_shared<CalaveraBaseIII>(CalaveraBaseIII(pos[0] + x * tam, pos[1] + y * tam, pos[2], es, window, cam,sha3)));
}

void Daga::GenerarCalaverasII(int n) {
	for (int i = 0; i < n; i++) {
		float x = distribution(gen);
		float y = distribution(gen);
		es->add(make_shared<CalaveraBase>(CalaveraBase(pos[0] + x * tam, pos[1] + y * tam, pos[2], es, window, cam, sha)));
	}
	float x = distribution(gen);
	float y = distribution(gen);
	es->add(make_shared<CalaveraBaseII>(CalaveraBaseII(pos[0] + x * tam, pos[1] + y * tam, pos[2], es, window, cam, sha2)));
}

void Daga::GenerarCalaverasIII(int n) {
	for (int i = 0; i < n; i++) {
		float x = distribution(gen);
		float y = distribution(gen);
		es->add(make_shared<CalaveraBase>(CalaveraBase(pos[0] + x * tam, pos[1] + y * tam, pos[2], es, window, cam, sha)));
	}
	float x = distribution(gen);
	float y = distribution(gen);
	es->add(make_shared<CalaveraBaseII>(CalaveraBaseII(pos[0] + x * tam, pos[1] + y * tam, pos[2], es, window, cam, sha2)));
	x = distribution(gen);
	y = distribution(gen);
	es->add(make_shared<CalaveraBaseIII>(CalaveraBaseIII(pos[0] + x * tam, pos[1] + y * tam, pos[2], es, window, cam, sha3)));

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
	if (gemas.size() <= 0 && es->getPer()->opciones->sonido == 1) {
		mciSendString("play ../DevilDaggers/videojuego/Codigo/Musica/calaveraI.mp3", NULL, 0, NULL);
	}
	return gemas.size() > 0;
}

void Daga::mover() {
	
	if(!generadaPos){
		generadaPos = true;
		posFinal[0] = distribution(gen);
		posFinal[1] = distribution(gen);
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
		//
	}
	
	GLfloat nuevaX, nuevaY, nuevaZ;
	GLfloat dirx = posFinal[0] - pos[0], diry = posFinal[1] - pos[1];
	GLfloat moduloDir = sqrt(dirx*dirx + diry * diry);
	GLfloat dirxNorm = dirx / moduloDir;
	GLfloat diryNorm = diry / moduloDir;
	if (distancia(pos[0], pos[1], posFinal[0], posFinal[1]) > tam/2.5) { 
		pos[0] += (0.005f * velocidad)*dirxNorm;
		pos[1] += (0.005f * velocidad)*diryNorm;
	}
	orientacion += velRot;
	GLfloat div = 2 * pi / gemas.size(); //para colocarlas alrededor de la torre
	for (int i = 0; i < gemas.size(); i++) {
		nuevaX = pos[0] + (tam * cos(orientacion+i*div));
		nuevaY = pos[1] + (tam * sin(orientacion + i*div));
		gemas[i]->setPos(nuevaX, nuevaY, gemas.at(i)->pos[2]);
	}

	//cout << int(clock() - tiempecito) / CLOCKS_PER_SEC << endl;
	if ((int(clock() - tiempecito) / CLOCKS_PER_SEC) % tiempoGen == 0 && generadas != int(clock() - tiempecito) / CLOCKS_PER_SEC) {
		if (nivel == 1) {
			GenerarCalaveras(8);
		}
		else if (nivel == 2) {
			GenerarCalaverasII(8);
		}
		else if (nivel == 3) {
			GenerarCalaverasIII(8);
		}
		//cout << "GENERANDOOOO" << endl;
		generadas = int(clock() - tiempecito) / CLOCKS_PER_SEC;
	}
}

void Daga::fisicas() {
	//mira colision con personaje pa matarlo
	shared_ptr<Personaje> a = es->getPer();
	GLfloat posPer[] = { 0,0,0 };
	a->getPosition(posPer);
	if (distancia(pos[0], pos[1], posPer[0], posPer[1]) <= tam * tam + a->tam * a->tam && !a->modoDios) {
		a->morir();
	}
	//colisiona las gemas con las balas
	sigue = sigueVivo();

	//busca colisiones con las balas para eliminarlas
	vector<shared_ptr<Bala>> * b = es->getBalas();
	int i = 0;
	while (i < b->size()) {
		if (distancia(pos[0], pos[1], b->at(i)->pos[0], b->at(i)->pos[1]) <= tam*tam + b->at(i)->tam * b->at(i)->tam) {
			b->erase(b->begin() + i);
		}
		else {
			i++;
		}
	}
}