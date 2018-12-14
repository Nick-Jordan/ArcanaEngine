vec3 rotate(float Angle, vec3 Axis, vec3 Vector)
{
    float cosa = cos(Angle);
    float sina = sin(Angle);
    float t = 1.0 - cosa;

    mat3x3 M = mat3x3(
        t * Axis.x * Axis.x + cosa,
        t * Axis.x * Axis.y - sina * Axis.z,
        t * Axis.x * Axis.z + sina * Axis.y,
        t * Axis.x * Axis.y + sina * Axis.z,
        t * Axis.y * Axis.y + cosa,
        t * Axis.y * Axis.z - sina * Axis.x,
        t * Axis.x * Axis.z - sina * Axis.y,
        t * Axis.y * Axis.z + sina * Axis.x,
        t * Axis.z * Axis.z + cosa);

    return Vector * M;
}