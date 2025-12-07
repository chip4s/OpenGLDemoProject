#ifndef APPLICATION_CLASS_H
#define APPLICATION_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<string>
#include<iostream>
#include "Renderer.h"
#include "Physics.h"
#include "Input.h"
class Application
{
	public:
		Application();
		void run();
	   ~Application();
	
	private:
		unsigned int windowWidth;
		unsigned int windowHeight;
		GLFWwindow* window;
		Renderer renderer;
		Physics physics;
		Input input;

		//time
		void CalculateDeltaTime();
		double deltaTime;
		double currentFrame;
		double lastFrame;

		//window resize callback
		static void window_Resize_Callback(GLFWwindow* window, int width, int height);
};
#endif