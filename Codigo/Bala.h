#ifndef BALA_H_
#define BALA_H_

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cmath>

class Bala {
	private:
		GLfloat pos[3] = { 0,0,0 };
		const GLfloat tam = 0.01f;
		float orientacion = 0;
		float velocidad = 2;

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
		}
		void avanzar() {
			pos[0] += (velocidad * 0.001f)*cos(orientacion);
			pos[1] += (velocidad * 0.001f)*sin(orientacion);
		}
		void renderizar(GLuint points_VBO) {
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

		}
		
};

#endif
