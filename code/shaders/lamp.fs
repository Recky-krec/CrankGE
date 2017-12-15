#version 330 core

out vec4 fragColor;

in vec2 texCoords;

uniform vec3 glassColor;
uniform sampler2D texture_0;

void main()
{
    vec4 result = vec4(texture(texture_0, texCoords));
    result.xyz *= glassColor;

    fragColor = result;
}