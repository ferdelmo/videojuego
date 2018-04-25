#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>

#include "Opciones.h"
#include "LoadShader.h"
#include "text2d.h"
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

Opciones::Opciones() {

	mode = 3;

	oldState = GLFW_RELEASE;
	pulsado = false;

	ifstream myfile;
	myfile.open(fich);
	if (myfile.is_open()) {
		myfile >> px >> py >> resolucion;
		myfile >> Fullscreen;
		myfile >> up >> down >> right >> left;
		myfile >> modo;
		myfile >> sonido;
		myfile >> Fov;
		myfile.close();
	}
	else {
		//generamos el opciones por defecto
		ofstream myfile;
		myfile.open(fich);
		px = resoluciones[resolucion][0]; py = resoluciones[resolucion][1];
		myfile << px << " " << py << " " << resolucion << '\n';
		myfile << Fullscreen << '\n';
		myfile << up << " " << down << " " << right << " " << left << '\n';
		myfile << modo << '\n';
		myfile << sonido << '\n';
		myfile << Fov << '\n';
		myfile.close();
	}
	oldresolucion = resolucion;
	aux_up = up; aux_down = down; aux_left = left; aux_right = right;
	aux_modo = modo;
	aux_sonido = sonido;
	aux_Fov = Fov;
}

void Opciones::setValue(GLFWwindow *window, Camara * c) {
	this->window = window;
	this->c = c;

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

	shaderProgram = LoadShaders("../DevilDaggers/videojuego/Codigo/Shaders/opciones.vert", "../DevilDaggers/videojuego/Codigo/Shaders/opciones.frag");
}

void Opciones::restaurarValores() {
	resolucion = oldresolucion;
	up = aux_up; down = aux_down; left = aux_left; right = aux_right;
	modo = aux_modo;
	sonido = aux_sonido;
	Fov = aux_Fov;
}

bool Opciones::cogerTecla() {
	for (int i = 32; i < 256; i++) {
		int state = glfwGetKey(window, i);
		if (state == GLFW_PRESS) {
			if (aCambiar == 0) {
				if (i != down && i != left && i != right) {
					up = i;
				}
			}
			else if (aCambiar == 1) {
				if (i != up && i != left && i != right) {
					down = i;
				}
			}
			else if (aCambiar == 2) {
				if (i != down && i != up && i != right) {
					left = i;
				}
			}
			else if (aCambiar == 3) {
				if (i != down && i != left && i != up) {
					right = i;
				}
			}
			
			i = 256;
			return true;
		}
	}
	return false;
}

void Opciones::controlesInFrame() {
	mode = 3;

	// Posición del cursor
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	int px, py;
	glfwGetWindowSize(window, &px, &py);

	//calula la posicion relativa de la raton en la pantalla
	x = (x / px - 0.5f) * 2;
	y = (abs(y - py) / py - 0.5f) * 2;

	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);

	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

	if (state == GLFW_RELEASE && oldState == GLFW_PRESS) {
		pulsado = false;
	}

	if (!pulsado) { // Control una pulsación
		if (state == GLFW_PRESS && !cambiando) {
			cout << "x: " << x << " ;; y: " << y << endl;
			if (x >= 0.1288 && x <= 0.4363 && y >= -0.8825 && y <= -0.7240) { // BOTÓN GUARDAR + MENÚ PRINCIPAL
				cout << "GUARDAR" << endl;
				cout << "_______________________" << endl;
				pulsado = true;
				mode = 1;
				//Escribe los datitos
				px = resoluciones[resolucion][0]; py = resoluciones[resolucion][1];
				aux_up = up; aux_down = down; aux_left = left; aux_right = right;
				ofstream myfile(fich);
				myfile << px << " " << py << " " << resolucion << '\n';
				myfile << Fullscreen << '\n';
				myfile << up << " " << down << " " << right << " " << left << '\n';
				myfile << modo << '\n';
				myfile << sonido << '\n';
				myfile << Fov << '\n';
				myfile.close();
				cargar();
			}
			else if (x >= -0.4304 && x <= -0.1185 && y >= -0.8825 && y <= -0.72229) { // ATRÁS
				cout << "ATRAS" << endl;
				cout << "_______________________" << endl;
				pulsado = true;
				mode = 1;
				restaurarValores();
			}
			else if (x >= -0.1786 && x <= -0.0219 && y >= 0.3698 && y <= 0.4633) { // FLECHA IZQ RESOLUCIÓN
				cout << "PULSADO FLECHA IZQ RESOLUCION" << endl;
				cout << "_______________________" << endl;
				pulsado = true;
				if (resolucion > 0) {
					resolucion--;
				}
			}
			else if (x >= 0.5549 && x <= 0.6969 && y >= 0.3832 && y <= 0.46595) { // FLECHA DRX RESOLUCIÓN
				cout << "PULSADO FLECHA DER RESOLUCIÓN" << endl;
				cout << "_______________________" << endl;
				pulsado = true;
				if (resolucion < 4) {
					resolucion++;
				}
			}
			else if (x >= -0.1742 && x <= -0.0219 && y >= -0.2950 && y <= -0.1935) { // FLECHA IZQ MODO
				cout << "PULSADO FLECHA IZQ MODO" << endl;
				cout << "_______________________" << endl;
				pulsado = true;
				if (modo == 2) {
					modo--;
				}
			}
			else if (x >= 0.5534 && x <= 0.6998 && y >= -0.2950 && y <= -0.1935) { // FLECHA DRX MODO
				cout << "PULSADO FLECHA DER MODO" << endl;
				cout << "_______________________" << endl;
				pulsado = true;
				if (modo == 1) {
					modo++;
				}
			}
			else if (x >= -0.1757 && x <= -0.0205 && y >= -0.4633 && y <= -0.3692) { // FLECHA IZQ SONIDO
				cout << "PULSADO FLECHA IZQ SONIDO" << endl;
				cout << "_______________________" << endl;
				pulsado = true;
				if (sonido == 1) {
					sonido++;
				}
			}
			else if (x >= 0.5534 && x <= 0.6999 && y >= -0.4633 && y <= -0.3692) { // FLECHA DRX SONIDO
				cout << "PULSADO FLECHA DER SONIDO" << endl;
				cout << "_______________________" << endl;
				pulsado = true;
				if (sonido == 2) {
					sonido--;
				}
			}
			else if (x >= -0.1757 && x <= -0.01756 && y >= -0.6635 && y <= -0.5674) { // FLECHA IZQ FOV
				cout << "PULSADO FLECHA IZQ FOV" << endl;
				cout << "_______________________" << endl;
				pulsado = true;
				if (Fov > minFov) {
					Fov -= 5;
				}
			}
			else if (x >= 0.5505 && x <= 0.6983 && y >= -0.6635 && y <= -0.5674) { // FLECHA DRX FOV
				cout << "PULSADO FLECHA DER FOV" << endl;
				cout << "_______________________" << endl;
				pulsado = true;
				if (Fov < maxFov) {
					Fov += 5;
				}
			}
			else if (x >= 0.3836 && x <= 0.4202 && y >= 0.2149 && y <= 0.2657) { // TECLA ARRIBA
				cout << "PULSADO TECLA DE ARRIBA" << endl;
				cout << "_______________________" << endl;
				pulsado = true;
				cambiando = true;
				aCambiar = 0;
			}
			else if (x >= 0.3850 && x <= 0.4187 && y >= 0.10814 && y <= 0.1615) { // TECLA ABAJO
				cout << "PULSADO TECLA DE ABAJO" << endl;
				cout << "_______________________" << endl;
				pulsado = true;
				cambiando = true;
				aCambiar = 1;
			}
			else if (x >= 0.3865 && x <= 0.4202 && y >= 0.0147 && y <= 0.0654) { // TECLA IZQ
				cout << "PULSADO TECLA DE IZQUIERDA" << endl;
				cout << "_______________________" << endl;
				pulsado = true;
				cambiando = true;
				aCambiar = 2;
			}
			else if (x >= 0.3806 && x <= 0.4158 && y >= -0.0707 && y <= -0.02) { // TECLA DRX
				cout << "PULSADO TECLA DE DERECHA" << endl;
				cout << "_______________________" << endl;
				pulsado = true;
				cambiando = true;
				aCambiar = 3;
			}
		}
		if (cambiando) {
			cambiando = !cogerTecla();
		}
	}
	
	oldState = state;
}

void Opciones::cargar() {
	ifstream myfile;
	myfile.open(fich);
	if (myfile.is_open()) {
		myfile >> px >> py >> resolucion;
		myfile >> Fullscreen;
		myfile >> up >> down >> right >> left;
		myfile.close();
	}
}

void Opciones::mostrarTexto() {
	text2D text2D("../DevilDaggers/videojuego/Codigo/Holstein.DDS");
	string res = to_string(resoluciones[resolucion][0]) + "x" + to_string(resoluciones[resolucion][1]);
	text2D.printText2D(res.c_str(), 428, 415, 20);

	if (modo == 1) { // 2D
		text2D.printText2D("JUEGO 2D", 427, 215, 20);
	}
	else {	// 3D
		text2D.printText2D("JUEGO 3D", 427, 215, 20);
	}

	if (sonido == 1) {
		text2D.printText2D("SI", 487, 160, 20);
	}
	else {
		text2D.printText2D("NO", 487, 160, 20);
	}

	text2D.printText2D(to_string(Fov).c_str(), 487, 103, 20);

	if (cambiando && aCambiar == 0) {
		text2D.printText2D("Pulsa una tecla", 550, 367, 15);
	}
	else {
		stringstream ssa;
		string sa;
		char a = up;
		ssa << a;
		ssa >> sa;
		text2D.printText2D(sa.c_str(), 550, 362, 20);
	}

	if (cambiando && aCambiar == 1) {
		text2D.printText2D("Pulsa una tecla", 550, 335, 15);
	}
	else {
		stringstream ssb;
		string sb;
		char b = down;
		ssb << b;
		ssb >> sb;
		text2D.printText2D(sb.c_str(), 550, 330, 20);
	}

	if (cambiando && aCambiar == 2) {
		text2D.printText2D("Pulsa una tecla", 550, 307, 15);
	}
	else {
		stringstream ssc;
		string sc;
		char c = left;
		ssc << c;
		ssc >> sc;
		text2D.printText2D(sc.c_str(), 550, 302, 20);
	}

	if (cambiando && aCambiar == 3) {
		text2D.printText2D("Pulsa una tecla", 550, 280, 15);
	}
	else {
		stringstream ssd;
		string sd;
		char d = right;
		ssd << d;
		ssd >> sd;
		text2D.printText2D(sd.c_str(), 550, 275, 20);
	}
	text2D.cleanupText2D();
}

int Opciones::renderizar() {
	controlesInFrame();

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

	mostrarTexto();

	return mode;
}