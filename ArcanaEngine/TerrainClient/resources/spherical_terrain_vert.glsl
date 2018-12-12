#version 400

layout(location = 0) in vec3 vs_Position;

out vec4 fs_Position;
out vec2 fs_TexCoord;
out float fs_Height;

out float fs_HALF_FCOEF;
out float fs_LogZ;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
//uniform mat4 u_ModelMatrix;

//uniform float u_ZFar;

uniform struct {
	vec4 offset;
	vec4 camera;
	vec2 blending;
	float radius;
	mat3 localToWorld;
	mat4 screenQuadCorners;
	mat4 screenQuadVerticals;
	vec4 screenQuadCornerNorms;
	mat3 tangentFrameToWorld;
} deformation;

struct samplerTile 
{
	sampler2DArray tilePool;
	float tileLayer;
	vec3 tileCoords;
};

vec4 textureTile(samplerTile tex, vec2 uv) 
{
	return texture(tex.tilePool, vec3(uv, tex.tileLayer));
}

uniform samplerTile elevationSampler;
uniform samplerTile parent_elevationSampler;


void main()
{
	fs_TexCoord = vs_Position.xy;

	vec4 zfc = vec4(0.0);//textureTile(elevationSampler, fs_TexCoord);
	vec4 pzfc = zfc;//textureTile(parent_elevationSampler, vs_Position.xy * 0.5);

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

	float h = (zfc.x * (1.0 - blend) + zfc.x * blend) / 200.0;

	float k = min(length(P) / dot(alpha, L) * 1.0000003, 1.0);
	float hPrime = (h + R * (1.0 - k)) / k;

#ifdef CUBE_PROJECTION
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(P + vec3(0.0, 0.0, h), 1.0);
#else
	gl_Position = (C + hPrime * N) * alphaPrime;
#endif

	fs_Position = vec4((deformation.radius + h) * normalize(deformation.localToWorld * P), 1.0);


	float u_ZFar = 100000.0;

	float FCOEF = 2.0 / log2(u_ZFar + 1.0);
	fs_HALF_FCOEF = 0.5 * FCOEF;
	gl_Position.z = (log2(max(0.000001, 1.0 + gl_Position.w)) * FCOEF - 1.0) * gl_Position.w;
	fs_LogZ = 1.0 + gl_Position.w;
}
