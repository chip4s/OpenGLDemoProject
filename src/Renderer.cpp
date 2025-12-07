#include "Renderer.h"

Renderer::Renderer()
{
	objShaderID = 0;
	shadowShaderID = 0;


	//resolution of shadow framebuffer
	size = 2048;
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


			//send farPlane to objFragShader as a uniform
			std::string FarPlaneStr = "pointLights[" + std::to_string(totalPointLights) + "].farPlane";
			int FarPlaneLocO = glGetUniformLocation(objShaderID, FarPlaneStr.c_str());
			glUniform1f(FarPlaneLocO, entityPointLight.farPlane);

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


void Renderer::CompileShaders(const char* vertShader, const char* fragShader, GLuint& shaderID)
{
	//for object shaders
	const char* vertexFile = vertShader;
	const char* fragmentFile = fragShader;

	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);
	//std::cout << vertexCodeO << "ov\n\n\n\n\n";
	//std::cout << fragmentCodeO << "of\n\n\n\n\n";

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	//Compiles sourcecode and creates vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	ShaderErrors(vertexShader, "VERTEX");

	//Same thing as above for fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	ShaderErrors(fragmentShader, "FRAGMENT");

	//Creates the shader program(only one type of program)
	shaderID = glCreateProgram();
	
	//Attaches both shaders to shader program and links program
	glAttachShader(shaderID, vertexShader);
	glAttachShader(shaderID, fragmentShader);
	glLinkProgram(shaderID);
	ShaderErrors(shaderID, "PROGRAM");


	//Deletes shaders after they are linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
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
				entityMesh.initializeBuffers();
			}
			if (entityTexture.isLoaded == false)
			{
				entityTexture.LoadTexture();
			}
			entityTexture.BindTexture(GL_TEXTURE0);
			glBindVertexArray(entityMesh.VAO);
			glDrawElements(GL_TRIANGLES, entityMesh.indices.size(), GL_UNSIGNED_INT, 0);
		}
	}
}

void Renderer::DrawUI(EntityManager& entityManager)
{
	//use UI shader
	glUseProgram(UIShaderID);

	//get uniform model matrix location in UI shader
	int modelLocUI = glGetUniformLocation(UIShaderID, "m");


	//get all comp.s needed
	auto& allTransforms = entityManager.GetComponentsByType<CTransform>();
	auto& allTextures = entityManager.GetComponentsByType<CTexture>();
	auto& allUIs = entityManager.GetComponentsByType<CUI>();

	//iterate through comp. list
	for (int i = 0; i < allUIs.size(); i++)
	{
		//check if entity has UI comp.
		CUI& UI = allUIs[i];
		if (UI.exists == false)
			continue;
		
		CTransform& transform = allTransforms[i];
		CTexture& texture = allTextures[i];

		
		//create and send model matrix based on transform comp.

		//create model matrix
		glm::mat4 model(1.0f);
		if (transform.exists)
		{
			model = glm::translate(model, transform.position);
			model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, transform.scale);
		}

		//send model matrix to Ui shader
		glUniformMatrix4fv(modelLocUI, 1, GL_FALSE, glm::value_ptr(model));


		//initialize UI buffers and textures if not initialized
		if (UI.isInitialized == false)
		{
			UI.initializeBuffers();
		}
		if (texture.isLoaded == false)
		{
			texture.LoadTexture();
		}

		//bind texture and draw
		texture.BindTexture(GL_TEXTURE8);
		glBindVertexArray(UI.VAO);
		glDrawElements(GL_TRIANGLES, UI.indices.size(), GL_UNSIGNED_INT, 0);
	}
}


void Renderer::ShadowPass(EntityManager& entityManager)
{
	glUseProgram(shadowShaderID);
	
	//First get list of pointLights to initialize shadow map
	auto& allPointLights = entityManager.GetComponentsByType<CPointLight>();
	for (int i = 0; i < allPointLights.size(); i++)
	{
		//cache point light
		CPointLight& pointLight = allPointLights[i];
		
		//if it doesn't exist skip this iteration of the loop
		if (pointLight.exists == false)
		{
			continue;
		}

		//If pointLight's shadow map isn't initialized
		if (pointLight.shadowMapIsInitialized == false)
		{
			//initializes fbo and cube map and sets isinitialized to true
			pointLight.ShadowBufferInitialize(2048, 0.51f, 40.0f);
		}
		
		
		//Next send farplane to shadowFragShader as a uniform
		int FarPlaneLocS = glGetUniformLocation(shadowShaderID, "farplane");
		glUniform1f(FarPlaneLocS, pointLight.farPlane);


		//then draw scenes to shadow shader on every cube face (6)


		//cache point light's transform and postion
		auto& entityTransforms = entityManager.GetComponentsByType<CTransform>();

		auto& pointLightTransform = entityTransforms[i];

		glm::vec3 lightPos = pointLightTransform.position;

		//send point light's postion as light pos in this draw call
		int LightPosLoc = glGetUniformLocation(shadowShaderID, "lightPos");
		glUniform3fv(LightPosLoc, 1, glm::value_ptr(lightPos));

		glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
		
		//draws entire scene six times for each texture in cube map (6) 
		for (int j = 0; j < 6; j++)
		{
			//bind shadow buffer so you draw to it
			pointLight.ShadowBufferWriteBind(pointLight.cameraDirections[j].cubeFace);


			//clear buffers
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

			//create a new view matrix to later send to shadow shader (per face)
			glm::mat4 lightView = glm::mat4(1.0f);

			lightView = glm::lookAt(lightPos,
									lightPos + pointLight.cameraDirections[j].target,
									pointLight.cameraDirections[j].up);


			//RENDER all meshes and set uniforms
			//only M mat4
			//pasted from draw method

			//send light's view matrix
			int viewLocS = glGetUniformLocation(shadowShaderID, "v");
			glUniformMatrix4fv(viewLocS, 1, GL_FALSE, glm::value_ptr(lightView));

			//send perspective projection matrix from pointlight
			int projLocS = glGetUniformLocation(shadowShaderID, "p");
			glUniformMatrix4fv(projLocS, 1, GL_FALSE, glm::value_ptr(pointLight.shadowPerspective));

			//send model matrix per object for every object you draw
			int modelLocS = glGetUniformLocation(shadowShaderID, "m");

			//iterates through entity meshes
			auto& meshesS = entityManager.GetComponentsByType<CMesh>();
			auto& transformsS = entityManager.GetComponentsByType<CTransform>();
			for (int k = 0; k < meshesS.size();k++)
			{
				//check if object has mesh to draw
				CMesh& entityMesh = meshesS[k];
				if (entityMesh.exists)
				{
					CTransform& entityTransform = transformsS[k];

					//If entity/object has a transform, create a model matrix with its
					//position, rotation, and scale from transform component
					//else just send an identity matrix
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
					glUniformMatrix4fv(modelLocS, 1, GL_FALSE, glm::value_ptr(model));

					if (entityMesh.isInitialized == false)
					{
						entityMesh.initializeBuffers();
					}

					//bind entity mesh VAO and draw the object
					glBindVertexArray(entityMesh.VAO);
					glDrawElements(GL_TRIANGLES, entityMesh.indices.size(), GL_UNSIGNED_INT, 0);
				}
			}
		}
	}
}

void Renderer::LightingPass(unsigned int windowWidth, unsigned int windowHeight, glm::mat4 proj, glm::mat4 view, EntityManager& entityManager)
{
	glUseProgram(objShaderID);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, windowWidth, windowHeight);


	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//first send the 4 pointLights with shadows and an initialized cube map to
	//objFragShader uniform shadowMapSampler
	auto& allPointLights = entityManager.GetComponentsByType<CPointLight>();

	//limit for how many point lights can have shadows (also in objFragShader)
	int maxShadowCasters = 4;
	//amount currently in loop of point lights casting shadows
	int currentShadowCaster = 0;
	

	//Code to set textureUnits to each samplerCube in shadowMapSampler (could be moved out of loop)
	int ShadowMapSamplerLoc = glGetUniformLocation(objShaderID, "shadowMapSampler");
	//uses tex units 1 - 4
	GLint texUnits[4] = {1, 2, 3, 4};
	glUniform1iv(ShadowMapSamplerLoc, 4, texUnits);
	int totalPointLights = 0;
	for (int i = 0; i < allPointLights.size(); i++)
	{
		//Cache current point light
		auto& pointLight = allPointLights[i];
		
		//checks if point light exists
		if (pointLight.exists == false)
		{
			continue;
		}

		//checks there are already max shadow casters bound
		if (currentShadowCaster == maxShadowCasters - 1)
		{
			continue;
		}

		//below code sets cube map index of pointLight
		std::string cubeMapIndexStr = "pointLights[" + std::to_string(totalPointLights++) + "].cubeMapIndex";
		int CubeMapIndexLoc = glGetUniformLocation(objShaderID, cubeMapIndexStr.c_str());

		glUniform1i(CubeMapIndexLoc, currentShadowCaster);


		//below code adds a shadow caster / binds its cubemap for reading to a certain unit
		// 
		//bind pointLight shadow buffer for reading
		pointLight.ShadowBufferReadBind(GL_TEXTURE1 + currentShadowCaster);
		currentShadowCaster++;
	}

	//after sending cubemap Textures of pointlight shadows
	// draw the scene normally

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