#ifndef CAMARA_H_
#define CAMARA_H_

#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>

using namespace std;

class Camara {
	public:
		//matrices de vista
		glm::mat4 View = glm::lookAt(
			glm::vec3(0, 0, 0), // Camera is at (4,3,3), in World Space
			glm::vec3(0, 0, 0), // and looks at the origin
			glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
		GLfloat FoV = 45;
		glm::mat4 Projection = glm::perspective(
			glm::radians(FoV), // El campo de visi�n vertical, en radi�n: la cantidad de "zoom". Piensa en el lente de la c�mara. Usualmente est� entre 90� (extra ancho) y 30� (zoom aumentado)
			4.0f / 3.0f,       // Proporci�n. Depende del tama�o de tu ventana 4/3 == 800/600 == 1280/960, Parece familiar?
			0.1f,              // Plano de corte cercano. Tan grande como sea posible o tendr�s problemas de precisi�n.
			100.0f             // Plano de corte lejano. Tan peque�o como se pueda.
		);
		glm::mat4 Model = glm::mat4(1.0f);
		glm::mat4 MVP;

		void actualizarMVP();
};

#endif // !CAMARA_H_

