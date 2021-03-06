#version 400

layout(location = 0) in vec2 vs_Position;
layout(location = 1) in vec2 vs_TexCoord;

out vec2 fs_TexCoord;

uniform vec4 u_Area;
uniform float u_ZPosition;
uniform mat4 u_OrthographicProjection;

void main()
{
	vec2 position = (vs_Position * 0.5 + 0.5) * u_Area.zw + u_Area.xy;

	fs_TexCoord = vs_TexCoord;
	gl_Position = u_OrthographicProjection * vec4(position, u_ZPosition, 1.0);
}