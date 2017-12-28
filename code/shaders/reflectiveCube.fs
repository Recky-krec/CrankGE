#version 450 core

in VS_OUT
{
    vec3 position;
    vec3 normal;
} fs_in;

uniform vec3 u_cameraPos;
uniform samplerCube u_skybox;


out vec4 fragColor;
void main()
{
    vec3 viewDirection = normalize(fs_in.position - u_cameraPos);
    vec3 vdReflection  = reflect(viewDirection, normalize(fs_in.normal));
    fragColor = vec4(texture(u_skybox, vdReflection).rgb, 1.0);
}