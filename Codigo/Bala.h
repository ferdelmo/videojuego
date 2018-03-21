#ifndef BALA_H_
#define BALA_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <ctime>

using namespace std;

class Bala {
	public:
		//Propiedades de la bala
		GLfloat pos[3] = { 0,0,0 };
		const GLfloat tam = 0.01f;
		float orientacion = 0;
		float velocidad = 7.5;

		int danyo = 5;
		clock_t inicio;

		int tiempoBala = 3;
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

		void rotatePoint(GLfloat centro[], GLfloat punto[], GLfloat angulo, GLfloat rot[]);

		Bala(GLfloat inicio[], float o);

		Bala operator=(const Bala& b);

		void avanzar();

		bool renderizar();
};


#endif 
