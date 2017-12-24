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
    //fragColor = texture(texture1, fs_in.texCoords);

    //inversion
    //fragColor = vec4(1.0 - vec3(texture(texture1, fs_in.texCoords)), 1.0);

    //grayscale
    //vec4 color = texture(texture1, fs_in.texCoords);
    //fragColor = vec4(vec3( (color.r + color.g + color.b) / 3.0 ), color.a);
    //fragColor = vec4(vec3( ( 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b) / 3.0 ), color.a);

    //kernels
    const float offset = 1.0 / 300.0;
    vec2 offsets[9] = vec2[]
    (
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right
    );

    float sharpenKernel[9] = float[]
    (
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );

    float blurKernel[9] = float[]
    (
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16
    );

    float edgeDetectionKernel[9] = float[]
    (
         1,  1,  1,
         1, -8,  1,
         1,  1,  1
    );

    vec3 sampleTex[9];
    for (int i = 0; i<9; i++)
    {
        sampleTex[i] = vec3(texture(texture1, fs_in.texCoords.st + offsets[i]));
    }

    vec3 color = vec3(0.0);
    for(int i = 0; i<9; i++)
    {
        //color += sampleTex[i] * sharpenKernel[i];
        //color += sampleTex[i] * blurKernel[i];
        color += sampleTex[i] * edgeDetectionKernel[i];

    }

    fragColor = vec4(color, 1.0);

}