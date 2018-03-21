#ifndef PERSONAJE_H_
#define PERSONAJE_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>

#include <random>
#include <cmath>
#include <ctime>
#include <vector>
#include <stdlib.h>

using namespace std;

class Bala;

class Personaje {
	public: 
		// propiedades del personaje

		const double pi = atan(1) * 4;
		const GLfloat velocidad = 3.75;
		const GLfloat tam = 0.1f;
		GLfloat pos[3] = {0,0,0};
		GLfloat orientacion = pi/2;
		vector<Bala> balas;
		//Para renderizar
		GLuint shaderProgram;
		GLuint VAO;
		GLuint EBO;
		GLuint points_VBO;
		GLuint colors_VBO;
		//para disparar
		clock_t ultimaBala;
		float cadencia=50;
		float margenBala = 50;
		//para generar numeros aleatorios
		mt19937 gen;
		uniform_real_distribution<float> distribution;

		bool escopeta = true;

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
		unsigned char* texImage = SOIL_load_image("../DevilDaggers/videojuego/Codigo/CARETO.jpg", &texWid,
			&texHei, &texChan, SOIL_LOAD_RGB);
		//Rota el punto "punto" sobre centro "angulo" grados(RAD) y lo guarda en rot
		void rotatePoint(GLfloat centro[], GLfloat punto[], GLfloat angulo, GLfloat rot[]) {
			rot[0] = cos(angulo)*(punto[0] - centro[0]) - sin(angulo)*(punto[1] - centro[1]) + centro[0];
			rot[1] = sin(angulo)*(punto[0] - centro[0]) + cos(angulo)*(punto[1] - centro[1]) + centro[1];
			rot[2] = centro[2];
		}
	public:
		//CONSTRUCTOR POR DEFECTO
		Personaje();

		//CONSTRUCTOR EN UNA POSICION ESPECIFICA
		Personaje(GLfloat x, GLfloat y, GLfloat z);

		void getPosition(GLfloat posi[]);

		vector<Bala> * getBalas();

		void eliminarBala(int i);

		//DEVUELVE LA UNICA INSTANCIA DE PERSONAJE
		static Personaje& getInstance();

		//CONTROLES POR INTERRUPCION TECLAS
		static void controles(GLFWwindow* window, int key, int scancode, int action, int mods);

		//CONTROLES POR INTERRUPCION RATON
		static void mouse(GLFWwindow* window, int button, int action, int mods);
		//FUNCION AUXILIAR CONTROLES POR INTERRUPCION TECLAS
		void controlesP(GLFWwindow* window, int key, int scancode, int action, int mods);
		//FUNCION AUXILIAR CONTROLES POR INTERRUPCION raton
		void mouseP(GLFWwindow* window, int button, int action, int mods);
		//dispara un escopetazo
		void escopetazo();

		//dispara una bala
		void lanzarBala();

		void controlesInFrame(GLFWwindow* window);
		
		//renderiza el personaje y las balas disparadas
		bool renderizar();
};

#endif