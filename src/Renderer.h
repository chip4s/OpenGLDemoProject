#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H
#define GLM_ENABLE_EXPERIMENTAL
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/string_cast.hpp>

#include "EntityManager.h"

#include<iostream>
#include<vector>
#include<cerrno>
#include<string>
#include<fstream>
#include<sstream>


std::string get_file_contents(const char* filename);//reads shader text file
class Renderer
{
	public:
		Renderer();

		void CompileShaders();
		void ShaderErrors(unsigned int shader, const  char* type);

		void Draw(glm::mat4 proj, glm::mat4 view, EntityManager& entityManager);//also sends uniforms of entity
		~Renderer();
		
		void HandlePointLights(glm::vec3 camPos, EntityManager& entityManager);

		void HandleDirectionalLights(glm::vec3 camPos, EntityManager& entityManager);

		void HandleSpotLights(glm::vec3 camPos, EntityManager& entityManager);

		GLuint objShaderID;
		GLuint lightShaderID;
};

#endif