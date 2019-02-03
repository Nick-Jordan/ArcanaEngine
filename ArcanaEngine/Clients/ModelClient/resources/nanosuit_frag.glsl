#version 400

layout(location = 0) out vec4 fs_PositionAO;
layout(location = 1) out vec4 fs_NormalRoughness;
layout(location = 2) out vec4 fs_AlbedoSpecular;
layout(location = 3) out vec4 fs_EmissiveMetallic;

in vec3 fs_Position;
in vec3 fs_Normal;
in vec2 fs_TexCoord;

uniform mat3 u_NormalMatrix;

uniform sampler2D baseColor;
uniform sampler2D normals;
uniform float roughness;
uniform float metallic;

mat3 cotangent( vec3 N, vec3 p, vec2 uv )
{
    vec3 dp1 = dFdx( p );
    vec3 dp2 = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );
 
    vec3 dp2perp = cross( dp2, N );
    vec3 dp1perp = cross( N, dp1 );
    vec3 T = u_NormalMatrix * (dp2perp * duv1.x + dp1perp * duv2.x);
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
 
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
    return mat3( T * invmax, B * invmax, N );
}

vec3 getNormal(sampler2D normalSampler, vec3 position, vec3 normal, vec2 texCoords)
{
	vec3 tangentNormal = texture(normalSampler, texCoords).xyz * 2.0 - 1.0;
	mat3 TBN = cotangent(normal, position, texCoords);
    return normalize(TBN * tangentNormal);
}

void main()
{
	vec3 albedo = texture(baseColor, fs_TexCoord).rgb;
	vec3 normal = getNormal(normals, fs_Position, fs_Normal, fs_TexCoord);

	fs_PositionAO = vec4(fs_Position, 1.0);
	fs_NormalRoughness = vec4(normal, 0.5);
	fs_AlbedoSpecular = vec4(albedo, 1.0);
	fs_EmissiveMetallic = vec4(0.0, 0.0, 0.0, 0.5);
}