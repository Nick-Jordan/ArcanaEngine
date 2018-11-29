#version 400

layout(location = 0) out vec4 fs_FragColor;

in vec4 fs_Position;

uniform struct {
    vec4 offset;
    vec4 camera;
    vec2 blending;
} deformation;

void main()
{
	fs_FragColor = vec4(vec3(0.2 + 0.2 * sin(0.1 * length(fs_Position.xy))), 1.0);
    fs_FragColor.r += mod(dot(floor(deformation.offset.xy / deformation.offset.z + 0.5), vec2(1.0)), 2.0);
}