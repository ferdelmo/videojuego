#ifndef FONDO_H_
#define FONDO_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>

#include <random>
#include <cmath>
#include "LoadShader.h"
#include "Renderizable.h"

using namespace std;

class Fondo : public Renderizable {
	public:
		Fondo(GLFWwindow * window, string imgFondo, GLfloat tam, GLfloat offset, Camara * c)
			: Renderizable(window, imgFondo, "../DevilDaggers/videojuego/Codigo/Shaders/fondo.vert", "../DevilDaggers/videojuego/Codigo/Shaders/fondo.frag", tam, offset,c) {
		
		}

		Fondo(GLfloat x, GLfloat y, GLfloat z, GLFWwindow * window, string imgFondo, GLfloat tam, GLfloat offset, Camara * c)
			: Renderizable(window, imgFondo, "../DevilDaggers/videojuego/Codigo/Shaders/fondo.vert", "../DevilDaggers/videojuego/Codigo/Shaders/fondo.frag", tam, offset, c) {
			this->pos[0] = x;
			this->pos[1] = y;
			this->pos[2] = z;
		}

			
};

#endif // !FONDO_H_

