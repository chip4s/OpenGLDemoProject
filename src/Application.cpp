#include "Application.h"
Application::Application() : input(window)
{
	currentFrame = 0.0;
	deltaTime = 0.0;
	lastFrame = 0.0;
	//Initialize GLFW
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW\n";
	}

	// Tell GLFW what version of OpenGL we are using
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	windowWidth = 1500; //glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
	windowHeight = 750;//glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
	// Create a GLFWwindow object of width by height, naming it
	//window = glfwCreateWindow(windowWidth, windowHeight, "OpenGLDemos", glfwGetPrimaryMonitor(), NULL); //for fullscreen
	window = glfwCreateWindow(windowWidth, windowHeight, "OpenGLDemos", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	//Load GLAD so it configures OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = max, y = max
	glViewport(0, 0, windowWidth, windowHeight);
	glfwSetKeyCallback(window, Input::key_Callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, Input::mouse_Callback);

	//enables face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	//enables depth testing
	glEnable(GL_DEPTH_TEST);
}
void Application::run()
{
	//shader uniform proto
	glm::mat4 view = glm::mat4(1.0f);

	glm::mat4 proj = glm::mat4(1.0f);
	int GO = renderer.AddCube(1.5f, 0.0f, 0.0f, OBJECT);
	int GL = renderer.AddCube(0.0f, 0.0f, 0.0f, POINT_LIGHT);
	int GT = renderer.AddCube(-1.5f, 0.0f, 0.0f, OBJECT);

	renderer.AddPointLight(GL, glm::vec3(1.0,1.0,1.0), 0.7f, 1.8f);

	renderer.AddDirectionalLight(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	renderer.AddDirectionalLight(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	renderer.CompileShaders();//could move the call to initialize
	renderer.initialize();


	glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f));
	renderer.SetCubeModelMat(GL, model, POINT_LIGHT);


	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//activate shader before SENDING the uniforms

		//3d matrices
		proj = glm::perspective(glm::radians(45.0f), (float)windowWidth / windowHeight, 0.1f, 100.0f);
		view = input.CreateViewMat();


		renderer.HandlePointLights(input.cam.cameraPos);
		renderer.HandleDirectionalLights(input.cam.cameraPos);

		//example of moving point light
		glm::vec3 moveLight = glm::vec3(0.0f);
		if (input.inputs[GLFW_KEY_UP] == true)
		{
			moveLight.z += -0.1f;
		}
		if (input.inputs[GLFW_KEY_DOWN] == true)
		{
			moveLight.z += 0.1f;
		}
		if (input.inputs[GLFW_KEY_RIGHT] == true)
		{
			moveLight.x += 0.1f;
		}
		if (input.inputs[GLFW_KEY_LEFT] == true)
		{
			moveLight.x += -0.1f;
		}
		glm::mat4 tML = glm::translate(glm::mat4(1.0f), moveLight);
		renderer.SetCubeModelMat(GL, tML, POINT_LIGHT);

		//input stuff
		input.handle_CameraMovement(deltaTime);

		//draws and takes pv Uniforms
		renderer.Draw(proj, view);

		this->CalculateDeltaTime();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
Application::~Application()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}
void Application::CalculateDeltaTime()
{
	currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}