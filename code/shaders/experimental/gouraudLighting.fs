#version 330 core

in vec3 resultColor;

out vec4 fragColor;

void main()
{
	fragColor = vec4(resultColor, 1.0);
}