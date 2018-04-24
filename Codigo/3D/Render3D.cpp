#include "Render3D.h"
#include "../LoadShader.h"
#include "../Camara.h"

using namespace std;

Render3D::Render3D(GLFWwindow * window, string vertSha, string fragSha, Camara * c, Obj3D obj, glm::vec3 color, GLfloat escalado) {

	this->window = window;
	cam = c;
	tam = escalado;
	vertices = vector<glm::vec3>(obj.vertices);
	uvs = {};
	normals = vector<glm::vec3>(obj.normals);

	//cout << "VERTICES" << endl;
	for (int i = 0; i < vertices.size(); i++) {
		colors.push_back(color);
		//glm::vec3 direccion = vertices[i];
		//cout << "{ " << direccion.x << ", " << direccion.y << ", " << direccion.z << " }" << endl;
	}
	//cout << vertices.size() << endl;
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
	ProjID = glGetUniformLocation(shaderProgram, "P");
	ViewMatrixID = glGetUniformLocation(shaderProgram, "V");
	ModelMatrixID = glGetUniformLocation(shaderProgram, "M");
	LightID = glGetUniformLocation(shaderProgram, "LightPosition_worldspace");
	// Remember, matrix multiplication is the other way around
}

Render3D::Render3D(GLFWwindow * window, Camara * c, string textura, GLfloat offset, GLfloat escalado) {
	this->window = window;
	cam = c;
	tam = escalado;
	vertices = { {1,0,1},{-1,0,-1},{-1,0,1},
				{ 1,0,1 }, {1,0,-1},{ -1,0,-1 } };
	uvs = { {1,1},{0,0},{0,1},
			{ 1,1 },{ 1,0 },{ 0,0 } };
	normals = { {0,1,0},{ 0,1,0 },{ 0,1,0 },
				{ 0,1,0 },{ 0,1,0 },{ 0,1,0 }};

	for (int i = 0; i < uvs.size(); i++) {
		uvs[i] *= offset;
	}
	//cout << vertices.size() << endl;
	cout << "PUES NO FUNCA ESTA MEIRDA" << endl;
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
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);

	//NORMALES
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);


	glBindVertexArray(0);

	shaderProgram = LoadShaders("../DevilDaggers/videojuego/Codigo/Shaders/3Dtext.vert", "../DevilDaggers/videojuego/Codigo/Shaders/3Dtext.frag");
	ProjID = glGetUniformLocation(shaderProgram, "P");
	ViewMatrixID = glGetUniformLocation(shaderProgram, "V");
	ModelMatrixID = glGetUniformLocation(shaderProgram, "M");
	LightID = glGetUniformLocation(shaderProgram, "LightPosition_worldspace");
	texImage = SOIL_load_image(textura.c_str(), &texWid,
		&texHei, &texChan, SOIL_LOAD_RGBA);
	//TEXTURA PROPIA
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWid, texHei, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, texImage);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(texImage);
}

bool Render3D::renderizar() {
	/*glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/


	glUseProgram(shaderProgram);
	glm::mat4 escalado =
		glm::scale(glm::mat4(1.0f), glm::vec3(tam,tam,tam));
	//glm::mat4 traslacion = glm::translate(glm::mat4(1.0f),pos);
	
	glm::mat4 rotation = glm::lookAt(
		pos, // Camera is at (4,3,3), in World Space
		pos + direccion, // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	if (uvs.size() > 0) {
		rotation = glm::mat4(1.0f);
	}
	Model = glm::inverse(rotation) * escalado;

	glUniformMatrix4fv(ProjID, 1, GL_FALSE, &(cam->Projection[0][0]));
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &Model[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &cam->View[0][0]);

	glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

	glBindVertexArray(VAO); // une el VAO, que contiene toda la
							//información de los vértices, al contexto
							//vertices
							//argumento 0 posicion
	glEnableVertexAttribArray(0);
	//bindea el buffer
	glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
	//lo leena con los puntos
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
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
	//glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors[0], GL_STATIC_DRAW);
	int aux = 3;
	if (uvs.size() > 0) {
		aux = 2;
	}
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		aux,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	//NORMALS
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	//glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
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
	if (uvs.size() > 0) {
		glBindTexture(GL_TEXTURE_2D, texture);
	}
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() * sizeof(glm::vec3));  // Starting from vertex 0; 3 vertices total -> 1 triangle
	glBindVertexArray(0); // deshace la unión del VAO

						  //desactiva estos argumentos una vez usados
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	return sigue;
}