#version 430 core

in vec3 FragPos;

uniform vec3 lightPos;


uniform float farplane;

void main()
{
	float lightDistance = length(FragPos - lightPos);

	lightDistance = lightDistance / farplane;

	gl_FragDepth = lightDistance;
}
