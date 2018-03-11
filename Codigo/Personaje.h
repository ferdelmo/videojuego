#ifndef STOKES_H_
#define STOKES_H_

#define GLEW_STATIC
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace std;

class Personaje {
	private: 
		GLfloat pos[3] = {0,0,0};
		void controles(GLFWwindow* window, int key, int scancode, int action, int mods) {
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			}
			else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
				for (int i = 0; i < 3; i++) {
					pos[1] += 0.25;
				}
			}
			else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
				for (int i = 0; i < 3; i++) {
					pos[0] -= 0.25;
				}
			}
			else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
				for (int i = 0; i < 3; i++) {
					pos[1] -= 0.25;
				}
			}
			else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
				for (int i = 0; i < 3; i++) {
					pos[1] += 0.25;
				}
			}
		}
	public:
		Personaje() {}
		Personaje(GLfloat x, GLfloat y, GLfloat z) {}
		void inicializarcontroles(GLFWwindow* window) {
			glfwSetKeyCallback(window, controles);
		}

};

#endif