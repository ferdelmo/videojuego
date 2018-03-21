#include "CalaveraBase.h"

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
#include "Personaje.h"
#include "Bala.h"
#include "Escena.h"

using namespace std;

CalaveraBase::CalaveraBase(Escena * es) {
	this->es = es;
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
	//numeros aleatorios
	distribution = uniform_real_distribution<float>(-1, 1);
	random_device rd;
	// Initialize Mersenne Twister pseudo-random number generator
	gen = mt19937(rd());

	dir[0] = distribution(gen);
	dir[1] = distribution(gen);

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

	shaderProgram = LoadShaders("../DevilDaggers/videojuego/Codigo/Shaders/calavera.vert", "../DevilDaggers/videojuego/Codigo/Shaders/calavera.frag");
}

CalaveraBase::CalaveraBase(GLfloat x, GLfloat y, GLfloat z, Escena * es) {
	pos[0] = x; pos[1] = y; pos[2] = z;
	this->es = es;
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
	//numeros aleatorios
	distribution = uniform_real_distribution<float>(-1, 1);
	random_device rd;
	// Initialize Mersenne Twister pseudo-random number generator
	gen = mt19937(rd());

	dir[0] = distribution(gen);
	dir[1] = distribution(gen);

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

	shaderProgram = LoadShaders("../DevilDaggers/videojuego/Codigo/Shaders/calavera.vert", "../DevilDaggers/videojuego/Codigo/Shaders/calavera.frag");
}

void CalaveraBase::seguir() {
	shared_ptr<Personaje> a= es->getPer();
	GLfloat posP[] = { 0,0,0 };
	a->getPosition(posP);
	GLfloat dirx = posP[0] - pos[0], diry = posP[1] - pos[1];
	GLfloat antigua = orientacion;
	orientacion = atan2(diry, dirx);
	float alpha = distribution(gen);
	//orientacion += alpha * pi / 6;
	if (orientacion > antigua) {
		orientacion = antigua + velRot;
	}
	else {
		orientacion = antigua - velRot;
	}
	pos[0] += (0.005 * velocidad)*cos(orientacion);
	pos[1] += (0.005 * velocidad)*sin(orientacion);
}
GLfloat CalaveraBase::distancia(GLfloat x, GLfloat y, GLfloat xp, GLfloat yp) {
	return (x - xp)*(x - xp) + (y - yp)*(y - yp);
}
bool CalaveraBase::vivo() {
	shared_ptr<Personaje> a = es->getPer();
	vector<shared_ptr<Bala>> * b = es->getBalas();
	int i = 0;
	while (i < b->size()) {
		//cout << distancia(pos[0], pos[1], b[i].pos[0], b[i].pos[1]) << endl;
		if (distancia(pos[0], pos[1], b->at(i)->pos[0], b->at(i)->pos[1]) <= 3 * tam * tam) {
			vida -= b->at(i)->danyo;
			b->erase(b->begin() + i);
			cout << vida << endl;
			cout << "ELIMINADO " << b->size() << endl;
		}
		else {
			i++;
		}
	}
	return vida>0;
}
CalaveraBase CalaveraBase::operator=(const CalaveraBase& b) {
	if (this != &b) { // self-assignment check expected
		for (int i = 0; i < 3; i++) {
			this->pos[i] = b.pos[i];
		}
		this->orientacion = b.orientacion;
		this->velocidad = b.velocidad;
		for (int i = 0; i < 12; i++) {
			this->colors[i] = b.colors[i];
		}
		this->shaderProgram = b.shaderProgram;
		this->points_VBO = b.points_VBO;
		this->colors_VBO = b.colors_VBO;
		this->vida = b.vida;
		this->VAO = b.VAO;
		this->EBO = EBO;
		this->muerto = b.muerto;


		for (int i = 0; i < 3; i++) {
			this->dir[i] = b.dir[i];
		}
		for (int i = 0; i < 8; i++) {
			this->texCoords[i] = b.texCoords[i];
		}
		this->texCoords_VBO = b.texCoords_VBO;
		this->texture = b.texture;
		for (int i = 0; i < 12; i++) {
			this->vertices[i] = b.vertices[i];
		}
		for (int i = 0; i < 6; i++) {
			this->indices[i] = b.indices[i];
		}
		this->texChan = b.texChan, this->texHei = b.texHei, this->texChan = b.texChan;
		this->texImage = b.texImage;
	}
	return *this;
}
bool CalaveraBase::renderizar() {
	seguir();
	muerto = !vivo();
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
	return !muerto;
}