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
#include <SOIL.h>

using namespace std;

const double pi = atan(1) * 4;

class Personaje {
	public: 
		//shaders
		
		// propiedades del personaje
		const GLfloat velocidad = 10;
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
			for (int i = 0; i < 4; i++) {
				GLfloat auxx = 1;
				if (i / 2 != 0) {
					auxx = -1;
				}
				GLfloat auxy = 1;
				if (i % 2 != 0) {
					auxy = -1;
				}
				GLfloat puntoaux[3] = { pos[0]+tam*auxx,pos[1]+tam*auxy,pos[2] };
				GLfloat punto1a[3] = { 0,0,0 };
				rotatePoint(pos, puntoaux, orientacion, punto1a);
				for (int j = 0; j < 3; j++) {
					vertices[i*3 + j] = punto1a[j];
				}
			}
			//VAO
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);
			//VERTICES
			// genera el buffer para vertices
			glGenBuffers(1, &points_VBO);
			// bindea el buffer
			glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
			// Llena el buffer con la informacion de los puntos
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			// EBO
			glGenBuffers(1, &EBO);
			// bindea ese buffer
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			// Lo rellena con la informacion de indices
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);

			//COLOR
			// Genera el buffer de colores
			glGenBuffers(1, &colors_VBO);
			// bindea ese buffer
			glBindBuffer(GL_ARRAY_BUFFER, colors_VBO);
			// Lo rellena con la informacion del color
			glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glBindVertexArray(0);

			//TEXTURA BUFFER
			glGenBuffers(1, &texCoords_VBO);
			// bindea ese buffer
			glBindBuffer(GL_ARRAY_BUFFER, texCoords_VBO);
			// Lo rellena con la informacion de las coordenadas
			glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

			//TEXTURA PROPIA
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				GL_LINEAR_MIPMAP_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWid, texHei, 0, GL_RGB,
				GL_UNSIGNED_BYTE, texImage);

			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
			SOIL_free_image_data(texImage);

			shaderProgram = LoadShaders("../DevilDaggers/videojuego/Codigo/Shaders/tri.vert", "../DevilDaggers/videojuego/Codigo/Shaders/tri.frag");
		}

		//CONSTRUCTOR EN UNA POSICION ESPECIFICA
		Personaje(GLfloat x, GLfloat y, GLfloat z) {
			pos[0] = x; pos[1] = y; pos[2] = z;
			Personaje();
		}

		void getPosition(GLfloat posi[]) {
			posi[0] = pos[0];
			posi[1] = pos[1];
			posi[2] = pos[2];
		}

		vector<Bala> getBalas() {
			return balas;
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
			int stateR = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
			if (state == GLFW_PRESS && (clock() - ultimaBala) / (CLOCKS_PER_SEC/1000) > cadencia) {
				lanzarBala();
				ultimaBala = clock();
				escopeta = false;
			}
			else if (state == GLFW_PRESS) {
				escopeta = false;
			}
			else if (stateR == GLFW_PRESS && escopeta) {
				escopetazo();
				escopeta = false;
			}
			else if (stateR == GLFW_RELEASE) {
				escopeta = true;
			}
		}
		
		//renderiza el personaje y las balas disparadas
		void renderizar() {
			//calcula los puntos del triangulo segun la orientacion
			for (int i = 0; i < 4; i++) {
				GLfloat auxx = 1;
				if (i / 2 != 0) {
					auxx = -1;
				}
				GLfloat auxy = 1;
				if (i % 2 != 0) {
					auxy = -1;
				}
				GLfloat puntoaux[3] = { pos[0] + tam * auxx,pos[1] + tam * auxy,pos[2] };
				GLfloat punto1a[3] = { 0,0,0 };
				rotatePoint(pos, puntoaux, orientacion, punto1a);
				for (int j = 0; j < 3; j++) {
					vertices[i*3 + j] = punto1a[j];
				}
			}

			glUseProgram(shaderProgram);
			
			glBindVertexArray(VAO); // une el VAO, que contiene toda la
									//información de los vértices, al contexto
			//vertices
			//argumento 0 posicion
			glEnableVertexAttribArray(0);
			//bindea el buffer
			glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
			//lo leena con los puntos
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(
				0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);
			// bindea ese buffer
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			// Lo rellena con la informacion de puntos
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
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
			//texure
			//argumento 2 textura
			glEnableVertexAttribArray(2);
			//bindea el buffer
			glBindBuffer(GL_ARRAY_BUFFER, texCoords_VBO);
			// le pasa el color de cada vertice
			glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
			glVertexAttribPointer(
				2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
				2,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
			);
			/*glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
				NULL); // en este caso el ID es 2 (0 y 1
					   // corresponden a las posiciones y colores)
			glEnableVertexAttribArray(2);*/
			//Dibuja los trinagulos (3 vertices)
			glBindTexture(GL_TEXTURE_2D, texture);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  // Starting from vertex 0; 3 vertices total -> 1 triangle
			glBindVertexArray(0); // deshace la unión del VAO

			//desactiva estos argumentos una vez usados
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);

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