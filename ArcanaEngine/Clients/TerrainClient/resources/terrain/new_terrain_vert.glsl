#version 400

layout(location = 0) in vec3 vs_Position;

out vec3 fs_Position;
out vec2 fs_TexCoord;

out float h;

out float fs_HALF_FCOEF;
out float fs_LogZ;

uniform mat4 u_ModelMatrix;

uniform mat4 gridMatrix;
uniform mat4 quadMVP;

uniform struct {
	vec4 offset;
	vec4 camera;
	vec2 blending;
	float radius;
} deformation;

void main()
{
	fs_TexCoord = vs_Position.xy;

	fs_Position = (gridMatrix * vec4(vs_Position, 1.0)).xyz;

	gl_Position = quadMVP * vec4(vs_Position, 1.0);

	//logarithmic depth buffer
	//make ZFar a uniform
	float u_ZFar = pow(10.0, 10.0);

	float FCOEF = 2.0 / log2(u_ZFar + 1.0);
	fs_HALF_FCOEF = 0.5 * FCOEF;
	gl_Position.z = (log2(max(0.000001, 1.0 + gl_Position.w)) * FCOEF - 1.0) * gl_Position.w;
	fs_LogZ = 1.0 + gl_Position.w;
}
