#version 400

layout(location = 0) out vec4 fs_FragColor;

in vec4 fs_Position;
in vec2 fs_TexCoord;
in float fs_Height;

in float fs_HALF_FCOEF;
in float fs_LogZ;

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

uniform samplerTile surfaceSampler;

void main()
{
	vec3 terrain = textureTile(surfaceSampler, fs_TexCoord).xyz;

	fs_FragColor = vec4(terrain, 1.0);//vec4(vec3(0.2 + 0.2 * sin(0.1 * length(fs_Position.xy))), 1.0);

	#ifdef DEBUG_QUADTREE
    fs_FragColor.r += mod(dot(floor(deformation.offset.xy / deformation.offset.z + 0.5), vec2(1.0)), 2.0);
    #endif

    gl_FragDepth = log2(fs_LogZ) * fs_HALF_FCOEF;
}