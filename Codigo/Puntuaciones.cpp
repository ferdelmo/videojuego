#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>
#include <GL/freeglut.h>

#include "Puntuaciones.h"
#include "LoadShader.h"

using namespace std;


Puntuaciones::Puntuaciones(GLFWwindow *window, Camara * c) {

	this->window = window;
	this->c = c;

	mode = 4;

	oldState = GLFW_RELEASE;
	pulsado = false;

	LeerFichero();

	//VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//VERTICES
	// genera el buffer para vertices
	glGenBuffers(1, &points_VBO);
	// bindea el buffer
	glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
	// Llena el buffer con la informacion de los puntos
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// EBO
	glGenBuffers(1, &EBO);
	// bindea ese buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// Lo rellena con la informacion de indices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);

	//COLOR
	// Genera el buffer de colores
	glGenBuffers(1, &colors_VBO);
	// bindea ese buffer
	glBindBuffer(GL_ARRAY_BUFFER, colors_VBO);
	// Lo rellena con la informacion del color
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	//TEXTURA BUFFER
	glGenBuffers(1, &texCoords_VBO);
	// bindea ese buffer
	glBindBuffer(GL_ARRAY_BUFFER, texCoords_VBO);
	// Lo rellena con la informacion de las coordenadas
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

	//TEXTURA PROPIA
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWid, texHei, 0, GL_RGB,
		GL_UNSIGNED_BYTE, texImage);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(texImage);

	shaderProgram = LoadShaders("../DevilDaggers/videojuego/Codigo/Shaders/puntuaciones.vert", "../DevilDaggers/videojuego/Codigo/Shaders/puntuaciones.frag");

}

void Puntuaciones::LeerFichero() {
	fstream f;
	int i = 0;

	f.open(fich, ios::in);
	if (f.is_open()) {

		while (!f.eof() && i<10) {
			f >> topPunt[i];
			i++;
		}

		f.close();
	}
	else {
		ofstream mifich(fich);
		mifich << 0 << '\n';
		mifich.close();
	}
}

void Puntuaciones::controlesInFrame() {

	mode = 4;

	// Posición del cursor
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	int px, py;
	glfwGetWindowSize(window, &px, &py);

	//calula la posicion relativa de la raton en la pantalla
	x = (x / px - 0.5f) * 2;
	y = (abs(y - py) / py - 0.5f) * 2;

	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);

	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

	if (state == GLFW_RELEASE && oldState == GLFW_PRESS) {
		pulsado = false;
	}

	if (!pulsado) { // Control una pulsación
		if (state == GLFW_PRESS) {
			//cout << "x: " << x << " ;; y: " << y << endl;
			if (x >= 0.5 && x <= 0.96&& y >= -0.89 && y <= -0.74) { // BOTÓN MENÚ PRINCIPAL
				pulsado = true;
				mode = 1;
			}
		}
	}

	oldState = state;

}

int Puntuaciones::renderizar() {
	controlesInFrame();

	glUseProgram(shaderProgram);

	glBindVertexArray(VAO); // une el VAO, que contiene toda la
							//información de los vértices, al contexto
							//vertices
							//argumento 0 posicion
	glEnableVertexAttribArray(0);
	//bindea el buffer
	glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
	//lo leena con los puntos
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	// bindea ese buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// Lo rellena con la informacion de puntos
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	//color
	//argumento 1 color
	glEnableVertexAttribArray(1);
	//bindea el buffer
	glBindBuffer(GL_ARRAY_BUFFER, colors_VBO);
	// le pasa el color de cada vertice
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);
	//texure
	//argumento 2 textura
	glEnableVertexAttribArray(2);
	//bindea el buffer
	glBindBuffer(GL_ARRAY_BUFFER, texCoords_VBO);
	// le pasa el color de cada vertice
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(
		2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);
	/*glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
	NULL); // en este caso el ID es 2 (0 y 1
	// corresponden a las posiciones y colores)
	glEnableVertexAttribArray(2);*/
	//Dibuja los trinagulos (3 vertices)
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  // Starting from vertex 0; 3 vertices total -> 1 triangle
	glBindVertexArray(0); // deshace la unión del VAO

						  //desactiva estos argumentos una vez usados
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	int cont = 0;
	GLfloat x = -1;
	GLfloat y = 1;

	for (int i = 0; i < 10; i++) {
		if (i == 0) {
			displayText(-0.1, 0.5 - i * 0.10, 1, 0, 0, "1ST " + topPunt[i]);
		}
		else if (i == 1) {
			displayText(-0.1, 0.5 - i * 0.10, 1, 0, 0, "2ND " + topPunt[i]);
		}
		else if (i == 2) {
			displayText(-0.1, 0.5 - i * 0.10, 1, 0, 0, "3RD " + topPunt[i]);
		}
		else {
			displayText(-0.1, 0.5 - i * 0.10, 1, 0, 0, topPunt[i]);
		}
	}


	return mode;

}