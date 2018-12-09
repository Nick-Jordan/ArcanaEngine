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
	int childIndex;
};

vec4 textureTile(samplerTile tex, vec2 uv) 
{
	return texture(tex.tilePool, vec3(uv, tex.tileLayer));
}

uniform samplerTile elevationSampler;
uniform samplerTile parent_elevationSampler;

float getHeight(vec2 coords, float blend)
{
	vec2 parentCoords;

	if(elevationSampler.childIndex == 0)
		parentCoords = coords * 0.5;
	if(elevationSampler.childIndex == 1)
		parentCoords = coords * 0.5 + vec2(0.5, 0.0);
	if(elevationSampler.childIndex == 2)
		parentCoords = coords * 0.5 + vec2(0.5, 0.5);
	if(elevationSampler.childIndex == 2)
		parentCoords = coords * 0.5 + vec2(0.0, 0.5);

	if(elevationSampler.childIndex == -1)
	{
		float tile = textureTile(elevationSampler, coords).x;

		return tile;
	}
	else
	{
		return textureTile(elevationSampler, parentCoords).x;
	}
}

void main()
{
	fs_TexCoord = vs_Position.xy;

	vec2 v = abs(deformation.camera.xy - vs_Position.xy);
	float d = max(max(v.x, v.y), deformation.camera.z);
	float blend = clamp((d - deformation.blending.x) / deformation.blending.y, 0.0, 1.0);

	fs_Height = getHeight(fs_TexCoord, blend);//pzfc.x * (1.0 - blend) + zfc.x * blend;
	fs_Position = vec4(vs_Position.xy * deformation.offset.z + deformation.offset.xy, fs_Height / 200.0, 1.0);

	gl_Position = u_ProjectionMatrix * u_ViewMatrix * fs_Position;


	float u_ZFar = pow(10.0, 10.0);//100000.0;

	float FCOEF = 2.0 / log2(u_ZFar + 1.0);
	fs_HALF_FCOEF = 0.5 * FCOEF;
	gl_Position.z = (log2(max(0.000001, 1.0 + gl_Position.w)) * FCOEF - 1.0) * gl_Position.w;
	fs_LogZ = 1.0 + gl_Position.w;
}
