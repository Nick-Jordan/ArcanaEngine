#version 400

layout(location = 0) out vec4 fs_FragColor;
layout(location = 1) out vec4 fs_EmissiveColor;

in vec3 fs_Normal;

void main()
{
	vec3 color = normalize(fs_Normal * 0.5 + 0.5);

	/*vec3 lightDir = normalize(vec3(-1, 1, 1));

    float diff = max(dot(fs_Normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, fs_Normal);

    vec3 ambient = 0.03 * color;
    vec3 diffuse = color * diff;*/

	fs_FragColor = vec4(color, 1.0);
	fs_EmissiveColor = vec4(0.0);
}