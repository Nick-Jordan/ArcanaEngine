#version 400

layout(location = 0) out vec4 fs_PositionAO;
layout(location = 1) out vec4 fs_NormalRoughness;
layout(location = 2) out vec4 fs_AlbedoSpecular;
layout(location = 3) out vec4 fs_EmissiveMetallic;

in vec3 fs_Position;
in vec3 fs_Normal;
in vec2 fs_TexCoord;

uniform sampler2D baseColor;
uniform sampler2D roughness;
uniform sampler2D metallic;
uniform sampler2D normals;
uniform sampler2D ao;

vec3 getNormal()
{
    vec3 tangentNormal = texture(normals, fs_TexCoord).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(fs_Position);
    vec3 Q2  = dFdy(fs_Position);
    vec2 st1 = dFdx(fs_TexCoord);
    vec2 st2 = dFdy(fs_TexCoord);

    vec3 N   = normalize(fs_Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{
	vec3 normal = getNormal();

	fs_PositionAO = vec4(fs_Position, texture(ao, fs_TexCoord).r);
	fs_NormalRoughness = vec4(normal, texture(roughness, fs_TexCoord).r);
	fs_AlbedoSpecular = vec4(texture(baseColor, fs_TexCoord).rgb, 1.0);
	fs_EmissiveMetallic = vec4(0.0, 0.0, 0.0, texture(metallic, fs_TexCoord).r);
}