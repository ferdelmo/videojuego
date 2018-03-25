#ifndef ESCENA_H_
#define ESCENA_H_

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>
#include <thread>
#include <mutex> 


using namespace std;

class CalaveraBase;

class CalaveraBaseII;

class CalaveraBaseIII;

class Daga;

class Personaje;

class Bala;

class Gema;

class Fondo;

class Escena {
	private:
		vector<shared_ptr<Bala>> balas;
		vector<shared_ptr<CalaveraBase>> calaveras;
		vector<shared_ptr<CalaveraBaseII>> calaverasII;
		vector<shared_ptr<CalaveraBaseIII>> calaverasIII;
		vector<shared_ptr<Daga>> dagas;
		vector<shared_ptr<Gema>> gemas;
		shared_ptr<Personaje> per;
		shared_ptr<Fondo> fon;

		bool fin = false;

		mutex dag, cal, calII, calIII;
	public:
		Escena();

		void add(shared_ptr<Bala> b);

		void add(vector<shared_ptr<Bala>> bs);

		void add(shared_ptr<CalaveraBase> cb);

		void add(shared_ptr<CalaveraBaseII> cb);

		void add(shared_ptr<CalaveraBaseIII> cb);

		void add(shared_ptr<Daga> d);

		void add(shared_ptr<Personaje> p);

		void add(shared_ptr<Gema> g);

		void setFondo(shared_ptr<Fondo> f);

		GLfloat getLimites();

		vector<shared_ptr<Bala>> * getBalas();

		vector<shared_ptr<CalaveraBase>> * getCalaveras();

		vector<shared_ptr<CalaveraBaseII>> * getCalaverasII();

		vector<shared_ptr<CalaveraBaseIII>> * getCalaverasIII();

		vector<shared_ptr<Daga>> * getDagas();

		vector<shared_ptr<Gema>> * getGemas();

		shared_ptr<Personaje> getPer();

		void renderizar();

		void actualizarFisicas();

		void pararFisicas();

};

#endif // !ESCENA_H_

