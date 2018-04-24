#ifndef PARTICULAS_H_
#define PARTICULAS_H_



#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>

#include <iostream>
#include "Render3D.h"
#include <ctime>
#include <thread>
#include "../Camara.h"
#include <mutex>


using namespace std;

class SistemaParticulas {
	public:
		class Particula : public Render3D {
			public:
				float velocidad;
				mt19937 gen;
				uniform_real_distribution<float> distribution;

				Particula(glm::vec3 pos, GLFWwindow * window, Camara * c, Obj3D obj,glm::vec3 color) 
					: Render3D(window, "../DevilDaggers/videojuego/Codigo/Shaders/3D.vert", "../DevilDaggers/videojuego/Codigo/Shaders/3D.frag", c, obj,color,0.1){
					this->pos = pos;
					distribution = uniform_real_distribution<float>(-1, 1);
					random_device rd;
					// Initialize Mersenne Twister pseudo-random number generator
					gen = mt19937(rd());
					direccion = { distribution(gen),distribution(gen),distribution(gen) };
					direccion = direccion / length(direccion);
				}
				void ParticulaRender() {
					pos += direccion*0.25f;
					renderizar();
				}
				
		};
		
		int numParticulas;
		float velocidad;
		vector<Particula> parts;
		const float vida = 250;
		clock_t inicio;
		float inicialTam;

		SistemaParticulas(glm::vec3 pos, GLFWwindow * window, Camara * c, Obj3D obj, glm::vec3 color, int n) {
			parts = vector<Particula>();
			for (int i = 0; i < n; i++) {
				parts.push_back(Particula(pos, window, c, obj, color));
			}

			inicialTam = parts[0].tam;
			inicio = clock();
		}

		bool renderizar() {
			int t = (clock() - inicio)*1.0f / (CLOCKS_PER_SEC / 1000);
			for (int i = 0; i < parts.size(); i++) {
				parts[i].tam = inicialTam * (1-t / vida);
				parts[i].ParticulaRender();
			}
			return t < vida;
		}

};

#endif // !PARTICULAS_H_
