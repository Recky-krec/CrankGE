#version 330 core

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct PointLight
{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float innerCutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;

	int isLanternOn;
};

struct DirLight
{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

uniform vec3 viewPos;
uniform Material material;

// uniform lights
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirLight dirLight;
uniform SpotLight spotLight;

out vec4 fragColor;

// prototypes
vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	// properties
	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(viewPos - fragPos);

	// phase 1: Directional lighting
	vec3 result = calcDirLight(dirLight, norm, viewDir);
	
	// phase 2: Point lights
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
		result += calcPointLight(pointLights[i], norm, fragPos, viewDir);

	// phase 3: Spot light
	if(spotLight.isLanternOn != 0)
		result += calcSpotLight(spotLight, norm, fragPos, viewDir);

	fragColor = vec4(result, 1.0);
}

// calculates the color when using a directional light.
vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// combine results
	vec3 ambient  = light.ambient  *        texture(material.diffuse, texCoords).rgb;
	vec3 diffuse  = light.diffuse  * diff * texture(material.diffuse, texCoords).rgb;
	vec3 specular = light.specular * spec * texture(material.specular, texCoords).rgb;

	return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// combine results
	vec3 ambient  = light.ambient  *        texture(material.diffuse, texCoords).rgb;
	vec3 diffuse  = light.diffuse  * diff * texture(material.diffuse, texCoords).rgb;
	vec3 specular = light.specular * spec * texture(material.specular, texCoords).rgb;

	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	// smooth edges
	float theta = dot(lightDir, normalize(-light.direction));
	float numerator = theta - light.outerCutOff;
	float epsilon = light.innerCutOff - light.outerCutOff;
	float intensity = clamp(numerator / epsilon, 0.0, 1.0);

	// combine results
	vec3 diffuse  = light.diffuse  * diff * texture(material.diffuse, texCoords).rgb;
	vec3 specular = light.specular * spec * texture(material.specular, texCoords).rgb;

	diffuse *= attenuation;
	spec    *= attenuation;
	
	diffuse *= intensity;
	spec    *= intensity;

	return  (diffuse + specular);
}
