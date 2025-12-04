#include "Renderer.h"

Renderer::Renderer()
{
	objShaderID = 0;
	shadowShaderID = 0;


	//resolution of shadow framebuffer
	size = 1024;
}

//sends light position uniform to obj frag shader for lighting
void Renderer::HandlePointLights(glm::vec3 camPos, EntityManager& entityManager)//updates pointlight position and sends pointlight struct to frag shader
{
	int viewPosLoc = glGetUniformLocation(objShaderID, "viewPos");
	glUniform3f(viewPosLoc, camPos.x, camPos.y, camPos.z);
	
	int totalPointLights = 0;

	//iterates through pointlights and sends light data
	//assums pointlight has transform component
	auto& pointLights = entityManager.GetComponentsByType<CPointLight>();
	auto& transforms = entityManager.GetComponentsByType<CTransform>();

	for (int i = 0; i < pointLights.size();i++)
	{
		CPointLight& entityPointLight = pointLights[i];
		CTransform& entityTransform = transforms[i];
		if (entityPointLight.exists == true)
		{
			//sends attenuation values per light and intensity
			std::string AttenLStr = "pointLights[" + std::to_string(totalPointLights) + "].AttenLinear";
			int AttenLinearLoc = glGetUniformLocation(objShaderID, AttenLStr.c_str());

			std::string AttenQStr = "pointLights[" + std::to_string(totalPointLights) + "].AttenQuad";
			int AttenQuadLoc = glGetUniformLocation(objShaderID, AttenQStr.c_str());

			std::string IntensityStr = "pointLights[" + std::to_string(totalPointLights) + "].intensity";
			int IntensityLoc = glGetUniformLocation(objShaderID, IntensityStr.c_str());


			glUniform1f(AttenLinearLoc, entityPointLight.linear);
			glUniform1f(AttenQuadLoc, entityPointLight.quadratic);
			glUniform1f(IntensityLoc, entityPointLight.intensity);


			//sends light position
			std::string LightPStr = "pointLights[" + std::to_string(totalPointLights) + "].lightPos";
			int LightPosLoc = glGetUniformLocation(objShaderID, LightPStr.c_str());

			glUniform3fv(LightPosLoc, 1, glm::value_ptr(entityTransform.position));


			//sends light color
			std::string LightCStr = "pointLights[" + std::to_string(totalPointLights) + "].lightColor";
			int LightColorLoc = glGetUniformLocation(objShaderID, LightCStr.c_str());

			glUniform3fv(LightColorLoc, 1, glm::value_ptr(entityPointLight.lightColor));


			totalPointLights++;
		}
	}

	int maxPointLightsLoc = glGetUniformLocation(objShaderID, "maxPointLights");
	glUniform1i(maxPointLightsLoc, totalPointLights);
}

void Renderer::HandleDirectionalLights(glm::vec3 camPos, EntityManager& entityManager)
{
	int viewPosLoc = glGetUniformLocation(objShaderID, "viewPos");
	glUniform3f(viewPosLoc, camPos.x, camPos.y, camPos.z);


	int totalDirectionalLights = 0;// current amount of directional lights


	auto& directionalLights = entityManager.GetComponentsByType<CDirectionalLight>();
	auto& transforms = entityManager.GetComponentsByType<CTransform>();

	for (int i = 0; i < directionalLights.size();i++)
	{
		CDirectionalLight& entityDirectionalLight = directionalLights[i];

		if (entityDirectionalLight.exists == true)
		{
			std::string lightDir = "directionalLights[" + std::to_string(totalDirectionalLights) + "].direction";
			int LightDirLoc = glGetUniformLocation(objShaderID, lightDir.c_str());
	

			//calc. direction off of transform
			CTransform& entityTransform = transforms[i];
			//set to default direction which is down
			glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);
			if (entityTransform.exists == true)
			{
				//calculate rotation to change direction by
				glm::mat4 rotDirection(1.0f);
				rotDirection = glm::rotate(rotDirection, glm::radians(entityTransform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				rotDirection = glm::rotate(rotDirection, glm::radians(entityTransform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				rotDirection = glm::rotate(rotDirection, glm::radians(entityTransform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

				//apply rotation to direction
				glm::vec4 newDirection = rotDirection * glm::vec4(direction, 1.0f);
				direction = glm::vec3(newDirection.x, newDirection.y, newDirection.z);
			}

			glUniform3fv(LightDirLoc, 1, glm::value_ptr(direction));


			//sends light color
			std::string lightCol = "directionalLights[" + std::to_string(totalDirectionalLights) + "].lightDirColor";
			int LightColLoc = glGetUniformLocation(objShaderID, lightCol.c_str());

			glUniform3fv(LightColLoc, 1, glm::value_ptr(entityDirectionalLight.lightColor));


			//send light intensity
			std::string lightIntensity = "directionalLights[" + std::to_string(totalDirectionalLights) + "].intensity";
			int LightIntensityLoc = glGetUniformLocation(objShaderID, lightIntensity.c_str());

			glUniform1f(LightIntensityLoc, entityDirectionalLight.intensity);

			totalDirectionalLights++;
		}
	}


	int maxDirectionalLightsLoc = glGetUniformLocation(objShaderID, "maxDirectionalLights");
	glUniform1i(maxDirectionalLightsLoc, totalDirectionalLights);
}

void Renderer::HandleSpotLights(glm::vec3 camPos, EntityManager& entityManager)
{
	int viewPosLoc = glGetUniformLocation(objShaderID, "viewPos");
	glUniform3f(viewPosLoc, camPos.x, camPos.y, camPos.z);


	int totalSpotLights = 0;


	auto& spotLights = entityManager.GetComponentsByType<CSpotLight>();
	auto& transforms = entityManager.GetComponentsByType<CTransform>();

	for (int i = 0; i < spotLights.size();i++)
	{
		CTransform& entityTransform = transforms[i];
		CSpotLight& entitySpotLight = spotLights[i];

		if (entityTransform.exists == true && entitySpotLight.exists == true)
		{
			//sends light position
			std::string lightPos = "spotLights[" + std::to_string(totalSpotLights) + "].lightPos";
			int LightPosLoc = glGetUniformLocation(objShaderID, lightPos.c_str());

			glUniform3fv(LightPosLoc, 1, glm::value_ptr(entityTransform.position));

			//sends light color
			std::string lightCol = "spotLights[" + std::to_string(totalSpotLights) + "].lightColor";
			int LightColLoc = glGetUniformLocation(objShaderID, lightCol.c_str());

			glUniform3fv(LightColLoc, 1, glm::value_ptr(entitySpotLight.lightColor));


			//sends light direction

			//calculates direction based off transform component
			glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);//default direction
		
			//calculate rotation to change direction by
			glm::mat4 rotDirection(1.0f);
			rotDirection = glm::rotate(rotDirection, glm::radians(entityTransform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			rotDirection = glm::rotate(rotDirection, glm::radians(entityTransform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			rotDirection = glm::rotate(rotDirection, glm::radians(entityTransform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

				//apply rotation to direction
			glm::vec4 newDirection = rotDirection * glm::vec4(direction, 1.0f);
			direction = glm::vec3(newDirection.x, newDirection.y, newDirection.z);


			//sends light direction
			std::string lightDir = "spotLights[" + std::to_string(totalSpotLights) + "].lightDirection";
			int LightDirLoc = glGetUniformLocation(objShaderID, lightDir.c_str());

			glUniform3fv(LightDirLoc, 1, glm::value_ptr(direction));


			//sends light cutoff
			std::string lightCut = "spotLights[" + std::to_string(totalSpotLights) + "].cutOff";
			int LightCutLoc = glGetUniformLocation(objShaderID, lightCut.c_str());

			glUniform1f(LightCutLoc, entitySpotLight.cutOff);
			std::cout << entitySpotLight.outerCutOff << "\n\n";


			//sends light outerCutOff
			std::string lightOutCut = "spotLights[" + std::to_string(totalSpotLights) + "].outerCutOff";
			int LightOutCutLoc = glGetUniformLocation(objShaderID, lightCut.c_str());

			glUniform1f(LightOutCutLoc, entitySpotLight.outerCutOff);


			//sends light intensity
			std::string lightIntensity = "spotLights[" + std::to_string(totalSpotLights) + "].intensity";
			int lightIntensityLoc = glGetUniformLocation(objShaderID, lightIntensity.c_str());

			glUniform1f(lightIntensityLoc, entitySpotLight.intensity);


			totalSpotLights++;
		}
	}


	int maxSpotLightsLoc = glGetUniformLocation(objShaderID, "maxSpotLights");

	glUniform1i(maxSpotLightsLoc, totalSpotLights);
}


void Renderer::CompileShaders()
{
	//for object shaders
	const char* vertexFileO = "objVertexShader.vert";
	const char* fragmentFileO = "objFragmentShader.frag";

	std::string vertexCodeO = get_file_contents(vertexFileO);
	std::string fragmentCodeO = get_file_contents(fragmentFileO);
	//std::cout << vertexCodeO << "ov\n\n\n\n\n";
	//std::cout << fragmentCodeO << "of\n\n\n\n\n";

	const char* vertexSourceO = vertexCodeO.c_str();
	const char* fragmentSourceO = fragmentCodeO.c_str();

	//Compiles sourcecode and creates vertex shader
	GLuint vertexShaderO = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderO, 1, &vertexSourceO, NULL);
	glCompileShader(vertexShaderO);
	ShaderErrors(vertexShaderO, "VERTEX");
	//Same thing as above for fragment shader
	GLuint fragmentShaderO = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderO, 1, &fragmentSourceO, NULL);
	glCompileShader(fragmentShaderO);
	ShaderErrors(fragmentShaderO, "FRAGMENT");

	//Creates the shader program(only one type of program)
	objShaderID = glCreateProgram();
	
	//Attaches both shaders to shader program and links program
	glAttachShader(objShaderID, vertexShaderO);
	glAttachShader(objShaderID, fragmentShaderO);
	glLinkProgram(objShaderID);
	ShaderErrors(objShaderID, "PROGRAM");
	//std::cout << glGetError() << "\n\n";

	//Deletes shaders after they are linked
	glDeleteShader(vertexShaderO);
	glDeleteShader(fragmentShaderO);
	//std::cout << "object shaders compile and work\n";



	//same for shadow shaders
	//for object shaders
	const char* vertexFileL = "Shadows.vert";
	const char* fragmentFileL = "Shadows.frag";

	std::string vertexCodeL = get_file_contents(vertexFileL);
	std::string fragmentCodeL = get_file_contents(fragmentFileL);
	//std::cout << vertexCodeO << "ov\n\n\n\n\n";
	//std::cout << fragmentCodeO << "of\n\n\n\n\n";

	const char* vertexSourceL = vertexCodeL.c_str();
	const char* fragmentSourceL = fragmentCodeL.c_str();

	//Compiles sourcecode and creates vertex shader
	GLuint vertexShaderL = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderL, 1, &vertexSourceL, NULL);
	glCompileShader(vertexShaderL);
	ShaderErrors(vertexShaderL, "VERTEX");
	//Same thing as above for fragment shader
	GLuint fragmentShaderL = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderL, 1, &fragmentSourceL, NULL);
	glCompileShader(fragmentShaderL);
	ShaderErrors(fragmentShaderL, "FRAGMENT");

	//Creates the shader program(only one type of program)
	shadowShaderID = glCreateProgram();

	//Attaches both shaders to shader program and links program
	glAttachShader(shadowShaderID, vertexShaderL);
	glAttachShader(shadowShaderID, fragmentShaderL);
	glLinkProgram(shadowShaderID);
	ShaderErrors(shadowShaderID, "PROGRAM");
	//std::cout << glGetError() << "\n\n";

	//Deletes shaders after they are linked
	glDeleteShader(vertexShaderL);
	glDeleteShader(fragmentShaderL);
	//std::cout << "object shaders compile and work\n";
	
}

void Renderer::ShaderErrors(unsigned int shader, const  char* type)
{
	//std::cout << glGetError() << " used shader errors\n\n";
	GLint hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);;
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << std::endl;
		}
		//std::cout << hasCompiled << " code\n";
	}
}

void Renderer::Draw(glm::mat4 proj, glm::mat4 view, EntityManager& entityManager)
{
	//iterates through entities meshes and sends model mat4 + draws them
	glUseProgram(objShaderID);

	int viewLocO = glGetUniformLocation(objShaderID, "v");
	glUniformMatrix4fv(viewLocO, 1, GL_FALSE, glm::value_ptr(view));

	int projLocO = glGetUniformLocation(objShaderID, "p");
	glUniformMatrix4fv(projLocO, 1, GL_FALSE, glm::value_ptr(proj));

	int modelLocO = glGetUniformLocation(objShaderID, "m");
	//iterates through entity meshes
	auto& meshesO = entityManager.GetComponentsByType<CMesh>();
	auto& transformsO = entityManager.GetComponentsByType<CTransform>();
	auto& texturesO = entityManager.GetComponentsByType<CTexture>();
	for (int i = 0; i < meshesO.size();i++)
	{
		//check if object has mesh to draw
		CMesh& entityMesh = meshesO[i];
		if (entityMesh.exists)
		{
			CTransform& entityTransform = transformsO[i];
			CTexture& entityTexture = texturesO[i];
			glm::mat4 model(1.0f);
			if (entityTransform.exists)
			{
				model = glm::translate(model, entityTransform.position);
				model = glm::rotate(model, glm::radians(entityTransform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(entityTransform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(entityTransform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				model = glm::scale(model, entityTransform.scale);
			}

			//sends model matrix 
			glUniformMatrix4fv(modelLocO, 1, GL_FALSE, glm::value_ptr(model));


			if (entityMesh.isInitialized == false)
			{
				entityMesh.initializeMesh();
			}
			if (entityTexture.isLoaded == false)
			{
				entityTexture.LoadTexture();
			}
			entityTexture.BindTexture();
			glBindVertexArray(entityMesh.VAO);
			glDrawElements(GL_TRIANGLES, entityMesh.indices.size(), GL_UNSIGNED_INT, 0);
		}
	}
}

void Renderer::ShadowBufferInitialize()
{
	//create perspective matrix
	shadowPerspective = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 25.0f);


	//create cube map to store in all directions
	glGenTextures(1, &shadowCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubeMap);

	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT32F, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	//create frame buffer
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowCubeMap, 0);


	//don't write or read color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << glCheckFramebufferStatus(GL_FRAMEBUFFER) << " not done\n\n";
	}

}

void Renderer::ShadowBufferWriteBind(GLenum cubeFace)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
	glViewport(0, 0, size, size);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, cubeFace, shadowCubeMap, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void Renderer::ShadowBufferReadBind(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubeMap);
}

void Renderer::ShadowPass(EntityManager& entityManager)
{
	glUseProgram(shadowShaderID);

	//uniform for light position as a vec3
	//stand in values for now (HardCoded)
	auto& entityTransforms = entityManager.GetComponentsByType<CTransform>();

	glm::vec3 lightPos = entityTransforms[1].position;


	int LightPosLoc = glGetUniformLocation(shadowShaderID, "lightPos");
	glUniform3fv(LightPosLoc, 1, glm::value_ptr(lightPos));

	glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);

	for (int i = 0; i < 6; i++)
	{
		ShadowBufferWriteBind(cameraDirections[i].cubeFace);

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		glm::mat4 lightView = glm::mat4(1.0f);
		//UNSURE: add light pos to second parameter?
		lightView = glm::lookAt(lightPos, lightPos + cameraDirections[i].target, cameraDirections[i].up);


		//RENDER all meshes and set uniforms
		//only M mat4
		//pasted from draw method

		int viewLocO = glGetUniformLocation(shadowShaderID, "v");
		glUniformMatrix4fv(viewLocO, 1, GL_FALSE, glm::value_ptr(lightView));

		int projLocO = glGetUniformLocation(shadowShaderID, "p");
		glUniformMatrix4fv(projLocO, 1, GL_FALSE, glm::value_ptr(shadowPerspective));

		int modelLocO = glGetUniformLocation(shadowShaderID, "m");

		//iterates through entity meshes
		auto& meshesO = entityManager.GetComponentsByType<CMesh>();
		auto& transformsO = entityManager.GetComponentsByType<CTransform>();
		for (int i = 0; i < meshesO.size();i++)
		{
			//check if object has mesh to draw
			CMesh& entityMesh = meshesO[i];
			if (entityMesh.exists)
			{
				CTransform& entityTransform = transformsO[i];

				glm::mat4 model(1.0f);
				if (entityTransform.exists)
				{
					model = glm::translate(model, entityTransform.position);
					model = glm::rotate(model, glm::radians(entityTransform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
					model = glm::rotate(model, glm::radians(entityTransform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
					model = glm::rotate(model, glm::radians(entityTransform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
					model = glm::scale(model, entityTransform.scale);
				}

				//sends model matrix 
				glUniformMatrix4fv(modelLocO, 1, GL_FALSE, glm::value_ptr(model));

				if (entityMesh.isInitialized == false)
				{
					entityMesh.initializeMesh();
				}

				glBindVertexArray(entityMesh.VAO);
				glDrawElements(GL_TRIANGLES, entityMesh.indices.size(), GL_UNSIGNED_INT, 0);
			}
		}
	}
}

void Renderer::LightingPass(unsigned int windowWidth, unsigned int windowHeight, glm::mat4 proj, glm::mat4 view, EntityManager& entityManager)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, windowWidth, windowHeight);


	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	ShadowBufferReadBind(GL_TEXTURE1);

	Draw(proj, view, entityManager);
}

Renderer::~Renderer()
{
	//deletes shaders
	glUseProgram(0);

	glDeleteProgram(objShaderID);

	glDeleteProgram(shadowShaderID);
}
//function to read glsl shader files
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		if (in.tellg() > 0)
		{
			contents.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&contents[0], contents.size());
			in.close();
			return(contents);
		}
		else
		{
			std::cout << "failed this way\n";
			return "not found";
		}
	}
	std::cout << "could not find shader at file\n";
	return "not found";
}