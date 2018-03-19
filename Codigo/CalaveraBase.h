#ifndef CALAVERA_BASE_H_
#define CALAVERA_BASE_H_

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <random>
#include <cmath>
#include <ctime>
#include <vector>
#include <stdlib.h>

#include "Bala.h"
#include "LoadShader.h"
#include "Personaje.h"
#include <SOIL.h>

using namespace std;

class CalaveraBase {
	private:
		//PROPIEDADES
		const GLfloat velocidad = 8;
		const GLfloat tam = 0.05f;
		GLfloat pos[3] = { 1,1,0 };
		GLfloat orientacion = pi / 2;
		//Para renderizar
		GLuint shaderProgram;
		GLuint VAO;
		GLuint EBO;
		GLuint points_VBO;
		GLuint colors_VBO;

		int vida = 20;
		bool muerto = false;

		//direccion para seguir al personaje
		GLfloat dir[3] = { 0,0,0 };
		//textura
		GLfloat colors[12] = {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f
		};
		GLfloat texCoords[8] = {
			0.0f, 0.0f,
			1.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f

		};
		GLuint texCoords_VBO;
		GLuint texture;
		GLfloat vertices[12] = {
			0.5f, 0.5f, 0.0f, // Arriba dcha
			0.5f, -0.5f, 0.0f, // Abajo dcha
			-0.5f, -0.5f, 0.0f, // Abajo izqda
			-0.5f, 0.5f, 0.0f // Arriba izqda
		};
		GLuint indices[6] = {
			0, 1, 2, // Tri�ngulo #1
			1, 3, 2 // Tri�ngulo #2
		};

		int texWid, texHei, texChan;
		unsigned char* texImage = SOIL_load_image("../DevilDaggers/videojuego/Codigo/skull.png", &texWid,
			&texHei, &texChan, SOIL_LOAD_RGB);
		//Rota el punto "punto" sobre centro "angulo" grados(RAD) y lo guarda en rot
		void rotatePoint(GLfloat centro[], GLfloat punto[], GLfloat angulo, GLfloat rot[]) {
			rot[0] = cos(angulo)*(punto[0] - centro[0]) - sin(angulo)*(punto[1] - centro[1]) + centro[0];
			rot[1] = sin(angulo)*(punto[0] - centro[0]) + cos(angulo)*(punto[1] - centro[1]) + centro[1];
			rot[2] = centro[2];
		}
	public:
		CalaveraBase() {
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

			glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				GL_LINEAR_MIPMAP_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWid, texHei, 0, GL_RGB,
				GL_UNSIGNED_BYTE, texImage);

			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
			SOIL_free_image_data(texImage);

			shaderProgram = LoadShaders("../DevilDaggers/videojuego/Codigo/Shaders/calavera.vert", "../DevilDaggers/videojuego/Codigo/Shaders/calavera.frag");
		}
		void seguir() {
			Personaje a = Personaje::getInstance();
			GLfloat posP[] = { 0,0,0 };
			a.getPosition(posP);
			GLfloat dirx = posP[0] - pos[0], diry = posP[1] - pos[1];
			GLfloat mod = sqrt((dirx*dirx) + (diry*diry));
			dirx /= mod;
			diry /= mod;
			pos[0] += dirx * 0.001f * velocidad;
			pos[1] += diry * 0.001f * velocidad;
		}
		GLfloat distancia(GLfloat x, GLfloat y, GLfloat xp, GLfloat yp) {
			return (x - xp)*(x - xp) + (y - yp)*(y - yp);
		}
		bool vivo() {
			Personaje a = Personaje::getInstance();
			vector<Bala> b = a.getBalas();
			for (int i = 0; i < b.size(); i++) {
				cout << distancia(pos[0], pos[1], b[i].pos[0], b[i].pos[1]) << endl;
				if (distancia(pos[0], pos[1], b[i].pos[0], b[i].pos[1]) <= 3*tam * tam) {
					vida -= b[i].danyo;
				}
			}
			return vida>0;
		}

		bool renderizar() {
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
									//informaci�n de los v�rtices, al contexto
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
			glBindVertexArray(0); // deshace la uni�n del VAO

									//desactiva estos argumentos una vez usados
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
			return !muerto;
		}
};

#endif;