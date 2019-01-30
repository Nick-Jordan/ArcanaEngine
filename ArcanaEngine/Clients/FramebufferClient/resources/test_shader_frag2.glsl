#version 400

layout(location = 0) out vec4 fs_FragColor;

void main()
{
	if(gl_FragCoord.x > 128)
	{
		fs_FragColor = vec4(0.5, 0.5, 1.0, 1.0);
	}
	else
	{
		fs_FragColor = vec4(0.0, 1.0, 1.0, 1.0);
	}
}