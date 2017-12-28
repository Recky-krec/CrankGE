#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;


out VS_OUT
{
    vec3 position;
    vec3 normal;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vs_out.normal   = mat3(transpose(inverse(model))) * aNormal;
    vs_out.position = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);

}