#ifndef PERSONAJE_H_
#define PERSONAJE_H_

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <random>
#include <cmath>
#include <ctime>
#include <vector>
#include <stdlib.h>

#include "Bala.h"
#include "Herramientas.h"

using namespace std;

const float pi = atan(1) * 4;

class Personaje {
	private: 
		//shaders
		const GLchar* vertexShaderSource = leerStringFromFIle("../DevilDaggers/videojuego/Codigo/tri.vert").c_str();
		const GLchar* fragmentShaderSource = leerStringFromFIle("../DevilDaggers/videojuego/Codigo/tri.frag").c_str();
		// propiedades del personaje
		const GLfloat velocidad = 10;
		const GLfloat tam = 0.1f;
		const GLfloat color[3] = { 1,0,0 };
		GLfloat pos[3] = {0,0,0};
		GLfloat orientacion = 0;
		vector<Bala> balas;
		//Para renderizar
		GLuint shaderProgram;
		GLuint VAO;
		GLuint points_VBO = 0;
		GLuint colors_VBO = 0;
		//para disparar
		clock_t ultimaBala;
		float cadencia=50;
		float margenBala = 50;
		//para generar numeros aleatorios
		mt19937 gen;
		uniform_real_distribution<float> distribution;

		void rotatePoint(GLfloat centro[], GLfloat punto[], GLfloat angulo, GLfloat rot[]) {
			rot[0] = cos(angulo)*(punto[0] - centro[0]) - sin(angulo)*(punto[1] - centro[1]) + centro[0];
			rot[1] = sin(angulo)*(punto[0] - centro[0]) + cos(angulo)*(punto[1] - centro[1]) + centro[1];
			rot[2] = centro[2];
		}
	public:
		Personaje() {
			ultimaBala = clock();
			balas = vector<Bala>();
			distribution = uniform_real_distribution<float>(0, 1);
			random_device rd;
			// Initialize Mersenne Twister pseudo-random number generator
			gen = mt19937(rd());

			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

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
		}

		Personaje(GLfloat x, GLfloat y, GLfloat z) {
			pos[0] = x; pos[1] = y; pos[2] = z;
			Personaje();
		}

		static Personaje& getInstance() // Singleton is accessed via getInstance()
		{
			static Personaje instance; // lazy singleton, instantiated on first use
			return instance;
		}

		static void controles(GLFWwindow* window, int key, int scancode, int action, int mods) {
			getInstance().controlesP(window, key, scancode, action, mods);
		}

		static void mouse(GLFWwindow* window, int button, int action, int mods){
			getInstance().mouseP(window, button, action, mods);
		}

		void controlesP(GLFWwindow* window, int key, int scancode, int action, int mods) {
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			}
			/*else if (key == GLFW_KEY_W && state==GLFW_PRESS) {
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
			}*/

		}

		void escopetazo() {
			for (int i = 0; i < 6; i++) {
				srand(time(0) + ultimaBala +i*50);
				float ale = distribution(gen);
				float angle = (ale*pi / 6) - pi / 12;
				GLfloat punto1[3] = { pos[0],pos[1] + tam,pos[2] + ale / 10 };
				GLfloat punto1a[3] = { 0,0,0 };
				rotatePoint(pos, punto1, orientacion, punto1a);
				balas.push_back(Bala(punto1a, orientacion + pi / 2+angle));
				cout << "BALITA " << angle << endl;
			}
		}

		void lanzarBala() {
			GLfloat punto1[3] = { pos[0],pos[1] + tam,pos[2] };
			GLfloat punto1a[3] = { 0,0,0 };
			rotatePoint(pos, punto1, orientacion, punto1a);
			balas.push_back(Bala(punto1a, orientacion + pi / 2));
			cout << "BALITA" << endl;
		}
		void mouseP(GLFWwindow* window, int button, int action, int mods) {
			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
				escopetazo();
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
			/*cout << "PANTALLA" << endl;
			cout << x << " x " << y << endl;
			cout << "NAVE" << endl;
			cout << pos[0] << " x " << pos[1] << endl;*/
			//Mathf.Atan2(-dir.x, dir.y)
			float dirx = x - pos[0];
			float diry = y - pos[1];
			orientacion = atan2(-dirx,diry);
			/*state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
			if (state == GLFW_PRESS && (clock() - ultimaBala) / (CLOCKS_PER_SEC/1000) > cadencia) {
				lanzarBala();
				ultimaBala = clock();
			}*/
		}
		
		void renderizar() {
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

			/*glBindBuffer(GL_ARRAY_BUFFER, colors_VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
			glEnableVertexAttribArray(1);*/
			glBindVertexArray(0);

			glUseProgram(shaderProgram);
			glBindVertexArray(VAO); // une el VAO, que contiene toda la información de los vértices, al contexto
			glDrawArrays(GL_TRIANGLES, 0, 3); /* dibuja los vértices del VBO/VAO
											  activo con el shader activo; el 1er
											  argumento es la primitiva a dibujar
											  (puntos, líneas, triángulos, etc.,
											  se pueden consultar en la
											  documentación12)*/
			glBindVertexArray(0); // deshace la unión del VAO
			int matar = -1;
			for (int i = 0; i < balas.size(); i++) {
				bool muerto = balas[i].renderizar();
				if (!muerto) {
					matar = i;
				}
			}
			if (matar >= 0) {
				cout << "ELIMINADO " << balas.size() << endl;
				balas.erase(balas.begin() + matar);
			}
			
		}
};

#endif