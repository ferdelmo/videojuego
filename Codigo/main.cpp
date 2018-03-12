#define GLEW_STATIC
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>

#include "Personaje.h"

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
	Personaje &per = Personaje::getInstance();
	//evita perder teclas entre frames
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	//glfwSetKeyCallback(window, Personaje::controles)
	//glfwSetMouseButtonCallback(window, Personaje::mouse);

	while (!glfwWindowShouldClose(window))
	{
		//BORRA EL FONDO
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//coge las entradas de este frame
		per.controlesInFrame(window);
		//renderiza el personaje y las balas
		per.renderizar();
		//pinta lo que haya en los buffers
		glfwSwapBuffers(window);
		//lee los eventos
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}