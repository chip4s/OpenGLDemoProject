#include "Renderer.h"

Renderer::Renderer()
{
	VBO = 0;
	VAO = 0;
	EBO = 0;
	shaderID = 0;
}
void Renderer::AddCube(float pX, float pY, float pZ)
{
	float posX = pX;
	float posY = pY;
	float posZ = pZ;
	
	indices.reserve(36);
	for (int f = 0; f < 24; f += 4)
	{
		indices.push_back(verts.size() * 3 + 0 + f);
		indices.push_back(verts.size() * 3 + 1 + f);
		indices.push_back(verts.size() * 3 + 2 + f);
		indices.push_back(verts.size() * 3 + 2 + f);
		indices.push_back(verts.size() * 3 + 3 + f);
		indices.push_back(verts.size() * 3 + 0 + f);
	}
	verts.reserve(24);
	//                pos       uv
	//front face
	verts.push_back({0.0f + posX, 0.0f + posY, 0.0f + posZ,  0.0f, 0.0f });
	verts.push_back({0.0f + posX, 1.0f + posY, 0.0f + posZ,  0.0f, 1.0f });
	verts.push_back({1.0f + posX, 1.0f + posY, 0.0f + posZ,  1.0f, 1.0f });
	verts.push_back({1.0f + posX, 0.0f + posY, 0.0f + posZ,  1.0f, 0.0f });
	//right face
	verts.push_back({1.0f + posX, 0.0f + posY, 0.0f + posZ,  0.0f, 0.0f });
	verts.push_back({1.0f + posX, 1.0f + posY, 0.0f + posZ,  0.0f, 1.0f });
	verts.push_back({1.0f + posX, 1.0f + posY, 1.0f + posZ,  1.0f, 1.0f });
	verts.push_back({1.0f + posX, 0.0f + posY, 1.0f + posZ,  1.0f, 0.0f });
	//left face
	verts.push_back({0.0f + posX, 0.0f + posY, 1.0f + posZ,  0.0f, 0.0f });
	verts.push_back({0.0f + posX, 1.0f + posY, 1.0f + posZ,  0.0f, 1.0f });
	verts.push_back({0.0f + posX, 1.0f + posY, 0.0f + posZ,  1.0f, 1.0f });
	verts.push_back({0.0f + posX, 0.0f + posY, 0.0f + posZ,  1.0f, 0.0f });
	//top face
	verts.push_back({0.0f + posX, 1.0f + posY, 0.0f + posZ,  0.0f, 0.0f });
	verts.push_back({0.0f + posX, 1.0f + posY, 1.0f + posZ,  0.0f, 1.0f });
	verts.push_back({1.0f + posX, 1.0f + posY, 1.0f + posZ,  1.0f, 1.0f });
	verts.push_back({1.0f + posX, 1.0f + posY, 0.0f + posZ,  1.0f, 0.0f });
	//back face
	verts.push_back({1.0f + posX, 0.0f + posY, 1.0f + posZ,  0.0f, 0.0f });
	verts.push_back({1.0f + posX, 1.0f + posY, 1.0f + posZ,  0.0f, 1.0f });
	verts.push_back({0.0f + posX, 1.0f + posY, 1.0f + posZ,  1.0f, 1.0f });
	verts.push_back({0.0f + posX, 0.0f + posY, 1.0f + posZ,  1.0f, 0.0f });
	//bottom face
	verts.push_back({0.0f + posX, 0.0f + posY, 1.0f + posZ,  0.0f, 0.0f });
	verts.push_back({0.0f + posX, 0.0f + posY, 0.0f + posZ,  0.0f, 1.0f });
	verts.push_back({1.0f + posX, 0.0f + posY, 0.0f + posZ,  1.0f, 1.0f });
	verts.push_back({1.0f + posX, 0.0f + posY, 1.0f + posZ,  1.0f, 0.0f });
}
void Renderer::initialize()
{
	this->CompileShaders("VertexShader.vert", "FragmentShader.frag");
	this->ActivateShader();
	//generates the vbo
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * verts.size(), verts.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//generates ebo
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

	//generates the vao
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}
void Renderer::CompileShaders(const char* vertexFile, const char* fragmentFile)
{
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

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
void Renderer::ActivateShader()
{
	glUseProgram(shaderID);
}
void Renderer::DeleteShader()
{
	glDeleteProgram(shaderID);
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
void Renderer::sendPVMUniforms(glm::mat4 proj, glm::mat4 view, glm::mat4 model)
{
	int modelLoc = glGetUniformLocation(shaderID, "m");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	int viewLoc = glGetUniformLocation(shaderID, "v");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	int projLoc = glGetUniformLocation(shaderID, "p");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
}
void Renderer::Draw()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//may only need to call vao bind but unsure
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
Renderer::~Renderer()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
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
	std::cout << "failed that way\n";
	return "not found";
}