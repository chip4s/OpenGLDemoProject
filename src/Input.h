#ifndef INPUT_CLASS_H
#define INPUT_CLASS_H
#define GLM_ENABLE_EXPERIMENTAL
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<unordered_map>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
struct Camera//freecam with mouse similar to learnopengl's
{
	bool firstMouse;
	double lastX;
	double lastY;
	double yaw;
	double pitch;
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
};
class Input
{
	public:
		Input(GLFWwindow* w);
		GLFWwindow* window;
		//keybinds
		inline static std::unordered_map<int, bool> inputs;
		static void key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods);


		//mouse and camera
		static void mouse_Callback(GLFWwindow* window, double xpos, double ypos);
		void handle_CameraMovement(float dt);

		
		inline static Camera cam =
		{
			true,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
		};
		glm::mat4 CreateViewMat();
};
#endif