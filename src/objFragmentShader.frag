#version 430 core

in vec3 afragColor;
out vec4 FragColor;

void main()
{
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * vec3(1.0f, 1.0f, 1.0f);
    
    vec3 result = ambient * afragColor;
    FragColor = vec4(result, 1.0f);
}
