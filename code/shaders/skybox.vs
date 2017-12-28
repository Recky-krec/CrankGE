#version 450 core

layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;

out vec3 texCoords;

void main()
{
    texCoords = aPos; // because the cube is center is (0,0,0) position vector is also direction vector
    vec4 position = projection * view * vec4(aPos, 1.0);
    gl_Position = position.xyww;
}