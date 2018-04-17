#define GLEW_STATIC

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <ctime>
#include <thread>
#include <string>
#include <Windows.h>
#include <mmsystem.h>
#include <GL/freeglut.h>
#include <fstream>
#include <stdio.h>   
#include <stdlib.h> 
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

#include "3D\Render3D.h"
#include "3D\CalaveraBase3D.h"
#include "3D\Personaje3D.h"
#include "3D\Escena3D.h"

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
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
	}
	glEnable(GL_TEXTURE_2D);
}

string canciones[6] = { "1.mp3", "2.mp3", "3.mp3", "4.mp3", "5.mp3", "6.mp3" };
int duraciones[6] = {268, 146, 261, 344, 215, 268};

void playSong(int indice) {
	if (indice == 1) {
		mciSendString("play ../DevilDaggers/videojuego/Codigo/Musica/1.mp3", NULL, 0, NULL);
	}
	else if (indice == 2) {
		mciSendString("play ../DevilDaggers/videojuego/Codigo/Musica/2.mp3", NULL, 0, NULL);
	}
	else if (indice == 3) {
		mciSendString("play ../DevilDaggers/videojuego/Codigo/Musica/3.mp3", NULL, 0, NULL);
	}
	else if (indice == 4) {
		mciSendString("play ../DevilDaggers/videojuego/Codigo/Musica/4.mp3", NULL, 0, NULL);
	}
	else if (indice == 5) {
		mciSendString("play ../DevilDaggers/videojuego/Codigo/Musica/5.mp3", NULL, 0, NULL);
	}
	else {
		mciSendString("play ../DevilDaggers/videojuego/Codigo/Musica/6.mp3", NULL, 0, NULL);
	}
}

void stopSong(int indice) {
	if (indice == 1) {
		mciSendString("pause ../DevilDaggers/videojuego/Codigo/Musica/1.mp3", NULL, 0, NULL);
	}
	else if (indice == 2) {
		mciSendString("pause ../DevilDaggers/videojuego/Codigo/Musica/2.mp3", NULL, 0, NULL);
	}
	else if (indice == 3) {
		mciSendString("pause ../DevilDaggers/videojuego/Codigo/Musica/3.mp3", NULL, 0, NULL);
	}
	else if (indice == 4) {
		mciSendString("pause ../DevilDaggers/videojuego/Codigo/Musica/4.mp3", NULL, 0, NULL);
	}
	else if (indice == 5) {
		mciSendString("pause ../DevilDaggers/videojuego/Codigo/Musica/5.mp3", NULL, 0, NULL);
	}
	else {
		mciSendString("pause ../DevilDaggers/videojuego/Codigo/Musica/6.mp3", NULL, 0, NULL);
	}
}
int main(int argc, char **argv) {
	glfwInit();
	
	srand(time(NULL));
	int indice = rand() % 6 + 1;
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
		glm::vec3(4, 3, 8), // Camera is at (4,3,3), in World Space
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
	Fondo atras(0.6, -1, 0, window, "../DevilDaggers/videojuego/Codigo/atras.png", 0.4, 1, &cam), guardar(-0.6, -1, 0, window, "../DevilDaggers/videojuego/Codigo/guardar.png", 0.4, 1, &cam);
	opciones.setBotones(&atras,&guardar);
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
	
	int mode = 6;
	//Fondo f(window, "../DevilDaggers/videojuego/Codigo/muerte.png", 1, 1, &cam);
	Fondo f(0, 0, 0.0f, window, "../DevilDaggers/videojuego/Codigo/muerte01.png", 2, 1, &cam);
	Muerte muerte(window, &cam, &f);
	GLfloat texCoords[8] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,

	};
	bool estabaMuerto = false;
	//glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 0);
	clock_t puntuacion = clock(), finPunt = clock();
	//playSong(indice);
	clock_t tiempecito = clock();
	int antigua = opciones.resolucion;
	bool gameover = true;
	float topPunt[10] = { 0,0,0,0,0,0,0,0,0,0 };
	bool pulsado = false;


	Obj3D plano;
	Render3D::loadOBJ("../DevilDaggers/videojuego/Codigo/3D/plano.obj", plano.vertices, plano.uvs, plano.normals);
	vector<Render3D> r3d;
	r3d.push_back(Render3D(window, "../DevilDaggers/videojuego/Codigo/Shaders/3D.vert", "../DevilDaggers/videojuego/Codigo/Shaders/3D.frag", &cam, plano, { 0.11,0.11,0.11 },6));
	Obj3D cubo;
	Escena3D es3D;
	Render3D::loadOBJ("../DevilDaggers/videojuego/Codigo/3D/cubo.obj", cubo.vertices, cubo.uvs, cubo.normals);
	Personaje3D per3D({0, 1, 0}, &es3D, window, &cam, cubo);
	es3D.add(make_shared<Personaje3D>(per3D));

	CalaveraBase3D cal1({ 1, 1, 1 }, { 0, 0, 0 }, &es3D, window, &cam, cubo, 1);
	es3D.add(make_shared<CalaveraBase3D>(cal1));
	CalaveraBase3D cal2({ 1, 1, 1 }, { 0,0,0 }, &es3D, window, &cam, cubo, 2);
	es3D.add(make_shared<CalaveraBase3D>(cal2));
	CalaveraBase3D cal3({ 1, 1, 1 }, { 0,0,0 }, &es3D, window, &cam, cubo, 3);
	es3D.add(make_shared<CalaveraBase3D>(cal3));
	//r3d.push_back(per3D);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	while (!glfwWindowShouldClose(window))
	{
		/*if (estabaMuerto && es.getPer()->vivo) {
			mciSendString("pause ../DevilDaggers/videojuego/Codigo/Musica/gameOver.wav", NULL, 0, NULL);
			estabaMuerto = false;
			tiempecito = clock();
			indice = rand() % 6 + 1;
			playSong(indice);
		}
		if (!es.getPer()->vivo) {
			cout << "parando cancion " << indice << endl;
			estabaMuerto = true;
			stopSong(indice);
			mciSendString("play ../DevilDaggers/videojuego/Codigo/Musica/gameOver.wav", NULL, 0, NULL);
		}
		if(int(clock() - tiempecito) / CLOCKS_PER_SEC >= duraciones[indice-1]){
			tiempecito = clock();
			indice = rand() % 6 + 1;
			playSong(indice);
			//PlaySound(TEXT("../DevilDaggers/videojuego/Codigo/Musica/quack.wav"), NULL, SND_ASYNC);
		}*/
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
				for (int i = 0; i < 10; i++) {
					topPunt[i] = 0;
				}
				gameover = true;
				pulsado = false;
			}
		}
		else if (mode == 2) {
			//chrono c++11
			inicio = clock();
			string tiempo;
			//BORRA EL FONDO
			if (es.getPer()->vivo) {
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

				tiempo = to_string((clock() - puntuacion) / (CLOCKS_PER_SEC / 1000) / 1000.0f);
				tiempo = tiempo.substr(0, tiempo.size() - 3);
				displayText(0 - 0.10f, 0 + 0.8f, 1, 1, 1, "Tiempo: " + tiempo);

				displayText(0 - 0.50f,0 + 0.8f, 1, 0, 0, "Asesinadas: " + to_string(es.calavsMatadas));

				displayText(0 + 0.35f, 0 + 0.8f, 1, 1, 1, "Gemas: " + to_string(es.getPer()->numGemas));
				//pinta lo que haya en los buffers

				if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && pulsado) {
					mode = 1;
					pulsado = false;
				}
				else {
					pulsado = true;
				}
				glfwSwapBuffers(window);
				//lee los eventos
				glfwPollEvents();
				finPunt = clock();
				int ms = (1000.0f / 60.0f) - (clock() - inicio) / (CLOCKS_PER_SEC / 1000);
				if (ms > 0) {
					this_thread::sleep_for(chrono::milliseconds(ms));
				}
			}
			else {
				if (gameover) {
					fstream f;
					int i = 0;
					f.open(puntuaciones.fich, ios::in);
					if (f.is_open()) {

						while (!f.eof()) {
							f >> topPunt[i];
							i++;
						}

						f.close();
					}
					gameover = false;
					pulsado = false;
				}
				cam.View = glm::lookAt(
					glm::vec3(0, 0, 3), // Camera is at (4,3,3), in World Space
					glm::vec3(0, 0, 0), // and looks at the origin
					glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
				);

				cam.actualizarMVP();
				es.renderizar();
				f.renderizar();
				float PUNTOS = (finPunt - puntuacion) / (CLOCKS_PER_SEC / 1000) / 1000.0f;
				tiempo = to_string(PUNTOS);
				tiempo = tiempo.substr(0, tiempo.size() - 3);
				//cout << tiempo << endl;
				displayText(-0.1f,-0.0f, 1, 0, 0,tiempo);	
				for (int i = 0; i < 10; i++) {
					if (PUNTOS > topPunt[i]) {
						displayText(-0.25f, -0.1f, 1, 1, 1, "NUEVO RECORD, PUESTO: " + to_string(i+1));
						i = 10;
					}
				}

				double x, y;
				glfwGetCursorPos(window, &x, &y);
				//calula la posicion relativa de la raton en la pantalla
				x = (x / opciones.px - 0.5f) * 2;
				y = (abs(y - opciones.py) / opciones.py - 0.5f) * 2;
				if (x > -0.4 && x < -0.1 && y>-1 && y < -0.6 && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && pulsado) {
					cout << "PULSADO Intentar de nuevo" << endl;
					cout << "_______________________" << endl;
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
					ofstream f(puntuaciones.fich);
					int k = -1;
					for (int j = 0; j < 10; j++) {
						if (PUNTOS > topPunt[j]) {
							k = j;
							j = 10;
						}
					}
					for (int j = 9; j > k; j--) {
						topPunt[j] = topPunt[j - 1];
					}
					topPunt[k] = PUNTOS;
					int i = 0;
					if (f.is_open()) {

						while (i<10) {
							f << to_string(topPunt[i]) << '\n';
							i++;
						}

						f.close();
					}
					for (int i = 0; i < 10; i++) {
						topPunt[i] = 0;
					}
					gameover = true;
					pulsado = false;
					puntuaciones.LeerFichero();
				}
				else if (x > 0.1 && x < 0.4 && y>-1 && y < -0.6 && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && pulsado) {
					cout << "PULSADO Menu" << endl;
					cout << "_______________________" << endl;
					mode = 1;
					ofstream f(puntuaciones.fich);
					int k = -1;
					for (int j = 0; j < 10; j++) {
						if (PUNTOS > topPunt[j]) {
							k = j;
							j = 10;
						}
					}
					for (int j = 9; j > k ; j--) {
						topPunt[j] = topPunt[j - 1];
					}
					topPunt[k] = PUNTOS;
					int i = 0;
					if (f.is_open()) {

						while (i<10) {
							f << to_string(topPunt[i]) << '\n';
							i++;
						}

						f.close();
					}
					for (int i = 0; i < 10; i++) {
						topPunt[i] = 0;
					}
					gameover = true;
					pulsado = false;
					puntuaciones.LeerFichero();
				}
				else if(!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
					pulsado = true;
				}
				//displayText(0.1, -0.5, 1, 0, 0, "Asesinadas: " + to_string(es.calavsMatadas));

				//displayText(0.1, -0.5, 1, 1, 1, "Gemas: " + to_string(es.getPer()->numGemas));
				//pinta lo que haya en los buffers
				glfwSwapBuffers(window);
				//lee los eventos
				glfwPollEvents();
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
		else if (mode == 6) {
			mode = 6;
			//cout << "RENDERIZANDO" << endl;
			for (int i = 0; i < r3d.size(); i++) {
				r3d[i].renderizar();
			}
			es3D.renderizar();
			es3D.moverObjetos();
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