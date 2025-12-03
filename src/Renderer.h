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
		void ShadowBufferInitialize()
		{
			//create depth buffer texture
			glGenTextures(1, &depth);
			glBindTexture(GL_TEXTURE_2D, depth);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glBindTexture(GL_TEXTURE_2D, 0);

			
			//create cube map to store in all directions
			glGenTextures(1, &shadowCubeMap);
			glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubeMap);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			for (int i = 0; i < 6; i++)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R32F, size, size, 0, GL_RED, GL_FLOAT, NULL);
			}

			
			//create frame buffer
			glGenFramebuffers(1, &FBO);
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth, 0);

			//don't write or read color buffer
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}
		void ShadowBufferDraw()
		{
			
		}
};

#endif