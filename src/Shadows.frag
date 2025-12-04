#version 430 core

in vec3 FragPos;

uniform vec3 lightPos;


const float farPlane = 25.0f;

void main()
{
	float lightDistance = length(FragPos - lightPos);

	lightDistance = lightDistance / farPlane;

	gl_FragDepth = lightDistance;
}
