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