#version 430 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 viewPos;

const float specularStrength = 0.5f;

//for point lights
struct PointLight
{
    vec3 lightPosition;
    vec3 lightColor;
    float AttenLinear;
    float AttenQuad;
};
uniform PointLight pointLights[128];
uniform int maxPointLights;


//for directional lights
struct DirectionalLight
{
    vec3 direction;
    vec3 lightDirColor;
};
uniform DirectionalLight directionalLights[128];
uniform int maxDirectionalLights;


vec3 calculatePointLighting(vec3 lightPos, vec3 lightColor, float AttenLinear, float AttenQuad)
{
    //Attenuation of light
    float distance = length(lightPos - FragPos);

    //formula for attenuation has a const, linear and quadratic affecting intensity
    //later pass these as a uniform for different light intensities
    float attenuation = 1.0f / (1.0f + AttenLinear * distance +
                        AttenQuad * (distance * distance));


    //calculates diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;


    //calculates specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = specularStrength * spec * lightColor;


    //calculates ambient lighting
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * vec3(1.0f, 1.0f, 1.0f);

    
    //applies attenuation to all types of lighting
    ambient *= attenuation;
    specular *= attenuation;
    diffuse *= attenuation;

    vec3 result = (ambient + diffuse + specular) * vec3(1.0f, 0.5f, 0.2f);  //object's color for now
    return result;
}
vec3 calculateDirectionalLighting(vec3 direction, vec3 lightDirColor)
{
    vec3 lightDirection = normalize(-direction);


    //calculates diffuse lighting
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDirection), 0.0f);
    vec3 diffuse = diff * lightDirColor;


    //calculates specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDirection, norm);
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = specularStrength * spec * lightDirColor;


    //calculates ambient lighting
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * vec3(1.0f, 1.0f, 1.0f);



    vec3 result = (ambient + diffuse + specular) * vec3(1.0f, 0.5f, 0.2f);  //object's color for now
    return result;
}
void main()
{
    vec3 result = vec3(0.0f, 0.0f, 0.0f);


    for(int i = 0; i < maxPointLights; i++)
    {
        result += calculatePointLighting(pointLights[i].lightPosition, pointLights[i].lightColor, pointLights[i].AttenLinear, pointLights[i].AttenQuad);
    }


    for(int j = 0; j < maxDirectionalLights; j++)
    {
        result += calculateDirectionalLighting(directionalLights[j].direction, directionalLights[j].lightDirColor);
    }

    FragColor = vec4(result, 1.0f);
}
