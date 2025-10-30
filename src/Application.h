#ifndef APPLICATION_CLASS_H
#define APPLICATION_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<string>
#include<iostream>
#include "Renderer.h"
#include "Input.h"
class Application
{
	public:
		Application();
		void run();
	   ~Application();
	   unsigned int windowWidth;
	   unsigned int windowHeight;
	   GLFWwindow* window;
	   Renderer renderer;
	   Input input;
	private:
		//time
		void CalculateDeltaTime();
		double deltaTime;
		double currentFrame;
		double lastFrame;
		//shader uniforms
		//void UniformMat4(glm::mat4 mvp, std::string);
};
#endif