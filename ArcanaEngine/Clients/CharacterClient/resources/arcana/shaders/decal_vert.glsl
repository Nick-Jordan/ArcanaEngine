#version 400

layout(location = 0) in vec3 vs_Position;

out vec3 fs_Position;
out vec3 fs_Normal;
out vec2 fs_TexCoord;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;

uniform vec3 u_CameraPosition;


void main()
{
	fs_Position = (u_ModelMatrix * vec4(vs_Position, 1.0)).xyz;
	fs_Normal = vec3(-1, 0, 0);//normalize(u_NormalMatrix * vs_Normal);
	fs_TexCoord = vec2(0.0);

	gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(fs_Position, 1.0);
}