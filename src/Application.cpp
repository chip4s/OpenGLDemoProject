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
	window = glfwCreateWindow(windowWidth, windowHeight, "OpenGLDemo", NULL, NULL);
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


	glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));

	glfwSetKeyCallback(window, Input::key_Callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, Input::mouse_Callback);

	glfwSetWindowSizeCallback(window, Application::window_Resize_Callback);


	//enables face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	//enables depth testing
	glEnable(GL_DEPTH_TEST);

	//enables blend (transparency)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//makes images not upside down
	stbi_set_flip_vertically_on_load(true);
}
void Application::run()
{
	//shader uniform proto
	glm::mat4 view = glm::mat4(1.0f);

	glm::mat4 proj = glm::mat4(1.0f);


	EntityManager entityManager(20);

	Entity& entityOne = entityManager.AddEntity("default");

	entityManager.AddComponent<CMesh>(entityOne);
	entityManager.AddComponent<CTransform>(entityOne, glm::vec3(0.0f, -3.0f, 0.0f), glm::vec3(0.0f), glm::vec3(10.0f, 0.25f, 10.0f));
	entityManager.AddComponent<CTexture>(entityOne, "Textures/default.png");


	Entity& entityTwo = entityManager.AddEntity("default");

	entityManager.AddComponent<CMesh>(entityTwo);
	entityManager.AddComponent<CTransform>(entityTwo, glm::vec3(0.0f, -1.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));
	entityManager.AddComponent<CDirectionalLight>(entityTwo, glm::vec3(1.0f, 1.0f, 1.0f), 0.75f);

	CTransform& entityTwoTrans = entityManager.GetComponentByEntity<CTransform>(entityTwo);

	renderer.CompileShaders();


	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//activate shader before SENDING the uniforms

		//3d matrices
		proj = glm::perspective(glm::radians(45.0f), (float)windowWidth / windowHeight, 0.1f, 10000.0f);
		view = input.CreateViewMat();

		glUseProgram(renderer.objShaderID);
		renderer.HandlePointLights(input.cam.cameraPos, entityManager);
		renderer.HandleDirectionalLights(input.cam.cameraPos, entityManager);
		renderer.HandleSpotLights(input.cam.cameraPos, entityManager);


		if (input.inputs[GLFW_KEY_UP])
		{
			entityTwoTrans.position.z += 0.3f;
		}
		if (input.inputs[GLFW_KEY_DOWN])
		{
			entityTwoTrans.position.z += -0.3f;
		}

		if (input.inputs[GLFW_KEY_RIGHT])
		{
			entityTwoTrans.rotation.z += 0.3f;
		}
		if (input.inputs[GLFW_KEY_LEFT])
		{
			entityTwoTrans.rotation.z += -0.3f;
		}


		//input stuff
		input.handle_CameraMovement(deltaTime);

		//draws and takes pv Uniforms
		renderer.Draw(proj, view, entityManager);

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
void Application::window_Resize_Callback(GLFWwindow* window, int width, int height)
{
	Application* handler = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
	if (handler != nullptr)
	{
		handler->windowWidth = width;
		handler->windowHeight = height;
		glViewport(0, 0, handler->windowWidth, handler->windowHeight);
	}
	
}