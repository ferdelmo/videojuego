#define GLEW_STATIC
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace std;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
		
}


int main() {
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 600, "PICHA BRAVA", nullptr,
		nullptr);
	if (window == nullptr)
	{
		glfwTerminate();
		return -1; // o cualquier otra cosa que queramos que se ejecute
	}
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

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

	// triangulo
	GLfloat points[] = {
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};


	while (!glfwWindowShouldClose(window))
	{
		// Renderizamos
		glClearColor(0.8f, 0.1f, 0.1f, 1.0f); /* especifica el color
		usado cuando los color buffers se borran.Por defecto es negro,
			aquí lo estamos fijando a un color rojizo. */
			glClear(GL_COLOR_BUFFER_BIT); /* la ventana tomará el color
		previamente fijado mediante glClearColor */
			// Hacemos el intercambio de front y back buffer
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}