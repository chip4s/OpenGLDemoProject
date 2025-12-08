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
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	//enables depth testing
	glEnable(GL_DEPTH_TEST);

	//enables blend (transparency)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//makes images not upside down
	stbi_set_flip_vertically_on_load(true);

	//enables multi sampling for anti aliasing
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);
}
void Application::run()
{
	//shader uniform proto
	glm::mat4 view = glm::mat4(1.0f);

	glm::mat4 proj = glm::mat4(1.0f);



	EntityManager entityManager(10);

	Entity& entityOne = entityManager.AddEntity("default");

	entityManager.AddComponent<CMesh>(entityOne, CUBE);
	entityManager.AddComponent<CTransform>(entityOne, glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(10.0f, 1.0f, 10.0f));
	entityManager.AddComponent<CBoxCollider>(entityOne, 10.0f, 1.0f, 10.0f);
	entityManager.AddComponent<CRigidBody>(entityOne, STATIC, 1.0f, -4.0f);



	Entity& entityTwo = entityManager.AddEntity("UI");

	entityManager.AddComponent<CUI>(entityTwo, -1);
	entityManager.AddComponent<CTransform>(entityTwo, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	entityManager.AddComponent<CTexture>(entityTwo, "Textures/HelloWorldUI.png");



	Entity& entityThree = entityManager.AddEntity("default");
	entityManager.AddComponent<CMesh>(entityThree, CUBE);
	entityManager.AddComponent<CTransform>(entityThree, glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	entityManager.AddComponent<CBoxCollider>(entityThree, 10.0f, 1.0f, 10.0f);
	entityManager.AddComponent<CRigidBody>(entityThree, DYNAMIC, 1.0f, -4.0f);


	Entity& entityFour = entityManager.AddEntity("default");
	entityManager.AddComponent<CTransform>(entityFour, glm::vec3(4.0f, 4.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	entityManager.AddComponent<CPointLight>(entityFour, glm::vec3(0.0f, 0.5f, 0.0f), 1.0f);



	renderer.CompileShaders("objVertexShader.vert", "objFragmentShader.frag", renderer.objShaderID);
	renderer.CompileShaders("Shadows.vert", "Shadows.frag", renderer.shadowShaderID);
	renderer.CompileShaders("UIShader.vert", "UIShader.frag", renderer.UIShaderID);


	//runs physics functions more/less () needs to be 1 or above
	physics.accuracy = 10;

	while (!glfwWindowShouldClose(window))
	{
		//activate shader before SENDING the uniforms

		//3d matrices
		proj = glm::perspective(glm::radians(45.0f), (float)windowWidth / windowHeight, 0.1f, 10000.0f);

		view = input.CreateViewMat();

	
		//glCullFace(GL_FRONT);
		glDisable(GL_CULL_FACE);
		renderer.ShadowPass(entityManager);
		glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);

		
		renderer.LightingPass(windowWidth, windowHeight, proj, view, entityManager);
		

		renderer.HandlePointLights(input.cam.cameraPos, entityManager);
		renderer.HandleDirectionalLights(input.cam.cameraPos, entityManager);
		renderer.HandleSpotLights(input.cam.cameraPos, entityManager);


		renderer.DrawUI(entityManager);


		//pause everything
		while (input.inputs[GLFW_KEY_P])
		{
			glfwPollEvents();
		}


		//handle physics
		for (int i = 0; i < physics.accuracy;i++)
		{
			physics.HandleRigidBodies(entityManager, deltaTime);
			physics.CheckBoxCollisions(entityManager, deltaTime);
		}

		//input stuff
		input.handle_CameraMovement(deltaTime);

		//draws and takes pv Uniforms
		//renderer.Draw(proj, view, entityManager);

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