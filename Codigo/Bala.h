#ifndef BALA_H_
#define BALA_H_

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>

#include "Personaje.h"
#include "LoadShader.h"

class Bala {
	private:
		//Propiedades de la bala
		GLfloat pos[3] = { 0,0,0 };
		const GLfloat tam = 0.01f;
		float orientacion = 0;
		float velocidad = 5;

		clock_t inicio;

		int tiempoBala = 5;
		//Para renderizar
		GLuint shaderProgram;
		GLuint points_VBO = 0;
		GLuint colors_VBO = 0;
		//Color de la bala
		GLfloat colors[9] = {
			1.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f
		};
		
		//Rota el punto "punto" sobre centro "angulo" grados(RAD) y lo guarda en rot
		void rotatePoint(GLfloat centro[], GLfloat punto[], GLfloat angulo, GLfloat rot[]) {
			rot[0] = cos(angulo)*(punto[0] - centro[0]) - sin(angulo)*(punto[1] - centro[1]) + centro[0];
			rot[1] = sin(angulo)*(punto[0] - centro[0]) + cos(angulo)*(punto[1] - centro[1]) + centro[1];
			rot[2] = centro[2];
		}

	public:
		//Genera una bala en la posicion inicio con orientacion o
		Bala(GLfloat inicio[], float o) {
			for (int i = 0; i < 3; i++) {
				pos[i] = inicio[i];
			}
			orientacion = o;
			//genera la primera posicion en funcion de la orientacion
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

			//VERTICES
			// genera el buffer para vertices
			glGenBuffers(1, &points_VBO);
			// bindea el buffer
			glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
			// Llena el buffer con la informacion de los puntos
			glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
			//COLOR
			// genera el buffer para el color
			glGenBuffers(1, &colors_VBO);
			//bindea el buffer
			glBindBuffer(GL_ARRAY_BUFFER, colors_VBO);
			//Llena el buffer con el color
			glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
			//carga los shaders
			shaderProgram = LoadShaders("../DevilDaggers/videojuego/Codigo/tri.vert", "../DevilDaggers/videojuego/Codigo/tri.frag");
			//guarda la marca temporal de inicio de la bala para borrarla pasado x tiempo
			this->inicio = clock();
		}
		// necesario para usar la clase vector, asignacion
		Bala operator=(const Bala& b) {
			if (this != &b) { // self-assignment check expected
				for (int i = 0; i < 3; i++) {
					this->pos[i] = b.pos[i];
				}
				this->orientacion=b.orientacion;
				this->velocidad = b.velocidad;
				for (int i = 0; i < 9; i++) {
					this->colors[i] = b.colors[i];
				}
				this->shaderProgram=b.shaderProgram;
				this->points_VBO = b.points_VBO;
				this->colors_VBO = b.colors_VBO;

				this->inicio=b.inicio;

				this->tiempoBala = b.tiempoBala;
			}
			return *this;
		}
		//Avanza la bala en la direccion de su orientacion a velocidad v
		void avanzar() {
			pos[0] += (velocidad * 0.001f)*cos(orientacion);
			pos[1] += (velocidad * 0.001f)*sin(orientacion);
		}

		//renderiza una bala y devuelve true si la bala debe seguir renderizandose (bala viva)
		bool renderizar() {
			//avanza cada frame
			avanzar();
			//calcula la posicion en funcion de la orientacion
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

			//usa los shaders
			glUseProgram(shaderProgram);
			//vertices
			//atributo 0 es la posicion
			glEnableVertexAttribArray(0);
			//bindea el buffer
			glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
			//lo leena con los puntos
			glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
			glVertexAttribPointer(
				0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);
			//color
			//color es el atributo 1
			glEnableVertexAttribArray(1);
			//bindea el buffer
			glBindBuffer(GL_ARRAY_BUFFER, colors_VBO);
			//lo llena con el color
			glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
			glVertexAttribPointer(
				1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
				3,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
			);
			// Pinta el triangulo(3 vertices)
			glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
			//desactiva los atributos
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			
			//devuelve true si a pasado menos tiempo que tiempo bala
			return (clock()-inicio)/CLOCKS_PER_SEC < tiempoBala;

		}
		
};

#endif
