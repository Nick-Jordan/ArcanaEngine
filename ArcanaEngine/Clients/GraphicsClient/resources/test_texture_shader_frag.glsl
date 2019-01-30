#version 400

layout(location = 0) out vec4 fs_FragColor;

in vec4 fs_Color;
in vec3 fs_Position;

uniform sampler2D test_texture;

void main()
{
	fs_FragColor = texture(test_texture, fs_Position.xy * vec2(0.5) + vec2(0.5))+ vec4(0.1);
}