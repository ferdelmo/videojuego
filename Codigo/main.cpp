#define GLEW_STATIC

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <ctime>
#include <thread>
#include <Windows.h>
#include <mmsystem.h>
#include <GL/freeglut.h>

#include "Personaje.h"
#include "Escena.h"
#include "Daga.h"
#include "Fondo.h"
#include "Camara.h"
#include "Partida.h"

#include "MenuPrincipal.h"
#include "Puntuaciones.h"
#include "Opciones.h"
#include "Creditos.h"
#include "Muerte.h"

using namespace std;


void displayText(float x, float y, int r, int g, int b, string str) {
	glUseProgram(0);

	glLoadIdentity();
	glRasterPos2i(x, y); glDisable(GL_LIGHTING);
	glColor3f(r, g, b);
	/*float posXcorrected = 0;
	for (int i = 0; i < str.size() / 2; i++) {
	posXcorrected += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, str[i]);
	}*/
	glRasterPos3f(x, y, 0);

	glDisable(GL_TEXTURE_2D);
	for (int i = 0; i < str.size(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
	}
	glEnable(GL_TEXTURE_2D);
}

int main(int argc, char **argv) {
	glfwInit();
	clock_t tiempecito = 0;
	
	/*const GLFWvidmode *res = glfwGetVideoMode(glfwGetPrimaryMonitor());
	GLFWwindow* window = glfwCreateWindow(res->width, res->height, "GAME", nullptr,
	nullptr);*/
	GLFWwindow* window;
	Opciones opciones;
	window = glfwCreateWindow(opciones.px, opciones.py, "Devil Daggers", nullptr,
		nullptr);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);

	if (window == nullptr)
	{
		glfwTerminate();
		return -1; // o cualquier otra cosa que queramos que se ejecute
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		std::cout << "Fallo inicializando GLEW" << std::endl;
		return -1; // o cualquier otra cosa que queramos que se ejecute
	}
	int ancho, alto;
	glfwGetFramebufferSize(window, &ancho, &alto);
	glViewport(0, 0, ancho, alto);

	Camara cam;

	cam.FoV = 60;
	cam.View = glm::lookAt(
		glm::vec3(0, 0, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	cam.Projection = glm::perspective(
		glm::radians(cam.FoV), // El campo de visión vertical, en radián: la cantidad de "zoom". Piensa en el lente de la cámara. Usualmente está entre 90° (extra ancho) y 30° (zoom aumentado)
		ancho*1.0f/alto,       // Proporción. Depende del tamaño de tu ventana 4/3 == 800/600 == 1280/960, Parece familiar?
		0.1f,              // Plano de corte cercano. Tan grande como sea posible o tendrás problemas de precisión.
		100.0f             // Plano de corte lejano. Tan pequeño como se pueda.
	);
	cam.actualizarMVP();

	Fondo FI(-0.4, 0.75, 0, window, "../DevilDaggers/videojuego/Codigo/flecha.png", 0.1, 1, &cam), FD(0.4, 0.75, 0, window, "../DevilDaggers/videojuego/Codigo/flecha.png", 0.1, 1, &cam);
	opciones.setFlechas(&FI, &FD);
	opciones.setBotones(&Fondo(0.6, -1, 0, window, "../DevilDaggers/videojuego/Codigo/atras.png", 0.4, 1, &cam), &Fondo(-0.6, -1, 0, window, "../DevilDaggers/videojuego/Codigo/guardar.png", 0.4, 1, &cam));
	opciones.setWindow(window);

	//instancia el personage
	Escena es;
	Personaje perso = Personaje(0, 0, 0, &es, window, &cam);
	Personaje * per = &perso;
	//evita perder teclas entre frames
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	//glfwSetKeyCallback(window, Personaje::controles)
	//glfwSetMouseButtonCallback(window, Personaje::mouse);
	es.add(make_shared<Personaje>(perso));
	clock_t inicio;
	Partida aux(&es);
	Partida * par = &aux;

	// Pantallas
	MenuPrincipal menu = MenuPrincipal(window);
	Puntuaciones puntuaciones = Puntuaciones(window, &cam);
	Creditos creditos = Creditos(window);
	
	int mode = 1;
	//PlaySound(TEXT("../DevilDaggers/videojuego/Codigo/Musica/quack.wav"), NULL, SND_ASYNC);
	Fondo f(window, "../DevilDaggers/videojuego/Codigo/muerte.png", 1, 1, &cam);
	Muerte muerte(window, &cam, &f);

	clock_t puntuacion = clock();
	int antigua = opciones.resolucion;
	while (!glfwWindowShouldClose(window))
	{
		if(double(clock() - tiempecito) / CLOCKS_PER_SEC >= 11){
			tiempecito = clock();
			//PlaySound(TEXT("../DevilDaggers/videojuego/Codigo/Musica/quack.wav"), NULL, SND_ASYNC);
		}
		//BORRA EL FONDO
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (mode == 1) {
			mode = menu.renderizar();
			//pinta lo que haya en los buffers
			glfwSwapBuffers(window);
			//lee los eventos
			glfwPollEvents();
			if (mode == 2) {
				//empezaar partida
				es.reset();
				Personaje per = Personaje(0, 0, 0, &es, window, &cam);
				//evita perder teclas entre frames
				glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
				//glfwSetKeyCallback(window, Personaje::controles)
				//glfwSetMouseButtonCallback(window, Personaje::mouse);
				es.add(make_shared<Personaje>(per));
				Fondo fondo(window, "../DevilDaggers/videojuego/Codigo/suelo.png", 2, 10, &cam);
				es.setFondo(make_shared<Fondo>(fondo));
				Partida partid(&es);
				par = &partid;
				par->start();
				puntuacion = clock();
			}
		}
		else if (mode == 2) {
			//chrono c++11
			inicio = clock();
			//BORRA EL FONDO
			if (per->vivo) {
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				//renderiza escena
				//glDepthMask(GL_FALSE);
				/*glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
				//glViewport(-es.getPer()->pos[0]*ancho, -es.getPer()->pos[1]*alto, ancho, alto);

				

				par->actualizar();
				es.actualizarFisicas();
				es.moverObjetos();
				es.renderizar();

				string tiempo = to_string((clock() - puntuacion) / (CLOCKS_PER_SEC / 1000) / 1000.0f);
				tiempo = tiempo.substr(0, tiempo.size() - 3);
				displayText(per->pos[0] - 0.20f, per->pos[0] + 0.8f, 1, 1, 1, "Tiempo: " + tiempo);

				displayText(per->pos[0] - 0.60f, per->pos[0] + 0.8f, 1, 0, 0, "Asesinadas: " + to_string(es.calavsMatadas));

				displayText(per->pos[0] + 0.20f, per->pos[0] + 0.8f, 1, 1, 1, "Gemas: " + to_string(es.getPer()->numGemas));
				//pinta lo que haya en los buffers
				glfwSwapBuffers(window);
				//lee los eventos
				glfwPollEvents();
				int ms = (1000.0f / 60.0f) - (clock() - inicio) / (CLOCKS_PER_SEC / 1000);
				if (ms > 0) {
					this_thread::sleep_for(chrono::milliseconds(ms));
				}
			}
			else {
				es.renderizar();
				muerte.renderizar();
			}
		}
		else if (mode == 3) {
			mode = opciones.renderizar();
			//pinta lo que haya en los buffers
			glfwSwapBuffers(window);
			//lee los eventos
			glfwPollEvents();
			if (mode != 3 && antigua != opciones.resolucion) {
				mode = 100;
			}
		}
		else if (mode == 4) {
			mode = puntuaciones.renderizar();
			//pinta lo que haya en los buffers
			glfwSwapBuffers(window);
			//lee los eventos
			glfwPollEvents();
		}
		else if (mode == 5) {
			mode = creditos.renderizar();
			//pinta lo que haya en los buffers
			glfwSwapBuffers(window);
			//lee los eventos
			glfwPollEvents();
		}
		else {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			//cout << mode << endl;
		}
	}
	//fisicas.join();
	par->stop();
	glfwTerminate();
	return 0;
}