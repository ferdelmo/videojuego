#define GLEW_STATIC
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>

#include "Personaje.h"

using namespace std;


GLfloat points[] = {
	0.0f, 0.25f, 0.0f,
	0.25f, -0.25f, 0.25f,
	-0.25f, -0.25f, 0.0f
};



string leerStringFromFIle(string path) {
	ifstream inFile;
	inFile.open(path);//open the input file

	stringstream strStream;
	strStream << inFile.rdbuf();//read the file
	string str = strStream.str();//str holds the content of the file

	cout << str << endl;//you can do anything with the string!!!
	//cout << "PENE" << endl;
	return str;
}

const GLchar* vertexShaderSource = leerStringFromFIle("../DevilDaggers/videojuego/Codigo/tri.vert").c_str();
const GLchar* fragmentShaderSource = leerStringFromFIle("../DevilDaggers/videojuego/Codigo/tri.frag").c_str();

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
	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	GLuint points_VBO = 0;
	GLuint colors_VBO = 0;
	glGenBuffers(1, &points_VBO);
	glGenBuffers(1, &colors_VBO);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, colors_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	while (!glfwWindowShouldClose(window))
	{
		// Renderizamos
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); /* especifica el color
		usado cuando los color buffers se borran.Por defecto es negro,
			aquí lo estamos fijando a un color rojizo. */

		glClear(GL_COLOR_BUFFER_BIT); /* la ventana tomará el color
									  previamente fijado mediante glClearColor */
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // une el VAO, que contiene toda la información de los vértices, al contexto
		glDrawArrays(GL_TRIANGLES, 0, 3); /* dibuja los vértices del VBO/VAO
		activo con el shader activo; el 1er
			argumento es la primitiva a dibujar
			(puntos, líneas, triángulos, etc.,
				se pueden consultar en la
				documentación12)*/
		glBindVertexArray(0); // deshace la unión del VAO

		// Hacemos el intercambio de front y back buffer
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
		glEnableVertexAttribArray(0);
	}

	glfwTerminate();
	return 0;
}