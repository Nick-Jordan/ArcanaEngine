#version 400

layout(location = 0) in vec3 vs_Position;

out vec4 fs_Position;

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
} deformation;

struct samplerTile 
{
	sampler2DArray tilePool; // tile cache
	float tileLayer; // coords of currently selected tile in tile cache (u,v,layer; u,v in [0,1]^2)
	vec3 tileSize; // size of currently selected tile in tile cache (du,dv,d; du,dv in [0,1]^2, d in pixels)
};

vec4 textureTile(samplerTile tex, vec2 uv) 
{
	return texture(tex.tilePool, vec3(uv, tex.tileLayer));
}

uniform samplerTile elevationSampler;


void main()
{
	vec4 zfc = textureTile(elevationSampler, vs_Position.xy);

	vec2 v = abs(deformation.camera.xy - vs_Position.xy);
	float d = max(max(v.x, v.y), deformation.camera.z);
	float blend = clamp((d - deformation.blending.x) / deformation.blending.y, 0.0, 1.0);

	float h = zfc.z * (1.0 - blend) + zfc.y * blend;
	fs_Position = vec4(vs_Position.xy * deformation.offset.z + deformation.offset.xy, h, 1.0);

	gl_Position = u_ProjectionMatrix * u_ViewMatrix * fs_Position;


	float u_ZFar = 100000.0;

	float FCOEF = 2.0 / log2(u_ZFar + 1.0);
	fs_HALF_FCOEF = 0.5 * FCOEF;
	gl_Position.z = (log2(max(0.000001, 1.0 + gl_Position.w)) * FCOEF - 1.0) * gl_Position.w;
	fs_LogZ = 1.0 + gl_Position.w;
}
