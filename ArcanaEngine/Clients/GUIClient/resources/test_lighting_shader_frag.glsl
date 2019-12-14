#version 400

layout(location = 0) out vec4 fs_FragColor;

uniform vec3 albedo;
uniform vec3 specular;    
uniform float shininess; 

struct Light 
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 fs_Position;  
in vec3 fs_Normal;
  
uniform vec3 u_CameraPosition;

void main()
{
  	vec3 position = vec3(10.0, 10.0, 10.0);
	vec3 lightColor = vec3(0.5);
	
	vec3 ambient = albedo * 0.04;
	
    vec3 norm = normalize(fs_Normal);
    vec3 lightDir = normalize(position - fs_Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * albedo;
	
	vec3 result = ambient + diffuse;
	
    fs_FragColor = vec4(result, 1.0);
} 