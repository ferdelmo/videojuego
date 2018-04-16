#ifndef ESCENA3D_H_
#define ESCENA3D_H_

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>

#include <Windows.h>
#include <mmsystem.h>
#include <thread>
#include <thread>


using namespace std;

class Personaje3D;

class Bala3D;

class Escena3D {
	private:
		shared_ptr<Personaje3D> per;
		vector<shared_ptr<Bala3D>> balas;
	public:
		int calavsMatadas = 0;

		Escena3D();
		
		void add(shared_ptr<Personaje3D> p);

		void add(shared_ptr<Bala3D> b);

		void add(vector<shared_ptr<Bala3D>> bs);

		void renderizar();

		void moverObjetos();

};

#endif // !ESCENA_H_