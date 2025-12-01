#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;
void main()
{
    gl_Position = p * v * m * vec4(aPos, 1.0f);


    //for lighting
    FragPos = vec3(m * vec4(aPos, 1.0f));//pos in world space


    Normal = mat3(transpose(inverse(m))) * aNormal;
    //hint for later *calc normal on cpu and send a uniform*
}
