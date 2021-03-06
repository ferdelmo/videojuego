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


#include "CalaveraBase3D.h"
#include "../LoadShader.h"
#include "Personaje3D.h"
#include "Render3D.h"
#include "Escena3D.h"
#include "Bala3D.h"
#include "Gema3D.h"
#include <SOIL.h>
#include <string>

#include "Sonidos.h"

using namespace std;

Daga3D::Daga3D(glm::vec3 pos, glm::vec3 dir, Escena3D * es, GLFWwindow* window, Camara * c, Obj3D obj, int nivel)
	: Render3D(window, "../DevilDaggers/videojuego/Codigo/Shaders/3D.vert", "../DevilDaggers/videojuego/Codigo/Shaders/3D.frag", c, obj, { 0.2,0.2,0.2 }, 1) {
	this->es = es;
	this->nivel = nivel;
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
	//a�adir las gemas necesarias
	GLfloat div = 2 * pi / nivel; //para colocarlas alrededor de la torre
	for (int i = 0; i < nivel; i++) {
		glm::vec3 posIni = { pos[0]+tam*cos(i*div), pos[1], pos[2] + tam * sin(i*div) };
		Obj3D cubo;
		Escena3D es3D;
		Render3D::loadOBJ("../DevilDaggers/videojuego/Codigo/3D/gema.obj", cubo.vertices, cubo.uvs, cubo.normals);
		shared_ptr<Gema3D> sg = make_shared<Gema3D>(Gema3D(posIni, { 1,1,0 }, es, window, cam, cubo));
		sg->iden = i;
		es->add(sg);
		gemas.push_back(sg);
	}
	
	//numeros aleatorios
	if (pos.x > 0) {
		distribution = uniform_real_distribution<float>(2.5, 5);
	}
	else if(pos.x == 0){
		distribution = uniform_real_distribution<float>(-2.5, 2.5);
	}
	else {
		distribution = uniform_real_distribution<float>(-5, -2.5);
	}
	if (pos.z > 0) {
		distribution2 = uniform_real_distribution<float>(2.5, 5);
	}
	else if (pos.z == 0) {
		distribution2 = uniform_real_distribution<float>(-2.5, 5);
	}
	else {
		distribution2 = uniform_real_distribution<float>(-5, -2.5);
	}
	
	random_device rd;
	// Initialize Mersenne Twister pseudo-random number generator
	gen = mt19937(rd());
	
	ultimaGen = clock();

	tiempecito = clock()+1;

	//ESCALA EL EJE Y PARA QUE SEAN MAS ALARGADAS
	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].y = vertices[i].y * 2;
	}
	actualizarVertices();
}

void Daga3D::GenerarCalaveras(int n) {
	Obj3D cubo;
	Render3D::loadOBJ("../DevilDaggers/videojuego/Codigo/3D/CALAVERAREC.obj", cubo.vertices, cubo.uvs, cubo.normals);
	glm::vec3 posVieja = pos;
	pos = pos + glm::vec3({ 0,2,0 });
	if (nivel == 1) {
		for (int i = 0; i < n; i++) {
			CalaveraBase3D cal1(pos, { 0, 0, 0 }, es, window, cam, cubo, 1);
			es->add(make_shared<CalaveraBase3D>(cal1));
		}
		CalaveraBase3D cal3( pos , { 0, 0, 0 }, es, window, cam, cubo, 3);
		es->add(make_shared<CalaveraBase3D>(cal3));
	}
	else if (nivel == 2) {
		for (int i = 0; i < n; i++) {
			CalaveraBase3D cal1(pos , { 0, 0, 0 }, es, window, cam, cubo, 1);
			es->add(make_shared<CalaveraBase3D>(cal1));
		}
		CalaveraBase3D cal2(pos , { 0, 0, 0 }, es, window, cam, cubo, 2);
		es->add(make_shared<CalaveraBase3D>(cal2));
	}
	else {
		for (int i = 0; i < n; i++) {
			CalaveraBase3D cal1(pos , { 0, 0, 0 }, es, window, cam, cubo, 1);
			es->add(make_shared<CalaveraBase3D>(cal1));
		}
		CalaveraBase3D cal2(pos, { 0, 0, 0 }, es, window, cam, cubo, 2);
		es->add(make_shared<CalaveraBase3D>(cal2));
		CalaveraBase3D cal3(pos, { 0, 0, 0 }, es, window, cam, cubo, 3);
		es->add(make_shared<CalaveraBase3D>(cal3));
	}
	pos = posVieja;
}

bool Daga3D::sigueVivo() {
	vector<shared_ptr<Bala3D>> * b = es->getBalas();
	int i = 0;
	while (i < b->size()) {
		if (glm::length(pos - b->at(i)->pos) < tam*2 + b->at(i)->tam) {
			b->erase(b->begin() + i);
		}
		/*float aux = b->at(i)->pos.y - pos.y+4;
		cout << aux << endl;
		if (glm::length(pos - b->at(i)->pos) < 1.6*tam*aux/4 + b->at(i)->tam && aux>0 && aux<10) {
		b->erase(b->begin() + i);
		}*/
		else {
			i++;
		}
	}
	i = 0;
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
	if (gemas.size() > 0) {
		if (!generadaPos) {
			generadaPos = true;
			posFinal = { distribution(gen), 2, distribution2(gen) };
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
		if (glm::length(pos - posFinal) > tam / 2.5) {
			pos += (0.01f * velocidad)*(direccioncita / glm::length(direccioncita));
		}

		float ang = glm::radians(velRot);
		glm::vec3 nueva = direccion;
		nueva.x = direccion.x*cos(ang) + direccion.z*sin(ang);
		nueva.z = -direccion.x*sin(ang) + direccion.z*cos(ang);
		direccion = nueva;

		GLfloat div = 2 * pi / nivel; //para colocarlas alrededor de la torre
		for (int i = 0; i < gemas.size(); i++) {
			/*nuevaX = pos[0] + (tam * cos(orientacion+i*div));
			nuevaY = pos[1] + (tam * sin(orientacion + i*div));*/

			glm::vec3 nueva = direccion;
			nueva.x = direccion.x*cos(gemas[i]->iden*div) + direccion.z*sin(gemas[i]->iden*div);
			nueva.z = -direccion.x*sin(gemas[i]->iden*div) + direccion.z*cos(gemas[i]->iden*div);
			glm::vec3 pollas = nueva * glm::vec3({ tam, 0, tam });
			nuevaPos = pos + pollas + glm::vec3({ 0,2,0 });

			gemas[i]->setPos(nuevaPos);
			gemas[i]->tam = 0.35f;
			gemas[i]->direccion = nueva;
		}
		if (glm::length(pos - es->per->pos) <= tam + es->per->tam && !es->per->modoDios) {
			es->per->morir();
		}
		//cout << int(clock() - tiempecito) / CLOCKS_PER_SEC << endl;
		if ((int(clock() - tiempecito) / CLOCKS_PER_SEC) % tiempoGen == 0 && generadas != int(clock() - tiempecito) / CLOCKS_PER_SEC) {
			GenerarCalaveras(8);

			//cout << "GENERANDOOOO" << endl;
			generadas = int(clock() - tiempecito) / CLOCKS_PER_SEC;
		}
	}
	else {
		if (pos.y > -3) {
			if (velocidad < 8) { 
				velocidad += 0.2; 
			}
			if (velRot < 15) {
				velRot += 0.2; 
			}
			pos += (0.01f * velocidad) * glm::vec3({0,-1,0});
			float ang = glm::radians(velRot);
			glm::vec3 nueva = direccion;
			nueva.x = direccion.x*cos(ang) + direccion.z*sin(ang);
			nueva.z = -direccion.x*sin(ang) + direccion.z*cos(ang);
			direccion = nueva;
		}
		else {
			sigue = false;
		}
	}
}

void Daga3D::fisicas() {
	//mira colision con personaje pa matarlo
	if (glm::length(pos - es->per->pos) <= tam + es->per->tam && !es->per->modoDios) {
		//a->morir();
	}
	//colisiona las gemas con las balas
	sigueVivo();

	//busca colisiones con las balas para eliminarlas
	/*vector<shared_ptr<Bala3D>> * b = es->getBalas();
	int i = 0;
	while (i < b->size()) {
		if (glm::length(pos - b->at(i)->pos) < tam + b->at(i)->tam) {
			b->erase(b->begin() + i);
		}
		else {
			i++;
		}
	}*/
}