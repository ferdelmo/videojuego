#include "Personaje.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>

#include <random>
#include <cmath>
#include <ctime>
#include <vector>
#include <stdlib.h>

#include "LoadShader.h"
#include "Bala.h"
#include "Escena.h"
#include "CalaveraBase.h"

using namespace std;

//CONSTRUCTOR POR DEFECTO
Personaje::Personaje(Escena * es) {
	this->es = es;
	//prepara las balas
	ultimaBala = clock();
	//numeros aleatorios
	distribution = uniform_real_distribution<float>(0, 1);
	random_device rd;
	// Initialize Mersenne Twister pseudo-random number generator
	gen = mt19937(rd());

	//calcula los puntos del triangulo segun la orientacion
	for (int i = 0; i < 4; i++) {
		GLfloat auxx = 1;
		if (i / 2 != 0) {
			auxx = -1;
		}
		GLfloat auxy = 1;
		if (i % 2 != 0) {
			auxy = -1;
		}
		GLfloat puntoaux[3] = { pos[0] + tam * auxx,pos[1] + tam * auxy,pos[2] };
		GLfloat punto1a[3] = { 0,0,0 };
		rotatePoint(pos, puntoaux, orientacion, punto1a);
		for (int j = 0; j < 3; j++) {
			vertices[i * 3 + j] = punto1a[j];
		}
	}
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

	shaderProgram = LoadShaders("../DevilDaggers/videojuego/Codigo/Shaders/tri.vert", "../DevilDaggers/videojuego/Codigo/Shaders/tri.frag");
}

//CONSTRUCTOR EN UNA POSICION ESPECIFICA
Personaje::Personaje(GLfloat x, GLfloat y, GLfloat z, Escena * es) {
	pos[0] = x; pos[1] = y; pos[2] = z;
	this->es = es;
}

void Personaje::getPosition(GLfloat posi[]) {
	posi[0] = pos[0];
	posi[1] = pos[1];
	posi[2] = pos[2];
}

void Personaje::addGema() {
	this->numGemas++;
}
//FUNCION AUXILIAR CONTROLES POR INTERRUPCION TECLAS
void Personaje::controlesP(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	/*else if (key == GLFW_KEY_W && state==GLFW_PRESS) {
	pos[1] += velocidad*0.01f;
	}
	else if (key == GLFW_KEY_A && state == GLFW_PRESS) {
	pos[0] -= velocidad*0.01f;
	}
	else if (key == GLFW_KEY_S && state == GLFW_PRESS) {
	pos[1] -= velocidad* 0.01f;
	}
	else if (key == GLFW_KEY_D && state == GLFW_PRESS) {
	pos[0] += velocidad* 0.01f;
	}*/

}
//FUNCION AUXILIAR CONTROLES POR INTERRUPCION raton
void Personaje::mouseP(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		escopetazo();
	}
}
//dispara un escopetazo
void Personaje::escopetazo() {
	//cout << "ESCOPETAZO " << endl;
	for (int i = 0; i < 6; i++) {
		float ale = distribution(gen);
		float angle = (ale*pi / 6) - pi / 12;
		GLfloat punto1[3] = { pos[0],pos[1] + tam,pos[2] + ale / 10 };
		GLfloat punto1a[3] = { 0,0,0 };
		rotatePoint(pos, punto1, orientacion, punto1a);
		es->add(make_shared<Bala>(Bala(punto1a, orientacion + pi / 2 + angle)));
		//cout << "BALITA " << angle << endl;
	}
}
//dispara una bala
void Personaje::lanzarBala() {
	GLfloat punto1[3] = { pos[0],pos[1] + tam,pos[2] };
	GLfloat punto1a[3] = { 0,0,0 };
	rotatePoint(pos, punto1, orientacion, punto1a);
	es->add(make_shared<Bala>(Bala(punto1a, orientacion + pi / 2)));
	//cout << "BALITA" << endl;
}

void Personaje::setWindow(GLFWwindow* window) {
	this->window = window;
}

bool Personaje::getPulsado() {
	return GLFW_PRESS == glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) || GLFW_PRESS == glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
}

void Personaje::controlesInFrame() {
	int state = glfwGetKey(window, GLFW_KEY_W);
	if (state == GLFW_PRESS) {
		pos[1] += velocidad * 0.005;
	}
	state = glfwGetKey(window, GLFW_KEY_A);
	if (state == GLFW_PRESS) {
		pos[0] -= velocidad * 0.005;
	}
	state = glfwGetKey(window, GLFW_KEY_S);
	if (state == GLFW_PRESS) {
		pos[1] -= velocidad * 0.005;
	}
	state = glfwGetKey(window, GLFW_KEY_D);
	if (state == GLFW_PRESS) {
		pos[0] += velocidad * 0.005;
	}
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	int px, py;
	glfwGetWindowSize(window, &px, &py);

	//calula la posicion relativa de la raton en la pantalla
	x = (x / px - 0.5f) * 2;
	y = (abs(y - py) / py - 0.5f) * 2;
	//calcula la orientacion para que mire al raton
	float dirx = x - pos[0];
	float diry = y - pos[1];
	orientacion = atan2(-dirx, diry);
	//cout << orientacion << endl;
	//mira si se ha disparado y ha pasado el tiempo de cadencia
	state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	int stateR = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	if (state == GLFW_PRESS && (clock() - ultimaBala) / (CLOCKS_PER_SEC / 1000) > cadencia) {
		lanzarBala();
		ultimaBala = clock();
		escopeta = false;
	}
	else if (state == GLFW_PRESS) {
		escopeta = false;
	}
	else if (stateR == GLFW_PRESS && escopeta) {
		escopetazo();
		escopeta = false;
	}
	else if (stateR == GLFW_RELEASE) {
		escopeta = true;
	}
}

GLfloat Personaje::distancia(GLfloat x, GLfloat y, GLfloat xp, GLfloat yp) {
	return (x - xp)*(x - xp) + (y - yp)*(y - yp);
}

bool Personaje::sigueVivo() {
	int i = 0;
	vector<shared_ptr<CalaveraBase>> * cb = es->getCalaveras();
	while (i < cb->size()) {
		//cout << distancia(pos[0], pos[1], b[i].pos[0], b[i].pos[1]) << endl;
		if (distancia(pos[0], pos[1], cb->at(i)->pos[0], cb->at(i)->pos[1]) <= 3 * tam * tam) {
			vivo = false;
		}
		else {
			i++;
		}
	}
	return vivo;
}

//renderiza el personaje y las balas disparadas
bool Personaje::renderizar() {
	controlesInFrame();
	//calcula los puntos del triangulo segun la orientacion
	for (int i = 0; i < 4; i++) {
		GLfloat auxx = 1;
		if (i / 2 != 0) {
			auxx = -1;
		}
		GLfloat auxy = 1;
		if (i % 2 != 0) {
			auxy = -1;
		}
		GLfloat puntoaux[3] = { pos[0] + tam * auxx,pos[1] + tam * auxy,pos[2] };
		GLfloat punto1a[3] = { 0,0,0 };
		rotatePoint(pos, puntoaux, orientacion, punto1a);
		for (int j = 0; j < 3; j++) {
			vertices[i * 3 + j] = punto1a[j];
		}
	}

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

	return vivo;
}