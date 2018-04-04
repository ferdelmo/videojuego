#include "Render3D.h"
#include "../LoadShader.h"
#include "../Camara.h"

using namespace std;

Render3D::Render3D(GLFWwindow * window, string vertSha, string fragSha, Camara * c, Obj3D obj, glm::vec3 color, GLfloat escalado) {

	this->window = window;
	cam = c;
	tam = escalado;
	vertices = vector<glm::vec3>(obj.vertices);
	uvs = vector<glm::vec2>(obj.uvs);
	normals = vector<glm::vec3>(obj.normals);

	for (int i = 0; i < vertices.size(); i++) {
		colors.push_back(color);

	}
	cout << vertices.size() << endl;
	//calcula los puntos del triangulo segun la orientacio}
	//VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//VERTICES
	// genera el buffer para vertices
	glGenBuffers(1, &points_VBO);
	// bindea el buffer
	glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
	// Llena el buffer con la informacion de los puntos
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);

	//COLOR
	// Genera el buffer de colores
	glGenBuffers(1, &colors_VBO);
	// bindea ese buffer
	glBindBuffer(GL_ARRAY_BUFFER, colors_VBO);
	// Lo rellena con la informacion del color
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3),&colors[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);

	//NORMALES
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);


	glBindVertexArray(0);

	shaderProgram = LoadShaders(vertSha.c_str(), fragSha.c_str());
	MatrixID = glGetUniformLocation(shaderProgram, "MVP");
	ViewMatrixID = glGetUniformLocation(shaderProgram, "V");
	ModelMatrixID = glGetUniformLocation(shaderProgram, "M");
	LightID = glGetUniformLocation(shaderProgram, "LightPosition_worldspace");
	// Remember, matrix multiplication is the other way around
}

bool Render3D::renderizar() {

	vector<glm::vec3> vertRe(vertices);
	for (int i = 0; i < vertices.size(); i++) {
		glm::vec3 aux = vertices[i];
		aux.x = (aux.x + pos[0])*tam;
		aux.y = (aux.y + pos[1])*tam;
		aux.z = (aux.z + pos[2])*tam;
		vertRe[i] = aux;
	}
	/*glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	glUseProgram(shaderProgram);

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(cam->MVP[0][0]));
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &cam->Model[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &cam->View[0][0]);

	glm::vec3 lightPos = { 4,3,8 };
	glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

	glBindVertexArray(VAO); // une el VAO, que contiene toda la
							//información de los vértices, al contexto
							//vertices
							//argumento 0 posicion
	glEnableVertexAttribArray(0);
	//bindea el buffer
	glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
	//lo leena con los puntos
	glBufferData(GL_ARRAY_BUFFER, vertRe.size() * sizeof(glm::vec3), &vertRe[0], GL_STATIC_DRAW);
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
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors[0], GL_STATIC_DRAW);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	//NORMALS
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
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
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() * sizeof(glm::vec3));  // Starting from vertex 0; 3 vertices total -> 1 triangle
	glBindVertexArray(0); // deshace la unión del VAO

						  //desactiva estos argumentos una vez usados
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

	return sigue;
}