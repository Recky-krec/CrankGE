#version 450 core



struct Material
{
	float shininess;
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_specular1;
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
	float constant;
	float linear;
	float quadratic;
	float innerCutOff;
	float outerCutOff;
	int isLanternOn;
};

struct DirLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in VS_OUT
{
    vec2 texCoords;
    vec3 fragPos;
    vec3 normal;
} fs_in;

// prototypes
vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir);
vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
float LinearizeDepth(float depth);

const float near = 0.1;
const float far  = 100.0;

#define NR_POINT_LIGHTS 2
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform DirLight dirLight;
uniform Material material;

uniform samplerCube skybox;
uniform vec3 viewPos;

out vec4 fragColor;
void main()
{
	vec3 norm = normalize(fs_in.normal);
	vec3 viewDir = normalize(fs_in.fragPos - viewPos);
	vec3 result = vec3(0.0);

	// Step 1: calculate point lights
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
		result += calcPointLight(pointLights[i], norm, viewDir);

	// Step 2: calculate spot light
	if(spotLight.isLanternOn != 0)
		result += calcSpotLight(spotLight, norm, viewDir);

	// Step 3: calculate dir light
	result += calcDirLight(dirLight, norm, viewDir);

    vec3 viewDirReflection = reflect(viewDir, norm);
    result += 0.5 * texture(skybox, viewDirReflection).rgb;

    //float alpha = texture(material.texture_diffuse1, fs_in.texCoords).a;
	fragColor = vec4(result, 1.0);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fs_in.fragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectedDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectedDir), 0.0), material.shininess);
	// attenuation
	float distance = length(light.position - fs_in.fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// combine results
	vec3 ambient  = light.ambient         * (texture(material.texture_diffuse1, fs_in.texCoords).rgb + texture(material.texture_diffuse2, fs_in.texCoords).rgb);
    vec3 diffuse  = light.diffuse  * diff * (texture(material.texture_diffuse1, fs_in.texCoords).rgb + texture(material.texture_diffuse2, fs_in.texCoords).rgb);
    vec3 specular = light.specular * spec *  texture(material.texture_specular1, fs_in.texCoords).r; // might have to change to .rgb?

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fs_in.fragPos);

	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// attenuation
	float distance = length(light.position - fs_in.fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// smooth edges
	float theta = dot(lightDir, normalize(-light.direction));
	float numerator = theta - light.outerCutOff;
	float epsilon = light.innerCutOff - light.outerCutOff;
	float intensity = clamp(numerator / epsilon, 0.0, 1.0);

	// combine results
	vec3 diffuse  = light.diffuse  * diff * (texture(material.texture_diffuse1, fs_in.texCoords).rgb + texture(material.texture_diffuse2, fs_in.texCoords).rgb);
	vec3 specular = light.specular * spec * texture(material.texture_specular1, fs_in.texCoords).r;

	diffuse *= attenuation;
	spec    *= attenuation;

	diffuse *= intensity;
	spec    *= intensity;

	return  (diffuse + specular);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir) , 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0) , material.shininess);

    vec3 ambient  = light.ambient         *  texture(material.texture_diffuse1, fs_in.texCoords).rgb;
    vec3 diffuse  = light.diffuse  * diff *  texture(material.texture_diffuse1, fs_in.texCoords).rgb;
    vec3 specular = light.specular * spec *  texture(material.texture_specular1, fs_in.texCoords).r; // might have to change to .rgb?

    return (ambient + diffuse + specular);
}
