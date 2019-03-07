#version 400

layout(location = 0) in vec3 vs_Position;
layout(location = 1) in vec3 vs_Offset;
layout(location = 2) in vec3 vs_Size;
layout(location = 3) in vec4 vs_Color;

out vec4 fs_Color;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;

uniform vec3 u_ViewRight;
uniform vec3 u_ViewUp;

void main()
{
	vec3 quadVertices = (vs_Position * 2.0 - 1.0);

	vec3 position = vs_Offset + (u_ViewRight * quadVertices.x * vs_Size.x) + (u_ViewUp * quadVertices.y * vs_Size.y);

	gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(position, 1.0);

	fs_Color = vs_Color;
}