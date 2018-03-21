#ifndef LOADSHADER_H_
#define LOADSHADER_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace std;

/*
	FUNCION PARA CARGAR LOS DOS SHADERS
*/
GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

#endif