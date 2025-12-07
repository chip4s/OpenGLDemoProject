#ifndef COMPONENTS_H
#define COMPONENTS_H
#define GLM_ENABLE_EXPERIMENTAL
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/string_cast.hpp>

#include <stb/stb_image.h>

#include<iostream>
#include<vector>
#include<string>
#include<utility>
#include<map>



class BaseComponent
{
public:
	bool exists = false;
};
class CTransform : public BaseComponent
{
public:
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);//degrees on axis euler
	glm::vec3 scale = glm::vec3(0.0f);
	CTransform() {}
	CTransform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& size)
		: position(pos), rotation(rot), scale(size) {}
};
enum Shapes : char
{
	CUBE,
};
class CMesh : public BaseComponent
{
private:
	struct Vertex
	{
		float posX;
		float posY;
		float posZ;

		float normX;
		float normY;
		float normZ;

		float texCoordX;
		float texCoordY;
	};
public:
	std::vector<Vertex> verts;
	std::vector<GLuint> indices;

	GLuint VBO = 0;
	GLuint VAO = 0;
	GLuint EBO = 0;

	bool isInitialized = false; //generated buffers?

	//later constructor can take file path to load with assimp for now enum with shapes
	
	Shapes meshShape = CUBE;//stores what kind of shape it is. Later might make shape its own component (physics?)

	//constructor right now just makes a cube
	CMesh(Shapes shape)
	{
		//first set up indices and vertices
		indices.reserve(36);
		for (int f = 0; f < 24; f += 4)
		{
			indices.push_back(0 + f);
			indices.push_back(1 + f);
			indices.push_back(2 + f);
			indices.push_back(2 + f);
			indices.push_back(3 + f);
			indices.push_back(0 + f);
		}
		verts.reserve(24);
		//                       pos               normal            texCoords
		//front face
		verts.push_back({ -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f,    0.0f, 0.0f});
		verts.push_back({ -0.5f, 0.5f, 0.5f,  0.0f, 0.0f, 1.0f,    0.0f, 1.0f});
		verts.push_back({ 0.5f, 0.5f, 0.5f,  0.0f, 0.0f, 1.0f,    1.0f, 1.0f });
		verts.push_back({ 0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f,    1.0f, 0.0f });
		//right face
		verts.push_back({ 0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 0.0f,    0.0f, 0.0f});
		verts.push_back({ 0.5f, 0.5f, 0.5f,  1.0f, 0.0f, 0.0f,    0.0f, 1.0f});
		verts.push_back({ 0.5f, 0.5f, -0.5f,  1.0f, 0.0f, 0.0f,    1.0f, 1.0f});
		verts.push_back({ 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,    1.0f, 0.0f});
		//left face
		verts.push_back({ -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,    0.0f, 0.0f});
		verts.push_back({ -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,    0.0f, 1.0f});
		verts.push_back({ -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,    1.0f, 1.0f});
		verts.push_back({ -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,    1.0f, 0.0f});
		//top face
		verts.push_back({ -0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,    0.0f, 0.0f});
		verts.push_back({ -0.5f, 0.5f, -0.5f,  0.0f, 1.0f, 0.0f,    0.0f, 1.0f});
		verts.push_back({ 0.5f, 0.5f, -0.5f,  0.0f, 1.0f, 0.0f,    1.0f, 1.0f});
		verts.push_back({ 0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,    1.0f, 0.0f});
		//back face
		verts.push_back({ 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,    0.0f, 0.0f});
		verts.push_back({ 0.5f, 0.5f, -0.5f,  0.0f, 0.0f, -1.0f,    0.0f, 1.0f});
		verts.push_back({ -0.5f, 0.5f, -0.5f,  0.0f, 0.0f, -1.0f,    1.0f, 1.0f});
		verts.push_back({ -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,    1.0f, 0.0f});
		//bottom face
		verts.push_back({ -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,    0.0f, 0.0f});
		verts.push_back({ -0.5f, -0.5f, 0.5f,  0.0f, -1.0f, 0.0f,    0.0f, 1.0f});
		verts.push_back({ 0.5f, -0.5f, 0.5f,  0.0f, -1.0f, 0.0f,    1.0f, 1.0f});
		verts.push_back({ 0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,    1.0f, 0.0f});
	}
	CMesh() {
		exists = false; 
	};
	void initializeMesh()
	{
		//generate vaos and buffers
		
		//generates the vao
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		//generates the vbo
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float) * verts.size(), verts.data(), GL_STATIC_DRAW);

		//sets attribute pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		//generates ebo
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
		
		isInitialized = true;
	}
	~CMesh()
	{
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteVertexArrays(1, &VAO);
	}
};
class CPointLight : public BaseComponent
{
public:
	//glm::vec3 lightPos;	get it from transform
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);//default color white

	//I do not think I need these yet (to be added)
	//glm::vec3 ambient;
	//glm::vec3 diffuse;
	//glm::vec3 specular;

	//for attenuation defaults are the lowest brightness. constant is always 1
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;

	float intensity = 0.0f;

	CPointLight() {}

	CPointLight(glm::vec3 color, float brightness, float fallOffLin, float fallOffQuad) 
		: lightColor(color), linear(fallOffLin), quadratic(fallOffQuad), intensity(brightness) {}

	CPointLight(glm::vec3 color, float brightness)//default attenuation
		: lightColor(color), linear(0.09f), quadratic(0.032f), intensity(brightness) {}

	//Shadow mapping things
	glm::mat4 shadowPerspective = glm::mat4(1.0f);

	GLuint shadowMapResolution = 0;
	GLuint shadowCubeMap = 0;
	GLuint shadowFBO = 0;
	bool shadowMapIsInitialized = false;
	float nearPlane = 0.0f;
	float farPlane = 0.0f;


	struct CubeDirection
	{
		GLenum cubeFace;
		glm::vec3 target;
		glm::vec3 up;
	};
	CubeDirection cameraDirections[6] =
	{
		{GL_TEXTURE_CUBE_MAP_POSITIVE_X, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)},
		{GL_TEXTURE_CUBE_MAP_NEGATIVE_X, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)},
		{GL_TEXTURE_CUBE_MAP_POSITIVE_Y, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)},
		{GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f,-1.0f)},
		{GL_TEXTURE_CUBE_MAP_POSITIVE_Z, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)},
		{GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)},
	};
	
	void ShadowBufferInitialize(GLuint shadowResolution, float near, float far)
	{
		//for now hard coded
		nearPlane = 0.51f;
		farPlane = 40.0f;
		shadowMapResolution = 2048;
		shadowPerspective = glm::perspective(glm::radians(90.0f), 1.0f, nearPlane, farPlane);


		//create cube map to store depth textures in all directions
		glGenTextures(1, &shadowCubeMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubeMap);

		for (int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT32F, shadowMapResolution, shadowMapResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}

		//Set texture settings for the cube map
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


		//create frame buffer for cube map
		glGenFramebuffers(1, &shadowFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

		//dont write/read into color buffer since shadows only need depth
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		shadowMapIsInitialized = true;
	}
	void ShadowBufferWriteBind(GLenum cubeFace)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadowFBO);
		glViewport(0, 0, shadowMapResolution, shadowMapResolution);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, cubeFace, shadowCubeMap, 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
	}
	void ShadowBufferReadBind(GLenum TextureUnit)
	{
		glActiveTexture(TextureUnit);
		glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubeMap);
	}
};
class CDirectionalLight : public BaseComponent
{
public:
	//glm::vec3 lightDirection;    get it from transform
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	float intensity = 0.0f;

	//I do not think I need these yet (to be added)
	//glm::vec3 ambient;
	//glm::vec3 diffuse;
	//glm::vec3 specular;
	CDirectionalLight() {}
	CDirectionalLight(glm::vec3 color, float brightness)
		: lightColor(color), intensity(brightness) {}
};
class CSpotLight : public BaseComponent
{
public:
	//glm::vec3 lightPos;	get it from transform
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	//glm::vec3 lightDirection;	 get it from transform.rotation
	float cutOff = 0.0f;
	float outerCutOff = 0.0f;
	
	float intensity = 0.0f;


	CSpotLight() {}
	CSpotLight(glm::vec3 color, float cut, float outCut, float brightness)
		: cutOff(glm::cos(glm::radians(cut))), outerCutOff(glm::cos(glm::radians(outCut))), intensity(brightness) {}
};
class CTexture : public BaseComponent
{
private:
	std::string m_FilePath;
	unsigned int m_TextureID = 0;
	int m_Width = 0;
	int m_Height = 0;
	int m_nrChannels = 0;
public:
	bool isLoaded = false;

	CTexture()
		: m_FilePath("Textures/default.png"){};
	CTexture(const std::string& filePath)
		: m_FilePath(filePath) {};

	//initializes Texture
	void LoadTexture()
	{
		//generate texture
		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);


		//texture wrapping options (could later be changed by parameters)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		//load texture from file
		unsigned char* data = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_nrChannels, 0);
		

		//generate texture with png
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
		isLoaded = true;
	}

	//bind Texture
	void BindTexture()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}
};
class CBoxCollider : public BaseComponent
{
public:
	float width = 0.0f;//x extents
	float height = 0.0f;//y extents
	float length = 0.0f;//z extents
	CBoxCollider() {};
	
	CBoxCollider(float w, float h, float l)
	{
		width = w;
		height = h;
		length = l;
	}
};

class CRigidBody : public BaseComponent
{
public:
	//statics push other statics and dynamics
	//dynamics push other dynamics
	bool dynamic = false;
	CRigidBody() {};
	CRigidBody(bool moves, float m) 
		: dynamic(moves), mass(m) {
	
	};
	
	//Rigid body comp.s (Add when I need more)
	//transform has position
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 netForce = glm::vec3(0.0f, 0.0f, 0.0f);
	float mass = 0.0f;
};
#endif