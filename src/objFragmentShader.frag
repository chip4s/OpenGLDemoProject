#version 430 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

const float specularStrength = 0.5f;
void main()
{
    //calculates diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * vec3(1.0f,1.0f,1.0f); //light color


    //calculates specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = specularStrength * spec * vec3(1.0f,1.0f,1.0f);//light color


    //calculates ambient lighting
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * vec3(1.0f, 1.0f, 1.0f);
    
    //adds lighting together
    vec3 result = (ambient + diffuse + specular) * vec3(1.0f, 0.5f, 0.2f);
    FragColor = vec4(result, 1.0f);
}
