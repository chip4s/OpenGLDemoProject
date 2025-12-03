#version 430 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

const float radians = 3.14157265f / 180;
uniform vec3 viewPos;

const float specularStrength = 0.5f;


//for point lights
struct PointLight
{
    vec3 lightPos;
    vec3 lightColor;
    float AttenLinear;
    float AttenQuad;
    float intensity;
};
uniform PointLight pointLights[24];
uniform int maxPointLights;


//for directional lights
struct DirectionalLight
{
    vec3 direction;
    vec3 lightDirColor;
    float intensity;
};
uniform DirectionalLight directionalLights[24];
uniform int maxDirectionalLights;


//for spot Lights
struct SpotLight
{
	vec3 lightPos;
	vec3 lightColor;
	vec3 lightDirection;
	float cutOff;
    float outerCutOff;
    float intensity;
};
uniform SpotLight spotLights[24];
uniform int maxSpotLights;


vec3 calculatePointLighting(PointLight light)
{
    //Attenuation of light
    float distance = length(light.lightPos - FragPos);

    //formula for attenuation has a const, linear and quadratic affecting intensity
    //later pass these as a uniform for different light intensities
    float attenuation = 1.0f / (1.0f + light.AttenLinear * distance +
                        light.AttenQuad * (distance * distance));


    //calculates diffuse lighting
    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(light.lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * light.lightColor;


    //calculates specular lighting
    float spec = pow(max(dot(norm, halfwayDir), 0.0f), 32);
    vec3 specular = specularStrength * spec * light.lightColor;


    //calculates ambient lighting
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * vec3(1.0f, 1.0f, 1.0f);

    
    //applies attenuation to all types of lighting
    //ambient *= attenuation;// * light.intensity;
    specular *= attenuation * light.intensity;
    diffuse *= attenuation * light.intensity;

    vec3 result = (ambient + diffuse + specular);// * vec3(1.0f, 0.5f, 0.2f);  //object's color for now
    return result;
}


vec3 calculateDirectionalLighting(DirectionalLight light)
{
    vec3 lightDirection = normalize(-light.direction);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDirection + viewDir);


    //calculates diffuse lighting
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDirection), 0.0f);
    vec3 diffuse = diff * light.lightDirColor;



    //calculates specular lighting
    float spec = pow(max(dot(norm, halfwayDir), 0.0f), 32);
    vec3 specular = specularStrength * spec * light.lightDirColor;



    //calculates ambient lighting
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * vec3(1.0f, 1.0f, 1.0f);


    
    //ambient *= light.intensity;
    diffuse *= light.intensity;
    specular *= light.intensity;


    vec3 result = (ambient + diffuse + specular);// * vec3(1.0f, 0.5f, 0.2f);  //object's color for now
    return result;
}


vec3 calculateSpotLighting(SpotLight light)
{
    vec3 lightDir = normalize(light.lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float theta = dot(lightDir, normalize(-light.lightDirection));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);


    vec3 result = vec3(0.0f, 0.0f, 0.0f);

    //calculates diffuse lighting
    vec3 norm = normalize(Normal);

    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * light.lightColor;


    //calculates specular lighting
    float spec = pow(max(dot(norm, halfwayDir), 0.0f), 32);
    vec3 specular = specularStrength * spec * light.lightColor;


    //calculates ambient lighting
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * vec3(1.0f, 1.0f, 1.0f);


    diffuse *= intensity * light.intensity;
    specular *= intensity * light.intensity;
    //ambient *= intensity;// * light.intensity;


    result = (ambient + diffuse + specular);// * vec3(1.0f, 0.5f, 0.2f);  //object's color for now

    return result;

}

const float gamma = 1 / 2.2f;

in vec2 TexCoord;

uniform sampler2D textureSampler;

void main()
{
    vec3 result = vec3(0.0f, 0.0f, 0.0f);


    for(int i = 0; i < maxPointLights; i++)
    {
        result += calculatePointLighting(pointLights[i]);
    }


    for(int j = 0; j < maxDirectionalLights; j++)
    {
        result += calculateDirectionalLighting(directionalLights[j]);
    }


    for(int o = 0; o < maxSpotLights; o++)
    {
        result += calculateSpotLighting(spotLights[o]);
    }

    //result is brightness from light, now apply texture color
    result *= texture(textureSampler, TexCoord).rgb;


    //basic gamma correction
    FragColor = vec4(pow(result, vec3(gamma)), texture(textureSampler, TexCoord).a);
}
