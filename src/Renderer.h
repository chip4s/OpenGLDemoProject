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
#include <cfloat>


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
		GLuint shadowShaderID;

		//Omni-directional shadows
		GLuint size;
		GLuint depth;
		GLuint shadowCubeMap;
		GLuint FBO;
		struct CubeDirection
		{
			GLenum cubeFace;
			glm::vec3 target;
			glm::vec3 up;
		};
		CubeDirection cameraDirections[6] =
		{
			{GL_TEXTURE_CUBE_MAP_POSITIVE_X, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)},
			{GL_TEXTURE_CUBE_MAP_NEGATIVE_X, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)},
			{GL_TEXTURE_CUBE_MAP_POSITIVE_Y, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)},
			{GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f,-1.0f)},
			{GL_TEXTURE_CUBE_MAP_POSITIVE_Z, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)},
			{GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)},
		};
		glm::mat4 shadowPerspective;

		void ShadowPass(EntityManager& entityManager);

		void LightingPass(unsigned int windowWidth, unsigned int windowHeight, glm::mat4 proj, glm::mat4 view, EntityManager& entityManager);
};

#endif