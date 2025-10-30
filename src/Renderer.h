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
std::string get_file_contents(const char* filename);//reads shader text file
class Renderer
{
	public:
		Renderer();
		void AddCube(float pX, float pY, float pZ);
		void initialize();
		void CompileShaders(const char* vertexFile, const char* fragmentFile);
		void ActivateShader();
		void DeleteShader();
		void ShaderErrors(unsigned int shader, const  char* type);
		void sendPVMUniforms(glm::mat4 proj, glm::mat4 view, glm::mat4 model);
		void Draw();
		~Renderer();
		std::vector<Vertex> verts;
		std::vector<GLuint> indices;
		GLuint VBO;
		GLuint VAO;
		GLuint EBO;
		GLuint shaderID;
};
#endif