#ifndef BALA_H_
#define BALA_H_

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cmath>
#include "Personaje.h"
#include <string>
#include <fstream>
#include <sstream>
#include "Herramientas.h"
#include <ctime>

class Bala {
	private:
		const GLchar* vertexShaderSource = leerStringFromFIle("../DevilDaggers/videojuego/Codigo/tri.vert").c_str();
		const GLchar* fragmentShaderSource = leerStringFromFIle("../DevilDaggers/videojuego/Codigo/tri.frag").c_str();

		GLfloat pos[3] = { 0,0,0 };
		const GLfloat tam = 0.01f;
		float orientacion = 0;
		float velocidad = 20;

		GLuint shaderProgram;
		GLuint VAOb;
		GLuint points_VBO = 0;
		GLuint colors_VBO = 0;
		
		clock_t inicio;

		int tiempoBala = 5;

		void rotatePoint(GLfloat centro[], GLfloat punto[], GLfloat angulo, GLfloat rot[]) {
			rot[0] = cos(angulo)*(punto[0] - centro[0]) - sin(angulo)*(punto[1] - centro[1]) + centro[0];
			rot[1] = sin(angulo)*(punto[0] - centro[0]) + cos(angulo)*(punto[1] - centro[1]) + centro[1];
			rot[2] = centro[2];
		}

	public:

		Bala(GLfloat inicio[], float o) {
			for (int i = 0; i < 3; i++) {
				pos[i] = inicio[i];
			}
			orientacion = o;

			glGenVertexArrays(1, &VAOb);
			glBindVertexArray(VAOb);

			glGenBuffers(1, &points_VBO);
			glGenBuffers(1, &colors_VBO);

			GLfloat punto1[3] = { pos[0],pos[1] + tam,pos[2] },
				punto2[3] = { pos[0] - tam,pos[1] - tam,pos[2] },
				punto3[3] = { pos[0] + tam,pos[1] - tam,pos[2] };
			GLfloat punto1a[3] = { 0,0,0 }, punto2a[3] = { 0,0,0 }, punto3a[3] = { 0,0,0 };
			rotatePoint(pos, punto1, orientacion, punto1a);
			rotatePoint(pos, punto2, orientacion, punto2a);
			rotatePoint(pos, punto3, orientacion, punto3a);
			GLfloat points[] = {
				punto1a[0], punto1a[1], punto1a[2],
				punto2a[0], punto2a[1], punto2a[2],
				punto3a[0], punto3a[1], punto3a[2]
			};

			glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
			glEnableVertexAttribArray(0);

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
			this->inicio = clock();
		}

		Bala operator=(const Bala& b) {
			if (this != &b) { // self-assignment check expected
				this->vertexShaderSource = leerStringFromFIle("../DevilDaggers/videojuego/Codigo/tri.vert").c_str();
				this->fragmentShaderSource = leerStringFromFIle("../DevilDaggers/videojuego/Codigo/tri.frag").c_str();

				for (int i = 0; i < 3; i++) {
					this->pos[i] = b.pos[i];
				}
				this->orientacion=b.orientacion;
				this->velocidad = b.velocidad;

				this->shaderProgram=b.shaderProgram;
				this->VAOb = b.VAOb;
				this->points_VBO = b.points_VBO;
				this->colors_VBO = b.colors_VBO;

				this->inicio=b.inicio;

				this->tiempoBala = b.tiempoBala;
			}
			return *this;
		}
		void avanzar() {
			pos[0] += (velocidad * 0.001f)*cos(orientacion);
			pos[1] += (velocidad * 0.001f)*sin(orientacion);
		}

		

		bool renderizar() {
			avanzar();
			GLfloat punto1[3] = { pos[0],pos[1] + tam,pos[2] },
				punto2[3] = { pos[0] - tam,pos[1] - tam,pos[2] },
				punto3[3] = { pos[0] + tam,pos[1] - tam,pos[2] };
			GLfloat punto1a[3] = { 0,0,0 }, punto2a[3] = { 0,0,0 }, punto3a[3] = { 0,0,0 };
			rotatePoint(pos, punto1, orientacion, punto1a);
			rotatePoint(pos, punto2, orientacion, punto2a);
			rotatePoint(pos, punto3, orientacion, punto3a);
			GLfloat points[] = {
				punto1a[0], punto1a[1], punto1a[2],
				punto2a[0], punto2a[1], punto2a[2],
				punto3a[0], punto3a[1], punto3a[2]
			};

			glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
			glEnableVertexAttribArray(0);

			glUseProgram(shaderProgram);
			glBindVertexArray(VAOb); // une el VAO, que contiene toda la información de los vértices, al contexto
			glDrawArrays(GL_TRIANGLES, 0, 3); /* dibuja los vértices del VBO/VAO
											  activo con el shader activo; el 1er
											  argumento es la primitiva a dibujar
											  (puntos, líneas, triángulos, etc.,
											  se pueden consultar en la
											  documentación12)*/
			glBindVertexArray(0); // deshace la unión del VAO

			return (clock()-inicio)/CLOCKS_PER_SEC < 5;

		}
		
};

#endif
