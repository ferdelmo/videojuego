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
#include "LoadShader.h"

using namespace std;

const double pi = atan(1) * 4;

class Personaje {
	private: 
		//shaders
		
		// propiedades del personaje
		const GLfloat velocidad = 2.5f;
		const GLfloat tam = 0.1f;
		const GLfloat color[3] = { 1,0,0 };
		GLfloat pos[3] = {0,0,0};
		GLfloat orientacion = 0;
		vector<Bala> balas;
		//Para renderizar
		GLuint shaderProgram;
		GLuint points_VBO;
		GLuint colors_VBO;
		//para disparar
		clock_t ultimaBala;
		float cadencia=50;
		float margenBala = 50;
		//para generar numeros aleatorios
		mt19937 gen;
		uniform_real_distribution<float> distribution;


		GLfloat colors[9] = {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		};
		//Rota el punto "punto" sobre centro "angulo" grados(RAD) y lo guarda en rot
		void rotatePoint(GLfloat centro[], GLfloat punto[], GLfloat angulo, GLfloat rot[]) {
			rot[0] = cos(angulo)*(punto[0] - centro[0]) - sin(angulo)*(punto[1] - centro[1]) + centro[0];
			rot[1] = sin(angulo)*(punto[0] - centro[0]) + cos(angulo)*(punto[1] - centro[1]) + centro[1];
			rot[2] = centro[2];
		}
	public:
		//CONSTRUCTOR POR DEFECTO
		Personaje() {
			//prepara las balas
			ultimaBala = clock();
			balas = vector<Bala>();
			//numeros aleatorios
			distribution = uniform_real_distribution<float>(0, 1);
			random_device rd;
			// Initialize Mersenne Twister pseudo-random number generator
			gen = mt19937(rd());

			//calcula los puntos del triangulo segun la orientacion
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
			// Genera el buffer de los puntos
			glGenBuffers(1, &points_VBO);
			// bindea ese buffer
			glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
			// Lo rellena con la informacion de puntos
			glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
			
			//COLOR
			// Genera el buffer de colores
			glGenBuffers(1, &colors_VBO);
			// bindea ese buffer
			glBindBuffer(GL_ARRAY_BUFFER, colors_VBO);
			// Lo rellena con la informacion del color
			glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

			shaderProgram = LoadShaders("../DevilDaggers/videojuego/Codigo/tri.vert", "../DevilDaggers/videojuego/Codigo/tri.frag");
		}

		//CONSTRUCTOR EN UNA POSICION ESPECIFICA
		Personaje(GLfloat x, GLfloat y, GLfloat z) {
			pos[0] = x; pos[1] = y; pos[2] = z;
			Personaje();
		}

		//DEVUELVE LA UNICA INSTANCIA DE PERSONAJE
		static Personaje& getInstance() // Singleton is accessed via getInstance()
		{
			static Personaje instance; // lazy singleton, instantiated on first use
			return instance;
		}

		//CONTROLES POR INTERRUPCION TECLAS
		static void controles(GLFWwindow* window, int key, int scancode, int action, int mods) {
			getInstance().controlesP(window, key, scancode, action, mods);
		}

		//CONTROLES POR INTERRUPCION RATON
		static void mouse(GLFWwindow* window, int button, int action, int mods){
			getInstance().mouseP(window, button, action, mods);
		}
		//FUNCION AUXILIAR CONTROLES POR INTERRUPCION TECLAS
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
		//FUNCION AUXILIAR CONTROLES POR INTERRUPCION raton
		void mouseP(GLFWwindow* window, int button, int action, int mods) {
			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
				escopetazo();
			}
		}
		//dispara un escopetazo
		void escopetazo() {
			for (int i = 0; i < 6; i++) {
				float ale = distribution(gen);
				float angle = (ale*pi / 6) - pi / 12;
				GLfloat punto1[3] = { pos[0],pos[1] + tam,pos[2] + ale / 10 };
				GLfloat punto1a[3] = { 0,0,0 };
				rotatePoint(pos, punto1, orientacion, punto1a);
				balas.push_back(Bala(punto1a, orientacion + pi / 2+angle));
				cout << "BALITA " << angle << endl;
			}
		}
		//dispara una bala
		void lanzarBala() {
			GLfloat punto1[3] = { pos[0],pos[1] + tam,pos[2] };
			GLfloat punto1a[3] = { 0,0,0 };
			rotatePoint(pos, punto1, orientacion, punto1a);
			balas.push_back(Bala(punto1a, orientacion + pi / 2));
			cout << "BALITA" << endl;
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

			//calula la posicion relativa de la raton en la pantalla
			x = (x / px - 0.5f) * 2;
			y = (abs(y-py) / py - 0.5f) * 2;
			//calcula la orientacion para que mire al raton
			float dirx = x - pos[0];
			float diry = y - pos[1];
			orientacion = atan2(-dirx,diry);
			//mira si se ha disparado y ha pasado el tiempo de cadencia
			state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
			if (state == GLFW_PRESS && (clock() - ultimaBala) / (CLOCKS_PER_SEC/1000) > cadencia) {
				lanzarBala();
				ultimaBala = clock();
			}
		}
		
		//renderiza el personaje y las balas disparadas
		void renderizar() {
			//calcula los puntos del triangulo en funcion de la orientacion
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
			GLfloat colors[] = {
				1.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 1.0f
			};

			

			glUseProgram(shaderProgram);
			//vertices
			//argumento 0 posicion
			glEnableVertexAttribArray(0);
			//bindea el buffer
			glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
			//le pasa los vertices
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
			//argumento 1 color
			glEnableVertexAttribArray(1);
			//bindea el buffer
			glBindBuffer(GL_ARRAY_BUFFER, colors_VBO);
			// le pasa el color de cada vertice
			glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
			glVertexAttribPointer(
				1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
				3,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
			);
			//Dibuja los trinagulos (3 vertices)
			glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
			//desactiva estos argumentos una vez usados
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);

			int matar = -1;
			// recorre la lista de balas disparadas renderizandolas
			for (int i = 0; i < balas.size(); i++) {
				//si la bala se destruye la borra del vector
				bool nomuerto = balas[i].renderizar();
				if (!nomuerto) {
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