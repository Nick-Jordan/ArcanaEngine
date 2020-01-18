#version 400

out vec4 fs_FragColor;

in vec2 fs_TexCoord;

uniform sampler2D u_Texture;

uniform vec2 u_ScreenTime;

void main()
{
	if(u_ScreenTime.x < u_ScreenTime.y * 0.2)
		discard;

	float dist1 = u_ScreenTime.y * 0.4 - u_ScreenTime.x;
	float dist2 = u_ScreenTime.x - u_ScreenTime.y * 0.8;

	vec4 color = vec4(0.0);
	vec4 textureColor = texture(u_Texture, fs_TexCoord);

	if(dist1 >= 0.0)
	{
		color = mix(color, textureColor, 1.0 - dist1 / (u_ScreenTime.y * 0.2));
	}
	else if(dist2 >= 0.0)
	{
		color = mix(color, textureColor, 1.0 - dist2 / (u_ScreenTime.y * 0.2));
	}
	else
	{
		color = textureColor;
	}

	fs_FragColor = color;
}