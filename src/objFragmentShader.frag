#version 430 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 lightPositions[];
uniform vec3 viewPos;
uniform vec3 lightColors[];

const float specularStrength = 0.5f;

uniform float AttenLinear[];
uniform float AttenQuad[];

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

    vec3 result = (ambient + diffuse + specular) * vec3(1.0f, 0.5f, 0.2f);//object's color which later will be texture
    return result;
}
void main()
{
    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    result += calculatePointLighting(lightPositions[0], lightColors[0], AttenLinear[0], AttenQuad[0]);
    result += calculatePointLighting(lightPositions[1], lightColors[1], AttenLinear[1], AttenQuad[1]);

    FragColor = vec4(result, 1.0f);
}
