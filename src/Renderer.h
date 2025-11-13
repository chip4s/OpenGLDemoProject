#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H
#define GLM_ENABLE_EXPERIMENTAL
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include<iostream>
#include<vector>
#include<cerrno>
#include<string>
#include<fstream>
#include<sstream>

struct Vertex
{
	float posX;
	float posY;
	float posZ;
	float normX;
	float normY;
	float normZ;
};
struct PointLight
{
	glm::vec3 lightPos;
	glm::vec3 lightColor;
	int objIndex;//stores what object its following

	//I do not think I need these yet (to be added)
	//glm::vec3 ambient;
	//glm::vec3 diffuse;
	//glm::vec3 specular;

	//for attenuation
	float constant;
	float linear;
	float quadratic;
};
struct DirectionalLight
{
	glm::vec3 lightDirection;

	//I do not think I need these yet (to be added)
	//glm::vec3 ambient;
	//glm::vec3 diffuse;
	//glm::vec3 specular;
};
struct Object
{
	std::vector<Vertex> verts;
	std::vector<GLuint> indices;
	glm::mat4 model = glm::mat4(1.0f);
	GLuint VBO = 0;
	GLuint VAO = 0;
	GLuint EBO = 0;
};
enum obj : char//determines type of object/light
{
	OBJECT,
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT,
};
std::string get_file_contents(const char* filename);//reads shader text file
class Renderer
{
	public:
		Renderer();
		int AddCube(float pX, float pY, float pZ, obj t);//returns index of where cube is
		void SetCubeModelMat(int index, glm::mat4 mod, obj t);
		void HandlePointLights(glm::vec3 camPos);
		void initialize();
		void CompileShaders();
		void ShaderErrors(unsigned int shader, const  char* type);
		void Draw(glm::mat4 proj, glm::mat4 view);//also sends uniforms of PV while model is stored in object
		~Renderer();
		std::vector<Object> objects;
		std::vector<Object> lightObjects;// to render the actual lights without them being affected by lights

		std::vector<PointLight> pointLights;
		void AddPointLight(int objInd, float lin, float quad);//follows an light object which is by index, 2 vars for attenuation the constant is 1.0f

		std::vector<DirectionalLight> directionalLights;
		void AddDirectionalLight(glm::vec3 dir);

		GLuint objShaderID;
		GLuint lightShaderID;
};
#endif