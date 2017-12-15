#version 450 core

struct Material
{
	float shininess;
	sampler2D texture_diffuse1; // diffuse map
	sampler2D texture_diffuse2; // diffuse map
	sampler2D texture_specular1;// specular map
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


float near = 0.1;
float far  = 100.0;

uniform vec3 viewPos;
uniform Material material;

#define NR_POINT_LIGHTS 2
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform DirLight dirLight;

out vec4 fragColor;

// prototypes
vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir);
vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
float LinearizeDepth(float depth);

void main()
{
    // discards the fragment if alpha value of texel is under 0.1
    float alpha = texture(material.texture_diffuse1, fs_in.texCoords).a;
    if(alpha < 0.1)
        discard;

	vec3 norm = normalize(fs_in.normal);
	vec3 viewDir = normalize(viewPos - fs_in.fragPos);
	vec3 result = vec3(0.0);
	
	// step 1: calculate point lights
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
		result += calcPointLight(pointLights[i], norm, viewDir);
	
	// step 2: calculate spot light
	if(spotLight.isLanternOn != 0)
		result += calcSpotLight(spotLight, norm, viewDir);

	// step 3: calculate dir light
	result += calcDirLight(dirLight, norm, viewDir);

	fragColor = vec4(result, alpha);

	//fragColor = vec4(vec3(LinearizeDepth(gl_FragCoord.z) / 50), 1.0);
}

float LinearizeDepth(float depth)
{
        float z = depth * 2.0 - 1.0; // back to NDC
        return (2.0 * near * far) / (far + near - z * (far - near));
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

    vec3 ambient  = light.ambient         * (texture(material.texture_diffuse1, fs_in.texCoords).rgb + texture(material.texture_diffuse2, fs_in.texCoords).rgb);
    vec3 diffuse  = light.diffuse  * diff * (texture(material.texture_diffuse1, fs_in.texCoords).rgb + texture(material.texture_diffuse2, fs_in.texCoords).rgb);
    vec3 specular = light.specular * spec *  texture(material.texture_specular1, fs_in.texCoords).r; // might have to change to .rgb?

    return (ambient + diffuse + specular);
}
