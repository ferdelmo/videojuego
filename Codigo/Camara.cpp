#include "Camara.h"

using namespace std;

void Camara::actualizarMVP() {
	MVP = Projection * View * Model;
}