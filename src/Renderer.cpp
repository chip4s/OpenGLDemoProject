#include "Renderer.h"

Renderer::Renderer()
{
	objShaderID = 0;
	lightShaderID = 0;
}
int Renderer::AddCube(float pX, float pY, float pZ, obj t)//fourth parameter is 0 if obj and > 1 if light
{
	//create new object to later add data then put it in vector
	Object c;

	c.model = glm::translate(glm::mat4(1.0f), glm::vec3(pX, pY, pZ));
	//populates vertices and indices o
	c.indices.reserve(36);
	for (int f = 0; f < 24; f += 4)
	{
		c.indices.push_back(0 + f);
		c.indices.push_back(1 + f);
		c.indices.push_back(2 + f);
		c.indices.push_back(2 + f);
		c.indices.push_back(3 + f);
		c.indices.push_back(0 + f);
	}
	c.verts.reserve(24);
	//                       pos               normal
	//front face
	c.verts.push_back({-0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f});
	c.verts.push_back({-0.5f, 0.5f, 0.5f,  0.0f, 0.0f, 1.0f});
	c.verts.push_back({0.5f, 0.5f, 0.5f,  0.0f, 0.0f, 1.0f});
	c.verts.push_back({0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f});
	//right face
	c.verts.push_back({0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 0.0f});
	c.verts.push_back({0.5f, 0.5f, 0.5f,  1.0f, 0.0f, 0.0f});
	c.verts.push_back({0.5f, 0.5f, -0.5f,  1.0f, 0.0f, 0.0f});
	c.verts.push_back({0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f});
	//left face
	c.verts.push_back({-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f});
	c.verts.push_back({-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f});
	c.verts.push_back({-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f});
	c.verts.push_back({-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f});
	//top face
	c.verts.push_back({-0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f});
	c.verts.push_back({-0.5f, 0.5f, -0.5f,  0.0f, 1.0f, 0.0f});
	c.verts.push_back({0.5f, 0.5f, -0.5f,  0.0f, 1.0f, 0.0f});
	c.verts.push_back({0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f});
	//back face
	c.verts.push_back({0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f});
	c.verts.push_back({0.5f, 0.5f, -0.5f,  0.0f, 0.0f, -1.0f});
	c.verts.push_back({-0.5f, 0.5f, -0.5f,  0.0f, 0.0f, -1.0f});
	c.verts.push_back({-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f});
	//bottom face
	c.verts.push_back({-0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f});
	c.verts.push_back({-0.5f, -0.5f, 0.5f,  0.0f, -1.0f, 0.0f});
	c.verts.push_back({0.5f, -0.5f, 0.5f,  0.0f, -1.0f, 0.0f});
	c.verts.push_back({0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f});
	//adds object to renderer's list of all objects/lights depending on which will later get bufferIDs and be drawn
	if (t > 0)
	{
		lightObjects.push_back(c);
		return lightObjects.size() - 1;//returns index into where the object is for later reference
	}
	else if (t == OBJECT)
	{
		objects.push_back(c);
		return objects.size() - 1;
	}
}
void Renderer::AddPointLight(int objInd, glm::vec3 color,float lin, float quad)
{ 
	PointLight p =
	{
		glm::vec3(lightObjects[objInd].model[3][0],lightObjects[objInd].model[3][1], lightObjects[objInd].model[3][2]),//pos of light obj which i need to update every frame
		color,
		objInd,
		1.0f,//constant for attenuation is always 1.0f(I think)
		lin,
		quad,
	};
	pointLights.push_back(p);
}
void Renderer::SetCubeModelMat(int index, glm::mat4 mod, obj t)
{
	if (t == OBJECT)
	{
		objects[index].model = objects[index].model * mod;
	}
	else if (t > 0)
	{
		lightObjects[index].model = lightObjects[index].model * mod;
	}
}
//sends light position uniform to obj frag shader for lighting
void Renderer::HandlePointLights(glm::vec3 camPos)//updates pointlight position and sends pointlight struct to frag shader
{
	int viewPosLoc = glGetUniformLocation(objShaderID, "viewPos");
	glUniform3f(viewPosLoc, camPos.x, camPos.y, camPos.z);
	
	int maxPointLightsLoc = glGetUniformLocation(objShaderID, "maxPointLights");
	glUniform1i(maxPointLightsLoc, pointLights.size());


	for (int i = 0; i < pointLights.size(); i++)
	{
		//updates position of pointlight with obj
		pointLights[i].lightPos = glm::vec3(lightObjects[pointLights[i].objIndex].model[3][0], lightObjects[pointLights[i].objIndex].model[3][1], lightObjects[pointLights[i].objIndex].model[3][2]);

		//sends attenuation values per light
		std::string AttenLStr = "pointLights[" + std::to_string(i) + "].AttenLinear";
		int AttenLinearLoc = glGetUniformLocation(objShaderID, AttenLStr.c_str());

		std::string AttenQStr = "pointLights[" + std::to_string(i) + "].AttenQuad";
		int AttenQuadLoc = glGetUniformLocation(objShaderID, AttenQStr.c_str());

		glUniform1f(AttenLinearLoc, pointLights[i].linear);
		glUniform1f(AttenQuadLoc, pointLights[i].quadratic);

		//sends light position
		std::string LightPStr = "pointLights[" + std::to_string(i) + "].lightPosition";
		int LightPosLoc = glGetUniformLocation(objShaderID, LightPStr.c_str());

		//sends light color
		std::string LightCStr = "pointLights[" + std::to_string(i) + "].lightColor";
		int LightColorLoc = glGetUniformLocation(objShaderID, LightCStr.c_str());

		glUniform3fv(LightPosLoc, 1, glm::value_ptr(pointLights[i].lightPos));

		glUniform3fv(LightColorLoc, 1, glm::value_ptr(pointLights[i].lightColor));
	}
}
void Renderer::AddDirectionalLight(glm::vec3 dir, glm::vec3 lightColor)
{
	DirectionalLight d =
	{
		dir,
		lightColor, 
	};
	directionalLights.push_back(d);
}
void Renderer::HandleDirectionalLights(glm::vec3 camPos)
{
	int viewPosLoc = glGetUniformLocation(objShaderID, "viewPos");
	glUniform3f(viewPosLoc, camPos.x, camPos.y, camPos.z);

	int maxDirectionalLightsLoc = glGetUniformLocation(objShaderID, "maxDirectionalLights");
	glUniform1i(maxDirectionalLightsLoc, directionalLights.size());

	for (int i = 0; i < directionalLights.size(); i++)
	{
		//sends light direction
		std::string lightDir = "directionalLights[" + std::to_string(i) + "].direction";
		int LightDirLoc = glGetUniformLocation(objShaderID, lightDir.c_str());

		glUniform3fv(LightDirLoc, 1, glm::value_ptr(directionalLights[i].lightDirection));


		//sends light color
		std::string lightCol = "directionalLights[" + std::to_string(i) + "].lightDirColor";
		int LightColLoc = glGetUniformLocation(objShaderID, lightCol.c_str());

		glUniform3fv(LightColLoc, 1, glm::value_ptr(directionalLights[i].lightColor));
	}
}
void Renderer::initialize()//creates buffers in all objects and lights
{
	for (Object& o : objects)
	{
		//generates the vao
		glGenVertexArrays(1, &o.VAO);
		glBindVertexArray(o.VAO);
		//generates the vbo
		glGenBuffers(1, &o.VBO);
		glBindBuffer(GL_ARRAY_BUFFER, o.VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * o.verts.size(), o.verts.data(), GL_STATIC_DRAW);

		//generates ebo
		glGenBuffers(1, &o.EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, o.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * o.indices.size(), o.indices.data(), GL_STATIC_DRAW);

		//sets attribute pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	int counter = 0;
	for (Object& l : lightObjects)
	{
		//generates the vao
		glGenVertexArrays(1, &l.VAO);
		glBindVertexArray(l.VAO);
		//generates the vbo
		glGenBuffers(1, &l.VBO);
		glBindBuffer(GL_ARRAY_BUFFER, l.VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * l.verts.size(), l.verts.data(), GL_STATIC_DRAW);

		//generates ebo
		glGenBuffers(1, &l.EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, l.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * l.indices.size(), l.indices.data(), GL_STATIC_DRAW);

		//sets attribute pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
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

	//Deletes shaders after they are linked
	glDeleteShader(vertexShaderO);
	glDeleteShader(fragmentShaderO);
	//std::cout << "object shaders compile and work\n";
	
	//for light shaders
	const char* vertexFileL = "litVertexShader.vert";
	const char* fragmentFileL = "litFragmentShader.frag";

	std::string vertexCodeL = get_file_contents(vertexFileL);
	std::string fragmentCodeL = get_file_contents(fragmentFileL);
	//std::cout << vertexCodeL << "lv\n\n\n\n\n";
	//std::cout << fragmentCodeL << "lf\n\n\n\n\n";

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
	lightShaderID = glCreateProgram();

	//Attaches both shaders to shader program and links program
	glAttachShader(lightShaderID, vertexShaderL);
	glAttachShader(lightShaderID, fragmentShaderL);
	glLinkProgram(lightShaderID);
	ShaderErrors(lightShaderID, "PROGRAM");

	//Deletes shaders after they are linked
	glDeleteShader(vertexShaderO);
	glDeleteShader(fragmentShaderO);
	//std::cout << "light shaders compile and work\n";
}
void Renderer::ShaderErrors(unsigned int shader, const  char* type)
{
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
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << std::endl;
		}
	}
}

void Renderer::Draw(glm::mat4 proj, glm::mat4 view)
{
	//iterates through lights and sends model mat4 + draws them
	glUseProgram(lightShaderID);
	int viewLocL = glGetUniformLocation(lightShaderID, "v");
	glUniformMatrix4fv(viewLocL, 1, GL_FALSE, glm::value_ptr(view));

	int projLocL = glGetUniformLocation(lightShaderID, "p");
	glUniformMatrix4fv(projLocL, 1, GL_FALSE, glm::value_ptr(proj));

	int modelLocL = glGetUniformLocation(lightShaderID, "m");
	for (Object& l : lightObjects)
	{
		glUniformMatrix4fv(modelLocL, 1, GL_FALSE, glm::value_ptr(l.model));

		glBindVertexArray(l.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, l.VBO);//may only need to call vao bind but unsure
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, l.EBO);
		glDrawElements(GL_TRIANGLES, l.indices.size(), GL_UNSIGNED_INT, 0);
	}

	//same for regular objects
	glUseProgram(objShaderID);
	int viewLocO = glGetUniformLocation(objShaderID, "v");
	glUniformMatrix4fv(viewLocO, 1, GL_FALSE, glm::value_ptr(view));

	int projLocO = glGetUniformLocation(objShaderID, "p");
	glUniformMatrix4fv(projLocO, 1, GL_FALSE, glm::value_ptr(proj));

	int modelLocO = glGetUniformLocation(objShaderID, "m");
	for (Object& o : objects)
	{
		//send model matrix
		glUniformMatrix4fv(modelLocO, 1, GL_FALSE, glm::value_ptr(o.model));

		glBindVertexArray(o.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, o.VBO);//may only need to call vao bind but unsure
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, o.EBO);
		glDrawElements(GL_TRIANGLES, o.indices.size(), GL_UNSIGNED_INT, 0);
	}
}
Renderer::~Renderer()
{
	//goes through all objects and deletes them
	glUseProgram(0);
	for (Object& o : objects)
	{
		glDeleteBuffers(1, &o.VBO);
		glDeleteBuffers(1, &o.EBO);
		glDeleteVertexArrays(1, &o.VAO);
	}
	glDeleteProgram(objShaderID);
	for (Object& l : lightObjects)
	{
		glDeleteBuffers(1, &l.VBO);
		glDeleteBuffers(1, &l.EBO);
		glDeleteVertexArrays(1, &l.VAO);
	}
	glDeleteProgram(lightShaderID);
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