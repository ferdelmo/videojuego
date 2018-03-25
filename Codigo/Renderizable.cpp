#include "Renderizable.h"
#include "LoadShader.h"
#include "Camara.h"

using namespace std;

Renderizable::Renderizable(GLFWwindow * window, string textura, string vertSha, string fragSha, GLfloat tam, Camara * c){
	this->tam = tam;
	this->window = window;
	cam = c;
	texImage = SOIL_load_image(textura.c_str(), &texWid,
		&texHei, &texChan, SOIL_LOAD_RGBA);
	//calcula los puntos del triangulo segun la orientacion
	for (int i = 0; i < 4; i++) {
		GLfloat auxx = 1;
		if (i / 2 != 0) {
			auxx = -1;
		}
		GLfloat auxy = 1;
		if (i % 2 != 0) {
			auxy = -1;
		}
		GLfloat puntoaux[3] = { pos[0] + tam * auxx,pos[1] + tam * auxy,pos[2] };
		GLfloat punto1a[3] = { 0,0,0 };
		rotatePoint(pos, puntoaux, orientacion, punto1a);
		for (int j = 0; j < 3; j++) {
			vertices[i * 3 + j] = punto1a[j];
		}
	}
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
	/*glGenBuffers(1, &colors_VBO);
	// bindea ese buffer
	glBindBuffer(GL_ARRAY_BUFFER, colors_VBO);
	// Lo rellena con la informacion del color
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);*/
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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWid, texHei, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, texImage);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(texImage);

	shaderProgram = LoadShaders(vertSha.c_str(), fragSha.c_str());
 	MatrixID = glGetUniformLocation(shaderProgram, "MVP");
	 // Remember, matrix multiplication is the other way around
}

Renderizable::Renderizable(GLFWwindow * window, string textura, string vertSha, string fragSha, GLfloat tam, GLfloat offset, Camara * c) {
	this->tam = tam;
	this->window = window;
	cam = c;
	texImage = SOIL_load_image(textura.c_str(), &texWid,
		&texHei, &texChan, SOIL_LOAD_RGBA);
	for (int i = 0; i < 8; i++) {
		texCoords[i] *= offset;
	}
	//calcula los puntos del triangulo segun la orientacion
	for (int i = 0; i < 4; i++) {
		GLfloat auxx = 1;
		if (i / 2 != 0) {
			auxx = -1;
		}
		GLfloat auxy = 1;
		if (i % 2 != 0) {
			auxy = -1;
		}
		GLfloat puntoaux[3] = { pos[0] + tam * auxx,pos[1] + tam * auxy,pos[2] };
		GLfloat punto1a[3] = { 0,0,0 };
		rotatePoint(pos, puntoaux, orientacion, punto1a);
		for (int j = 0; j < 3; j++) {
			vertices[i * 3 + j] = punto1a[j];
		}
	}
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
	/*glGenBuffers(1, &colors_VBO);
	// bindea ese buffer
	glBindBuffer(GL_ARRAY_BUFFER, colors_VBO);
	// Lo rellena con la informacion del color
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);*/
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

	shaderProgram = LoadShaders(vertSha.c_str(), fragSha.c_str());
	MatrixID = glGetUniformLocation(shaderProgram, "MVP");
}

Renderizable::Renderizable(GLFWwindow * window, string textura, GLfloat tam, Camara * c, GLuint sha) {
	this->tam = tam;
	this->window = window;
	cam = c;
	texImage = SOIL_load_image(textura.c_str(), &texWid,
		&texHei, &texChan, SOIL_LOAD_RGBA);
	//calcula los puntos del triangulo segun la orientacion
	for (int i = 0; i < 4; i++) {
		GLfloat auxx = 1;
		if (i / 2 != 0) {
			auxx = -1;
		}
		GLfloat auxy = 1;
		if (i % 2 != 0) {
			auxy = -1;
		}
		GLfloat puntoaux[3] = { pos[0] + tam * auxx,pos[1] + tam * auxy,pos[2] };
		GLfloat punto1a[3] = { 0,0,0 };
		rotatePoint(pos, puntoaux, orientacion, punto1a);
		for (int j = 0; j < 3; j++) {
			vertices[i * 3 + j] = punto1a[j];
		}
	}
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
	/*glGenBuffers(1, &colors_VBO);
	// bindea ese buffer
	glBindBuffer(GL_ARRAY_BUFFER, colors_VBO);
	// Lo rellena con la informacion del color
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);*/
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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWid, texHei, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, texImage);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(texImage);

	this->shaderProgram = sha;
	MatrixID = glGetUniformLocation(this->shaderProgram, "MVP");
}

Renderizable Renderizable::operator=(const Renderizable& b) {
	if (this != &b) { // self-assignment check expected
		this->tam = b.tam;
		for (int i = 0; i < 3; i++) {
			this->pos[i] = b.pos[i];
		}
		this->orientacion = b.orientacion;
		//Para renderizar
		this->shaderProgram = b.shaderProgram;
		this->VAO = b.VAO;
		this->EBO = b.EBO;
		this->colors_VBO = b.colors_VBO;
		//this->points_VBO = b.points_VBO;

		for (int i = 0; i < 12; i++) {
			this->colors[i] = b.colors[i];
			this->vertices[i] = b.vertices[i];
		}
		for (int i = 0; i < 8; i++) {
			this->texCoords[i] = b.texCoords[i];
		}
		this->texCoords_VBO = b.texCoords_VBO;
		this->texture = b.texture;
		for (int i = 0; i < 6; i++) {
			this->indices[i] = b.indices[i];
		}
		this->texWid = b.texWid;
		this->texHei = b.texHei;
		this->texChan = b.texChan;
		this->texImage = b.texImage;

		this->window = b.window;

		this->sigue = b.sigue;

		this->cam = b.cam;
	}
	return *this;
}

bool Renderizable::renderizar() {
	//mover();
	//calcula los puntos del triangulo segun la orientacion
	for (int i = 0; i < 4; i++) {
		GLfloat auxx = 1;
		if (i / 2 != 0) {
			auxx = -1;
		}
		GLfloat auxy = 1;
		if (i % 2 != 0) {
			auxy = -1;
		}
		GLfloat puntoaux[3] = { (pos[0] + tam * auxx),(pos[1] + tam * auxy),pos[2] };
		GLfloat punto1a[3] = { 0,0,0 };
		rotatePoint(pos, puntoaux, orientacion, punto1a);
		for (int j = 0; j < 3; j++) {
			vertices[i * 3 + j] = punto1a[j];
		}
	}

	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(shaderProgram);

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(cam->MVP[0][0]));

	glBindVertexArray(this->VAO); // une el VAO, que contiene toda la
							//información de los vértices, al contexto
							//vertices
							//argumento 0 posicion
	glEnableVertexAttribArray(0);
	//bindea el buffer
	//glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
	//lo leena con los puntos
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	/*glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);*/
	// bindea ese buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// Lo rellena con la informacion de puntos
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
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
	/*glEnableVertexAttribArray(1);
	//bindea el buffer
	//glBindBuffer(GL_ARRAY_BUFFER, colors_VBO);
	// le pasa el color de cada vertice
	//glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	/*glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);*/
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
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

	return sigue;
}

void Renderizable::mover() {}

void Renderizable::fisicas() {}

GLfloat Renderizable::distancia(GLfloat x, GLfloat y, GLfloat xp, GLfloat yp) {
	return (x - xp)*(x - xp) + (y - yp)*(y - yp);
}