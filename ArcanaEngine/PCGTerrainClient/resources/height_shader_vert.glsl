#version 400

layout(location = 0) in vec2 vs_Position;
layout(location = 1) in vec2 vs_TexCoord;

out vec3 fs_Position;

uniform float u_Radius;
uniform vec4 u_QuadOffset;
uniform mat4 u_Transformation;

void main()
{
	vec3 P = vec3(vs_TexCoord.xy * u_QuadOffset.z + u_QuadOffset.xy, u_Radius);

	fs_Position = vec3(u_Radius * normalize(mat3(u_Transformation) * P));

	gl_Position = vec4(vec3(vs_Position.xy, 0.0), 1.0);
}