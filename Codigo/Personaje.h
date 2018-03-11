#ifndef PERSONAJE_H_
#define PERSONAJE_H_

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cmath>

using namespace std;

const float pi = atan(1) * 4;

class Personaje {
	private: 
		const GLfloat velocidad = 2.5f;
		const GLfloat tam = 0.1f;
		const GLfloat color[3] = { 1,0,0 };
		GLfloat pos[3] = {0,0,0};
		GLfloat orientacion = 0;
		bool holi = true;
		void rotatePoint(GLfloat centro[], GLfloat punto[], GLfloat angulo, GLfloat rot[]) {
			rot[0] = cos(angulo)*(punto[0] - centro[0]) - sin(angulo)*(punto[1]-centro[1]) + centro[0];
			rot[1] = sin(angulo)*(punto[0] - centro[0]) + cos(angulo)*(punto[1]-centro[1]) + centro[1];
			rot[2] = centro[2];
			if (holi) {
				cout << rot[0] << ", " << rot[1] << ", " << rot[2] << endl;
			}
		}
	public:
		Personaje() {}
		Personaje(GLfloat x, GLfloat y, GLfloat z) {}

		static Personaje& getInstance() // Singleton is accessed via getInstance()
		{
			static Personaje instance; // lazy singleton, instantiated on first use
			return instance;
		}

		static void controles(GLFWwindow* window, int key, int scancode, int action, int mods) {
			getInstance().controlesP(window, key, scancode, action, mods);
		}

		void controlesP(GLFWwindow* window, int key, int scancode, int action, int mods) {
			int state = glfwGetKey(window, key);
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			}
			else if (key == GLFW_KEY_W && state==GLFW_PRESS) {
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
			}
		}

		void controlesInFrame(GLFWwindow* window) {
			int state = glfwGetKey(window, GLFW_KEY_W);
			if (state == GLFW_PRESS) {
				pos[1] += velocidad * 0.001f;
			}
			state = glfwGetKey(window, GLFW_KEY_A);
			if (state == GLFW_PRESS) {
				pos[0] -= velocidad * 0.001f;
			}
			state = glfwGetKey(window, GLFW_KEY_S);
			if (state == GLFW_PRESS) {
				pos[1] -= velocidad * 0.001f;
			}
			state = glfwGetKey(window, GLFW_KEY_D);
			if (state == GLFW_PRESS) {
				pos[0] += velocidad * 0.001f;
			}
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			int px, py;
			glfwGetWindowSize(window, &px, &py);

			x = (x / px - 0.5f) * 2;
			y = (abs(y-py) / py - 0.5f) * 2;
			cout << "PANTALLA" << endl;
			cout << x << " x " << y << endl;
			cout << "NAVE" << endl;
			cout << pos[0] << " x " << pos[1] << endl;
			//Mathf.Atan2(-dir.x, dir.y)
			float dirx = x - pos[0];
			float diry = y - pos[1];
			orientacion = atan2(-dirx,diry);
		}
		
		void renderizar(GLuint points_VBO, GLuint colors_VBO) {
			GLfloat cosenito = cos(orientacion);
			GLfloat senito = sin(orientacion);
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
			holi = false;
			glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
			glEnableVertexAttribArray(0);
		}
};

#endif