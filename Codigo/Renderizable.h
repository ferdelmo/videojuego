#ifndef RENDERIZABLE_H_
#define RENDERIZABLE_H_


#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>
#include <SOIL.h>


using namespace std;

class Camara;

class Renderizable {
	public:
		// propiedades del personaje

		const double pi = atan(1) * 4;
		GLfloat tam = 0.1f;
		GLfloat pos[3] = { 0,0,0 };
		GLfloat orientacion = pi / 2;
		//Para renderizar
		GLuint shaderProgram;
		GLuint VAO;
		GLuint EBO;
		GLuint points_VBO;
		GLuint colors_VBO;

		//textura
		GLfloat colors[12] = {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f
		};
		GLfloat texCoords[8] = {
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,

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
			0, 1, 2, // Triángulo #1
			1, 3, 2 // Triángulo #2
		};
		int texWid, texHei, texChan;
		unsigned char* texImage;

		GLFWwindow* window;

		GLuint MatrixID;
		//Rota el punto "punto" sobre centro "angulo" grados(RAD) y lo guarda en rot
		void rotatePoint(GLfloat centro[], GLfloat punto[], GLfloat angulo, GLfloat rot[]) {
			rot[0] = cos(angulo)*(punto[0] - centro[0]) - sin(angulo)*(punto[1] - centro[1]) + centro[0];
			rot[1] = sin(angulo)*(punto[0] - centro[0]) + cos(angulo)*(punto[1] - centro[1]) + centro[1];
			rot[2] = centro[2];
		}
		
		bool sigue=true;

		Camara * cam;

	public:
		//CONSTRUCTOR POR DEFECTO
		Renderizable(GLFWwindow * window,string textura, string vertSha, string fragSha, GLfloat tam, Camara * c);

		//CONSTRUCTOR POR DEFECTO
		Renderizable(GLFWwindow * window, string textura, string vertSha, string fragSha, GLfloat tam, GLfloat offset, Camara * c);

		Renderizable(GLFWwindow * window, string textura, GLfloat tam, Camara * c, GLuint sha);

		Renderizable operator=(const Renderizable& b);
		//Renderiza
		bool renderizar();
		//mover
		virtual void mover();

		//mover
		virtual void fisicas();

		//calcula la distancia^2 entre {x,y} y {xp,yp}
		GLfloat distancia(GLfloat x, GLfloat y, GLfloat xp, GLfloat yp);
};

#endif // !RENDERIZABLE_H_
