#ifndef ESCENA_H_
#define ESCENA_H_

#include <vector>
#include <memory>


using namespace std;

class CalaveraBase;

class Daga;

class Personaje;

class Bala;

class Escena {
	private:
		vector<shared_ptr<Bala>> balas;
		vector<shared_ptr<CalaveraBase>> calaveras;
		vector<shared_ptr<Daga>> dagas;
		shared_ptr<Personaje> per;
	public:
		Escena();

		void add(shared_ptr<Bala> b);

		void add(shared_ptr<CalaveraBase> cb);

		void add(shared_ptr<Daga> d);

		void add(shared_ptr<Personaje> p);

		vector<shared_ptr<Bala>> * getBalas();

		vector<shared_ptr<CalaveraBase>> * getCalaveras();

		vector<shared_ptr<Daga>> * getDagas();

		shared_ptr<Personaje> getPer();

		void renderizar();

};

#endif // !ESCENA_H_

