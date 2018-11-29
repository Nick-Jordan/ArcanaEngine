#version 400

layout(location = 0) in vec3 vs_Position;

out vec4 fs_Position;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
//uniform mat4 u_ModelMatrix;

uniform struct {
    vec4 offset;
    vec4 camera;
    vec2 blending;
} deformation;

void main()
{
	fs_Position = vec4(vs_Position.xy * deformation.offset.z + deformation.offset.xy, 0.0, 1.0);

	gl_Position = u_ProjectionMatrix * u_ViewMatrix * fs_Position;
}
