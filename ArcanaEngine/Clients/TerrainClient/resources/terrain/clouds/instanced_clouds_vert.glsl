#version 400

layout(location = 0) in vec3 vs_Position;

//deformation
layout(location = 1) in vec4 deformation_Offset;
layout(location = 2) in vec4 deformation_Camera;
layout(location = 3) in mat4 deformation_ScreenQuadCorners;
layout(location = 7) in mat4 deformation_ScreenQuadVerticals;
layout(location = 11) in vec4 deformation_ScreenQuadCornerNorms;
layout(location = 12) in mat4 deformation_TangentFrameToWorld;

out vec3 fs_Position;
out vec3 fs_Normal;

uniform struct {
	vec2 blending;
	float radius;
} deformation;

out float fs_HALF_FCOEF;
out float fs_LogZ;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;

void main()
{
	vec2 v = abs(deformation_Camera.xy - vs_Position.xy);
	float d = max(max(v.x, v.y), deformation_Camera.z);
	float blend = clamp((d - deformation.blending.x) / deformation.blending.y, 0.0, 1.0);

	float R = deformation.radius;
	mat4 C = deformation_ScreenQuadCorners;
	mat4 N = deformation_ScreenQuadVerticals;
	vec4 L = deformation_ScreenQuadCornerNorms;
	vec3 P = vec3(vs_Position.xy * deformation_Offset.z + deformation_Offset.xy, R);

	vec4 uvUV = vec4(vs_Position.xy, vec2(1.0) - vs_Position.xy);
	vec4 alpha = uvUV.zxzx * uvUV.wwyy;
	vec4 alphaPrime = alpha * L / dot(alpha, L);

	float k = min(length(P) / dot(alpha, L) * 1.0000003, 1.0);
	float hPrime = (R * (1.0 - k)) / k;

#ifdef CUBE_PROJECTION
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(P + vec3(0.0, 0.0, 0.0), 1.0);
#else
	gl_Position = (C + hPrime * N) * alphaPrime;
#endif

	fs_Position = (deformation.radius) * normalize(mat3(u_ModelMatrix) * P);
	fs_Normal = normalize(fs_Position);

	float u_ZFar = pow(10.0, 10.0);

	float FCOEF = 2.0 / log2(u_ZFar + 1.0);
	fs_HALF_FCOEF = 0.5 * FCOEF;
	gl_Position.z = (log2(max(0.000001, 1.0 + gl_Position.w)) * FCOEF - 1.0) * gl_Position.w;
	fs_LogZ = 1.0 + gl_Position.w;
}
