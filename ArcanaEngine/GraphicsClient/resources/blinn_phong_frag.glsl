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
  	vec3 lightPosition = vec3(10.0, 10.0, 10.0);

    vec3 color = albedo;

    vec3 ambient = 0.04 * color;

    vec3 lightDir = normalize(lightPosition - fs_Position);
    vec3 normal = normalize(fs_Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

    vec3 viewDir = normalize(u_CameraPosition - fs_Position);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specularLight = vec3(0.3) * spec;

    vec3 result = ambient + diffuse + specularLight;

    fs_FragColor = vec4(result, 1.0);
} 