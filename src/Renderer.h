#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H
#define GLM_ENABLE_EXPERIMENTAL
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include<iostream>
#include<vector>
#include<cerrno>
#include<string>
#include<fstream>
#include<sstream>

struct Vertex
{
	float posX;
	float posY;
	float posZ;
	float UVX;
	float UVY;
};
struct Object
{
	std::vector<Vertex> verts;
	std::vector<GLuint> indices;
	glm::mat4 model = glm::mat4(1.0f);
	GLuint VBO = 0;
	GLuint VAO = 0;
	GLuint EBO = 0;
};
enum obj
{
	OBJECT,
	LIGHT,
};
std::string get_file_contents(const char* filename);//reads shader text file
class Renderer
{
	public:
		Renderer();
		int AddCube(float pX, float pY, float pZ, obj t);//returns index of where cube is
		void SetCubeModelMat(int index, glm::mat4 mod, obj t);
		void initialize();
		void CompileShaders();
		void ShaderErrors(unsigned int shader, const  char* type);
		void Draw(glm::mat4 proj, glm::mat4 view);//also sends uniforms of PV while model is stored in object
		~Renderer();
		std::vector<Object> objects;
		std::vector<Object> lights;
		GLuint objShaderID;
		GLuint lightShaderID;
};
#endif