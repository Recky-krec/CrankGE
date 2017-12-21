#version 450 core

out vec4 fragColor;

in VS_OUT
{
    vec2 texCoords;
} fs_in;

uniform sampler2D texture1;


void main()
{
    // post processing effects
    //normal
    fragColor = texture(texture1, fs_in.texCoords);
    //inversion
    //fragColor = vec4(1.0 - vec3(texture(texture1, fs_in.texCoords)), 1.0);
    //grayscale

}