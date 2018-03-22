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
		Fondo(GLFWwindow * window, string imgFondo, GLfloat tam)
			: Renderizable(window, imgFondo, "../DevilDaggers/videojuego/Codigo/Shaders/fondo.vert", "../DevilDaggers/videojuego/Codigo/Shaders/fondo.frag", tam,5) {
		
		}
			
};

#endif // !FONDO_H_

