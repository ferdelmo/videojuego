#include "Daga.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <random>
#include <cmath>
#include <ctime>
#include <vector>
#include <stdlib.h>

#include "CalaveraBase.h"
#include "LoadShader.h"
#include "Escena.h"
#include "Gema.h"
#include <SOIL.h>

using namespace std;

Daga::Daga(Escena * es, int numGemas) {
	this->es = es;
	gemas = vector<shared_ptr<Gema>>();
	//añadir las gemas necesarias
	GLfloat div = 2 * pi / numGemas; //para colocarlas alrededor de la torre
	for (int i = 0; i < numGemas; i++) {
		GLfloat posIni[3] = { pos[0]+tam*cos(i*div), pos[1]+tam*sin(i*div), pos[2] };
		shared_ptr<Gema> sg = make_shared<Gema>(Gema(posIni[0], posIni[1], posIni[2],es));
		shared_ptr<Gema> sg1(sg);
		gemas.push_back(sg);
		es->add(sg1);
	}

	//numeros aleatorios
	distribution = uniform_real_distribution<float>(-1, 1);
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
	ultimaGen = clock();
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

	shaderProgram = LoadShaders("../DevilDaggers/videojuego/Codigo/Shaders/daga.vert", "../DevilDaggers/videojuego/Codigo/Shaders/daga.frag");
}

void Daga::GenerarCalaveras(int n) {
	for (int i = 0; i < n; i++) {
		float x = distribution(gen);
		float y = distribution(gen);
		es->add(make_shared<CalaveraBase>(CalaveraBase(pos[0] + x * tam, pos[1] + y * tam, pos[2],es)));
	}
}

bool Daga::sigueVivo() {
	int i = 0;
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

bool Daga::renderizar() {
	if ((clock() - ultimaGen) / CLOCKS_PER_SEC > tiempoGen && lanzar) {
		GenerarCalaveras(6);
		ultimaGen = clock();
		lanzar = false;
	}
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

	return sigueVivo();
}