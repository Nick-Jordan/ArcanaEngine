#version 400

layout(location = 0) out vec4 fs_FragColor;

in vec2 fs_TexCoord;

uniform sampler2D u_Texture;

float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898, 78.233))) * 43758.5453);
}

void main(void)
{
    float noiseValue = rand(fs_TexCoord) * 0.1;
    vec3 rgb = texture(u_Texture, fs_TexCoord).rgb;
    vec3 green = vec3(0.0, 1.0, 0.0);
    fs_FragColor = vec4((noiseValue + rgb) * green, 1.0);
}