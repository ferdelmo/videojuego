#ifndef RENDER_3D_H_
#define RENDER_3D_H_


#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>
#include <SOIL.h>
#include <vector>
#include <fstream>

using namespace std;

class Camara;

class Obj3D {
	public:
		vector< glm::vec3 > vertices;
		vector< glm::vec2 > uvs;
		vector< glm::vec3 > normals;
};

class Render3D {
	public:
		// propiedades del personaje

		const GLfloat pi = atan(1) * 4;
		GLfloat tam = 0.1f;
		glm::vec3 pos = { 0,0,0 };
		glm::vec3 direccion = { 0,0,0 };
		//Para renderizar
		GLuint shaderProgram;
		GLuint VAO;
		GLuint points_VBO;
		GLuint colors_VBO;
		GLuint normalbuffer;

		//textura
		vector< glm::vec3 > colors;

		vector< glm::vec3 > vertices;
		vector< glm::vec2 > uvs;
		vector< glm::vec3 > normals;

		glm::mat4 Model = glm::mat4(1.0f);

		GLFWwindow* window;

		GLuint ProjID;
		GLuint ModelMatrixID;
		GLuint ViewMatrixID;
		GLuint LightID;
		//Rota el punto "punto" sobre centro "angulo" grados(RAD) y lo guarda en rot
		void rotatePoint(GLfloat centro[], GLfloat punto[], GLfloat angulo, GLfloat rot[]) {
			rot[0] = cos(angulo)*(punto[0] - centro[0]) - sin(angulo)*(punto[1] - centro[1]) + centro[0];
			rot[1] = sin(angulo)*(punto[0] - centro[0]) + cos(angulo)*(punto[1] - centro[1]) + centro[1];
			rot[2] = centro[2];
		}

		bool sigue = true;

		Camara * cam;

	public:

		static bool loadOBJ(
			const char * path,
			std::vector<glm::vec3> & out_vertices,
			std::vector<glm::vec2> & out_indics,
			std::vector<glm::vec3> & out_normals
		) {
			printf("Loading OBJ file %s...\n", path);

			std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
			std::vector<glm::vec3> temp_vertices;
			std::vector<glm::vec2> temp_uvs;
			std::vector<glm::vec3> temp_normals;

			ifstream file;
			file.open(path);
			if (!file.is_open()) {
				printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
				getchar();
				return false;
			}
			char lineHeader[128];

			while (!file.eof()) {
				file >> lineHeader;
				//cout << lineHeader << " -> ";
				if (strcmp(lineHeader, "v") == 0) {
					glm::vec3 vertex;
					file >> vertex.x >> vertex.y >> vertex.z;
					//cout << vertex.x << " - " << vertex.y << " - " << vertex.z << endl;
					temp_vertices.push_back(vertex);
				}
				else if (strcmp(lineHeader, "vt") == 0) {
					glm::vec2 uv;
					file >> uv.x >> uv.y;
					uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
					//cout << uv.x << " - " << uv.y << endl;
					temp_uvs.push_back(uv);
				}
				else if (strcmp(lineHeader, "vn") == 0) {
					glm::vec3 normal;
					file >> normal.x >> normal.y >> normal.z;
					//cout << normal.x << " - " << normal.y << " - " << normal.z << endl;
					temp_normals.push_back(normal);
				}
				else if (strcmp(lineHeader, "f") == 0) {
					std::string vertex1, vertex2, vertex3;
					unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
					int matches = 9;
					file >> vertexIndex[0];
					file.ignore(100, '/');
					file >> uvIndex[0]; 
					file.ignore(100, '/');
					file >> normalIndex[0]; 
					file >> vertexIndex[1];
					file.ignore(100, '/');
					file >> uvIndex[1];
					file.ignore(100, '/');
					file >> normalIndex[1];
					file >> vertexIndex[2];
					file.ignore(100, '/');
					file >> uvIndex[2];
					file.ignore(100, '/');
					file >> normalIndex[2];
					if (matches != 9) {
						printf("File can't be read by our simple parser :-( Try exporting with other options\n");
						file.close();
						return false;
					}
					//cout << vertexIndex[0] << " - " << vertexIndex[1] << " - " << vertexIndex[2] << endl;
					//cout << uvIndex[0] << " - " << uvIndex[1] << " - " << uvIndex[2] << endl;
					//cout << normalIndex[0] << " - " << normalIndex[1] << " - " << normalIndex[2] << endl;
					vertexIndices.push_back(vertexIndex[0]);
					vertexIndices.push_back(vertexIndex[1]);
					vertexIndices.push_back(vertexIndex[2]);
					/*uvIndices.push_back(uvIndex[0]);
					uvIndices.push_back(uvIndex[1]);
					uvIndices.push_back(uvIndex[2]);*/
					normalIndices.push_back(normalIndex[0]);
					normalIndices.push_back(normalIndex[1]);
					normalIndices.push_back(normalIndex[2]);
				}
				else {
					// Probably a comment, eat up the rest of the line
					char stupidBuffer[1000];
					file >> stupidBuffer;
				}

			}

			// For each vertex of each triangle
			for (unsigned int i = 0; i<vertexIndices.size(); i++) {

				// Get the indices of its attributes
				unsigned int vertexIndex = vertexIndices[i];
				//unsigned int uvIndex = uvIndices[i];
				unsigned int normalIndex = normalIndices[i];

				// Get the attributes thanks to the index
				glm::vec3 vertex = temp_vertices[vertexIndex - 1];
				//glm::vec2 uv = temp_uvs[uvIndex - 1];
				glm::vec3 normal = temp_normals[normalIndex - 1];

				// Put the attributes in buffers
				out_vertices.push_back(vertex);
				//out_uvs.push_back(uv);
				out_normals.push_back(normal);

					

			}
			file.close();
			return true;
		}


		//CONSTRUCTOR POR DEFECTO
		Render3D(GLFWwindow * window, string vertSha, string fragSha, Camara * c, Obj3D obj, glm::vec3 color, GLfloat escalado);

		//Renderiza
		bool renderizar();
};

#endif
