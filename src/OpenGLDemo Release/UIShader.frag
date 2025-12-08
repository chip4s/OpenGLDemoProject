#version 430 core

//UI texture unit
layout(binding = 8) uniform sampler2D UITextureSampler;

in vec2 texCoord;

out vec4 FragColor;

void main()
{
	FragColor = texture(UITextureSampler, texCoord);
}