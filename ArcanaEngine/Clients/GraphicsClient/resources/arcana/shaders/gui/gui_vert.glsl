#version 400

layout(location = 0) in vec4 vs_Position;
layout(location = 1) in vec4 vs_TexCoords;
layout(location = 2) in vec4 vs_Color;
layout(location = 3) in vec4 vs_ObjectPositionSize;
layout(location = 4) in vec4 vs_StateData;

uniform mat4 u_OrthographicProjection;

out vec4 fs_Color;
out vec4 fs_StateData;
out vec4 fs_ObjectPositionSize;
out vec3 fs_Position;
flat out int fs_Type;

void main()
{
	fs_Color = vs_Color;
	fs_StateData = vs_StateData;
	fs_ObjectPositionSize = vs_ObjectPositionSize;
	fs_Position = vs_Position.xyz;
	fs_Type = int(vs_Position.w);

	gl_Position = u_OrthographicProjection * vec4(fs_Position, 1.0);
}