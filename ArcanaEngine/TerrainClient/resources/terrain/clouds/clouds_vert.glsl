#version 400

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform vec3 u_CameraPosition;

layout(location = 0) in vec3 vs_Position;

out vec3 fs_Position;
out vec3 fs_Normal;

out float fs_HALF_FCOEF;
out float fs_LogZ;

uniform struct {
	vec4 offset;
	vec4 camera;
	vec2 blending;
	float radius;
	mat4 screenQuadCorners;
	mat4 screenQuadVerticals;
	vec4 screenQuadCornerNorms;
	mat3 tangentFrameToWorld;
} deformation;

void main(void)
{		
	vec2 v = abs(deformation.camera.xy - vs_Position.xy);
	float d = max(max(v.x, v.y), deformation.camera.z);
	float blend = clamp((d - deformation.blending.x) / deformation.blending.y, 0.0, 1.0);

	float R = deformation.radius;
	mat4 C = deformation.screenQuadCorners;
	mat4 N = deformation.screenQuadVerticals;
	vec4 L = deformation.screenQuadCornerNorms;
	vec3 P = vec3(vs_Position.xy * deformation.offset.z + deformation.offset.xy, R);

	vec4 uvUV = vec4(vs_Position.xy, vec2(1.0) - vs_Position.xy);
	vec4 alpha = uvUV.zxzx * uvUV.wwyy;
	vec4 alphaPrime = alpha * L / dot(alpha, L);

	float k = min(length(P) / dot(alpha, L) * 1.0000003, 1.0);
	float hPrime = (R * (1.0 - k)) / k;

#ifdef CUBE_PROJECTION
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(P, 1.0);
#else
	gl_Position = (C + hPrime * N) * alphaPrime;
#endif

	fs_Position = (deformation.radius) * normalize(mat3(u_ModelMatrix) * P);
	fs_Normal = normalize(fs_Position);

    float u_ZFar = pow(10.0, 10.0);

	float FCOEF = 2.0 / log2(u_ZFar + 1.0);
	fs_HALF_FCOEF = 0.5 * FCOEF;
	gl_Position.z = log2(max(0.000001, 1.0 + gl_Position.w)) * FCOEF - 1.0;
	fs_LogZ = 1.0 + gl_Position.w;
}
  