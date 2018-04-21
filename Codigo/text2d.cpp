#include "text2D.h"
#include "LoadShader.h"

#include <string>
#include <fstream>
#include <sstream>

using namespace std;
using namespace glm;

// Tipos de compresiones
#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint text2D::loadDDs(const char* imagepath) {
	unsigned char header[124];

	FILE * f;
	fopen_s(&f, imagepath, "rb");
	if (f == NULL) {
		cout << "Error al abrir el fichero" << endl;
		return 0;
	}

	// Verificar tipo de archivo
	char filecode[4];
	fread(filecode, 1, 4, f);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		cout << "Error. Es necesario un fichero DDS" << endl;
		fclose(f);
		return 0;
	}

	fread(&header, 124, 1, f);

	height = *(unsigned int*)&(header[8]);
	width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);
	//cout << "height: " << height << " ;; width: " << width << endl;
	//cout << "linearSize: " << linearSize << " ;; mipMapCount: " << mipMapCount << ";; fourCC: " << fourCC << endl;

	unsigned char* buffer;
	unsigned int bufsize;

	// Tamaño total para todos los mipmaps
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));

	fread(buffer, 1, bufsize, f);
	fclose(f);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;

	switch (fourCC) {
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		// cout << "free buffer" << endl;
		return 0;
	}
	
	// textura
	GLuint tex;
	glGenTextures(1, &tex);

	glBindTexture(GL_TEXTURE_2D, tex);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	// Cargar mipmaps
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) {
		unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, buffer + offset);
		offset += size;
		width /= 2;
		height /= 2;

		if (width < 1) width = 1;
		if (height < 1) height = 1;
	}

	free(buffer);

	return tex;
}

void text2D::init_string_renderer()
{

	GLdouble winx, winy, winz;
	GLint viewp[4];
	GLdouble model[16], pro[16];


	glGetDoublev(GL_MODELVIEW_MATRIX, model);
	glGetDoublev(GL_PROJECTION_MATRIX, pro);
	glGetIntegerv(GL_VIEWPORT, viewp);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);


	glPushMatrix();   // Push-1

	gluProject(0.0, 0.0, 0.0, model, pro, viewp, &winx, &winy, &winz);

	glMatrixMode(GL_PROJECTION);

	glPushMatrix(); // Push-2

	glLoadIdentity();

	glOrtho(0.0, viewp[2], 0.0, viewp[3], 0.0, 1.0);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix(); // Pop-3

	glLoadIdentity();

	glTranslated(winx, winy, -winz);
}

void text2D::end_string_renderer()
{
	glPopMatrix();    // Pop-3

	glMatrixMode(GL_PROJECTION);

	glPopMatrix();    // Pop-2

	glMatrixMode(GL_MODELVIEW);

	glPopMatrix();    // Pop-1

	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);

	glPopAttrib();
}

text2D::text2D(const char* texturePath) {
	// Inicializar textura
	texture = loadDDs(texturePath);

	// Inicializar VBO
	glGenBuffers(1, &vertices_VBO);
	glGenBuffers(1, &UV_VBO);

	// Inicializar shader
	shaderProgram = LoadShaders("../DevilDaggers/videojuego/Codigo/Shaders/text.vert", "../DevilDaggers/videojuego/Codigo/Shaders/text.frag");

	// Inicializar ID
	Uniform = glGetUniformLocation(shaderProgram, "myTextureSampler");
}

// x e y --> posición a escribir
void text2D::printText2D(const char* text, int x, int y, int size) {
	int length = strlen(text);

	vector<glm::vec2> vertices;
	vector<glm::vec2> UVs;

	// Para cada elemento, se calculan las coordenadas de los cuatro vértices que
	// determinan el quad y se agregan los dos triángulos
	for (unsigned int i = 0; i < length; i++) {
		vec2 vertex_down_left = vec2(x + i * size, y + size);
		vec2 vertex_down_right = vec2(x + i * size + size, y + size);
		vec2 vertex_up_right = vec2(x + i * size + size, y);
		vec2 vertex_up_left = vec2(x + i * size, y);

		// Triángulo #1
		vertices.push_back(vertex_up_left);
		vertices.push_back(vertex_down_left);
		vertices.push_back(vertex_up_right);

		// Triángulo #2
		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		char c = text[i];
		float uv_x = (c % 16) / 16.0f; // columna
		float uv_y = (height - 0.0625) - (c / 16) / 16.0f; // fila

		//cout << "letra: " << c << " ;; columna: " << uv_x << " ;; fila: " << uv_y << endl;

		vec2 uv_up_left = vec2(uv_x, 1.0f - uv_y);
		vec2 uv_up_right = vec2(uv_x + 1.0f / 16.0f, 1.0f - uv_y);
		vec2 uv_down_right = vec2(uv_x + 1.0f / 16.0f, 1.0f - (uv_y + 1.0f / 16.0f));
		vec2 uv_down_left = vec2(uv_x, 1.0f - (uv_y + 1.0f / 16.0f));

		//cout << uv_up_left[1] - uv_down_left[1] << endl;

		// Triángulo 1
		UVs.push_back(uv_up_left);
		UVs.push_back(uv_down_left);
		UVs.push_back(uv_up_right);

		// Triángulo 2
		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
	}

	glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, UV_VBO);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(vec2), &UVs[0], GL_STATIC_DRAW);

	// Bind shader
	glUseProgram(shaderProgram);

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(texture, 0);

	// Buffer de atributos: vértices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Buffer de atributos: UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, UV_VBO);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Pintar
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void text2D::printText3D(const char* text, int x, int y, int size) {
	init_string_renderer();
	printText2D(text, x, y, size);
	end_string_renderer();
}

void text2D::cleanupText2D() {
	// Borrar buffers
	glDeleteBuffers(1, &vertices_VBO);
	glDeleteBuffers(1, &UV_VBO);

	// Borrar textura
	glDeleteTextures(1, &texture);

	// Borrar shader
	glDeleteProgram(shaderProgram);
}
