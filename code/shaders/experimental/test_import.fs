#version 450 core

struct Material
{
    vec3 Color;
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_specular1;
    float Shininess;
};

struct PointLight
{
    vec3 Position;

    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;

    float Constant;
    float Linear;
    float Quadratic;
};

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

out vec4 fragColor;
uniform vec3 cameraPos;

uniform Material material;
uniform PointLight pointLight;

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir);

void main()
{
    vec3 norm = normalize(normal);
    vec3 viewDirection = normalize(cameraPos - fragPos);


    vec3 result = vec3(0);
    // step: point lights
    result += calcPointLight(pointLight, norm, viewDirection);


    fragColor = vec4(result, 1.0);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
    // diffuse shading
    vec3 lightDirection = normalize(light.Position - fragPos);
    float diff = max(dot(lightDirection, normal), 0.0);

    // specular shading
    vec3 lightDirReflection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, lightDirection), 0), material.Shininess);

   	// attenuation
   	float distance = length(light.Position - fragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));

    vec3 ambient  = light.Ambient * vec3(texture(material.texture_diffuse1, texCoords));
    vec3 diffuse  = light.Diffuse * diff * vec3(texture(material.texture_diffuse1, texCoords));
    vec3 specular = light.Specular * spec * vec3(texture(material.texture_diffuse1, texCoords));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}
