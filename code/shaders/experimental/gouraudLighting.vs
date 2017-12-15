#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

out vec3 resultColor;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);

	// gouraud shading
    // ------------------------
	vec3 position = vec3(model * vec4(aPos, 1.0));
	vec3 normal = mat3(transpose(inverse(model))) * aNorm;

	// ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;
	
	// diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - position);
	float diff = max(dot(norm, lightDir), 0);
	vec3 diffuse = diff * lightColor;

	// specular
	float specularStrength = 1.0;
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 viewDir = normalize(viewPos - position);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	resultColor = (ambient + diffuse + specular) * objectColor;
}