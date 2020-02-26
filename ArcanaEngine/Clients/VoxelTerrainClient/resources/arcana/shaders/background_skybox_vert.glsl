#version 400

layout (location = 0) in vec3 vs_Position;

out vec3 fs_TexCoords;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;

void main()
{
    fs_TexCoords = vs_Position;
    vec4 position = u_ProjectionMatrix * vec4(mat3(u_ViewMatrix) * vs_Position, 1.0);
	gl_Position = position.xyww;
}