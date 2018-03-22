#define GLEW_STATIC
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <ctime>
#include <thread>

#include "Personaje.h"
#include "Escena.h"
#include "Daga.h"

using namespace std;

int main() {
	glfwInit();


	GLFWwindow* window = glfwCreateWindow(800, 800, "GAME", nullptr,
		nullptr);
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
	
	//instancia el personage
	Escena es;
	Personaje per = Personaje(&es);
	per.setWindow(window);
	//evita perder teclas entre frames
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	//glfwSetKeyCallback(window, Personaje::controles)
	//glfwSetMouseButtonCallback(window, Personaje::mouse);
	Daga d(&es,2);
	es.add(make_shared<Personaje>(per));
	es.add(make_shared<Daga>(d));
	clock_t inicio;
	while (!glfwWindowShouldClose(window))
	{
		//chrono c++11
		inicio = clock();
		//BORRA EL FONDO
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//renderiza escena
		/*glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
		es.renderizar();
		/*glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);*/
		//pinta lo que haya en los buffers
		glfwSwapBuffers(window);
		//lee los eventos
		glfwPollEvents();
		int ms = (1000 / 60.0) - (clock() - inicio) / (CLOCKS_PER_SEC / 1000);
		this_thread::sleep_for(chrono::milliseconds(ms));
	}

	glfwTerminate();
	return 0;
}