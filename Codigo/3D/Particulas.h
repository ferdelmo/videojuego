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
		vector<glm::vec3> verts = { { 1, 1, -1 } ,
								{ 1, -1, -1 } ,
								{ -1, -1, -1 } ,
								{ 1, 1, -1 } ,
								{ -1, -1, -1 } ,
								{ -1, 1, -1 } ,
								{ -1, -1, 1 } ,
								{ -1, 1, 1 } ,
								{ -1, 1, -1 } ,
								{ -1, -1, 1 } ,
								{ -1, 1, -1 } ,
								{ -1, -1, -1 } ,
								{ 1, -1, 1 } ,
								{ 0.999999, 1, 1 } ,
								{ -1, -1, 1 } ,
								{ 0.999999, 1, 1 } ,
								{ -1, 1, 1 } ,
								{ -1, -1, 1 } ,
								{ 1, -1, -1 } ,
								{ 1, 1, -1 } ,
								{ 1, -1, 1 } ,
								{ 1, 1, -1 } ,
								{ 0.999999, 1, 1 } ,
								{ 1, -1, 1 } ,
								{ 1, 1, -1 } ,
								{ -1, 1, -1 } ,
								{ 0.999999, 1, 1 } ,
								{ -1, 1, -1 } ,
								{ -1, 1, 1 } ,
								{ 0.999999, 1, 1 } ,
								{ 1, -1, -1 } ,
								{ 1, -1, 1 } ,
								{ -1, -1, 1 } ,
								{ 1, -1, -1 } ,
								{ -1, -1, 1 } ,
								{ -1, -1, -1 } };
		vector<glm::vec2> uvs = {};
		vector<glm::vec3> normals = { { 0, 0, -1 } ,
							{ 0, 0, -1 } ,
							{ 0, 0, -1 } ,
							{ 0, 0, -1 } ,
							{ 0, 0, -1 } ,
							{ 0, 0, -1 } ,
							{ -1, -0, -0 } ,
							{ -1, -0, -0 } ,
							{ -1, -0, -0 } ,
							{ -1, -0, -0 } ,
							{ -1, -0, -0 } ,
							{ -1, -0, -0 } ,
							{ -0, -0, 1 } ,
							{ -0, -0, 1 } ,
							{ -0, -0, 1 } ,
							{ -1e-06, 0, 1 } ,
							{ -1e-06, 0, 1 } ,
							{ -1e-06, 0, 1 } ,
							{ 1, -0, 0 } ,
							{ 1, -0, 0 } ,
							{ 1, -0, 0 } ,
							{ 1, 0, 1e-06 } ,
							{ 1, 0, 1e-06 } ,
							{ 1, 0, 1e-06 } ,
							{ 0, 1, -0 } ,
							{ 0, 1, -0 } ,
							{ 0, 1, -0 } ,
							{ 0, 1, -0 } ,
							{ 0, 1, -0 } ,
							{ 0, 1, -0 } ,
							{ -0, -1, 0 } ,
							{ -0, -1, 0 } ,
							{ -0, -1, 0 } ,
							{ -0, -1, 0 } ,
							{ -0, -1, 0 } ,
							{ -0, -1, 0 } };
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
		Obj3D modelo;

		SistemaParticulas(glm::vec3 pos, GLFWwindow * window, Camara * c, glm::vec3 color, int n) {
			parts = vector<Particula>();
			modelo.vertices = verts;
			modelo.normals = normals;
			modelo.uvs = uvs;
			for (int i = 0; i < n; i++) {
				parts.push_back(Particula(pos, window, c, modelo, color));
			}

			inicialTam = parts[0].tam;
			inicio = clock();
		}
		static void cargaModelo() {
			vector<glm::vec3> verts;
			vector<glm::vec2> uvs;
			vector<glm::vec3> normals;
			Render3D::loadOBJ("../DevilDaggers/videojuego/Codigo/3D/cubo.obj", verts, uvs, normals);
			cout << "VERTICES" << endl;
			for (int i = 0; i < verts.size(); i++) {
				cout << "{ " << verts[i].x << ", " << verts[i].y << ", " << verts[i].z << " } ," << endl;
			}
			cout << "NORMALESSS" << endl;
			for (int i = 0; i <  verts.size(); i++) {
				cout << "{ " << normals[i].x << ", " << normals[i].y << ", " << normals[i].z << " } ," << endl;
			}
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
