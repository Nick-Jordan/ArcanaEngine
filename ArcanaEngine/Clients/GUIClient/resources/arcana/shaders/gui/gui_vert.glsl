#version 400

layout(location = 0) in vec2 vs_VertexPosition;
layout(location = 1) in vec4 vs_ObjectPositionSize;
layout(location = 2) in vec4 vs_TexCoords;
layout(location = 3) in vec4 vs_Color;
layout(location = 4) in vec4 vs_SecondaryColor;
layout(location = 5) in vec3 vs_Attributes;

uniform mat4 u_OrthographicProjection;

out vec4 fs_Color;
out vec4 fs_SecondaryColor;
out vec4 fs_ObjectPositionSize;
out vec2 fs_TexCoords;
out vec3 fs_Position;
out float fs_Radius;
flat out int fs_Gradient;

void main()
{
	fs_ObjectPositionSize = vs_ObjectPositionSize;
	fs_Color = vs_Color;
	fs_SecondaryColor = vs_SecondaryColor;
	fs_Radius = vs_Attributes.x;
	fs_TexCoords = vs_VertexPosition * vs_TexCoords.zw + vs_TexCoords.xy;
	fs_Position = vec3(vs_VertexPosition * vs_ObjectPositionSize.zw, 0.0) + vec3(vs_ObjectPositionSize.xy, vs_Attributes.y);
	fs_Gradient = int(vs_Attributes.z);

	gl_Position = u_OrthographicProjection * vec4(fs_Position, 1.0);
}