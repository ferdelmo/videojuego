#pragma once
#ifndef TEXT2D_H
#define TEXT2D_H


#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>

#include <iostream>
#include <vector>

using namespace std;

class text2D {

	GLuint texture;
	unsigned int width, height;
	GLuint vertices_VBO;
	GLuint UV_VBO;
	GLuint shaderProgram;
	GLuint Uniform;

	GLuint VertexArrayID;


public:

	GLuint loadDDs(const char* imagepath);

	void init_string_renderer();

	void end_string_renderer();

	text2D(const char* texturePath);

	void printText2D(const char* text, int x, int y, int size);

	void printText3D(const char* text, int x, int y, int size);

	void cleanupText2D();

};

#endif // !TEXT2D_H

