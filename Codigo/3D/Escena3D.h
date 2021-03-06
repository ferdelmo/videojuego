#ifndef ESCENA3D_H_
#define ESCENA3D_H_

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>

#include <thread>
#include <thread>
#include "Render3D.h"
#include <al.h>
#include <alc.h>


using namespace std;

class Personaje3D;

class Bala3D;

class CalaveraBase3D;

class Gema3D;

class Daga3D;

class Network;

class Escena3D {
	public:
		shared_ptr<Personaje3D> per;
		std::vector<shared_ptr<Bala3D>> balas;
		std::vector<shared_ptr<CalaveraBase3D>> calaveras;
		std::vector<shared_ptr<Gema3D>> gemas;
		std::vector<shared_ptr<Daga3D>> dagas;
		shared_ptr<Render3D> fondo;

		ALuint buffer;
		ALsizei size;
		ALsizei frequency;
		ALenum format;
		int Sonido = -1;

		Network * redCalavs;

	public:
		int calavsMatadas = 0;

		bool espectador = false;

		float limite = 10; //(10,10) (-10,-10)

		bool fin = false;

		Escena3D();
		
		void entrenarRed();

		void add(shared_ptr<Personaje3D> p);

		void add(shared_ptr<Bala3D> b);

		void add(shared_ptr<Gema3D> g);

		void add(shared_ptr<CalaveraBase3D> c);

		void add(shared_ptr<Daga3D> D);

		void add(std::vector<shared_ptr<Bala3D>> bs);

		std::vector<shared_ptr<Bala3D>>* getBalas();

		std::vector<shared_ptr<Gema3D>>* getGemas();

		std::vector<shared_ptr<Daga3D>>* getDagas();

		void renderizar();

		void moverObjetos();

		void reset();

};

#endif // !ESCENA_H_