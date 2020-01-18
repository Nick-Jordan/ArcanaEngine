#version 400

layout(location = 0) out vec4 fs_FragColor;

in vec2 fs_TexCoord;

uniform sampler2D u_Texture;

void main()
{	
	vec4 color = texture(u_Texture, fs_TexCoord);
	fs_FragColor = vec4(vec3(1.0) - color.rgb, 1.0);
}