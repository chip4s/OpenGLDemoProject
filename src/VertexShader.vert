#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aColor;

out vec4 afragColor;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;
void main()
{
    gl_Position = p * v * m * vec4(aPos, 1.0f);
    afragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}
