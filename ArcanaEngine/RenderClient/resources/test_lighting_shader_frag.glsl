#version 400

layout(location = 0) out vec4 fs_FragColor;

uniform vec3 diffuse;
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
	Light light;
	light.position = vec3(10.0, 0.0, 0.0);
	light.ambient = vec3(1.0);
	light.diffuse = vec3(1.0, 0.0, 0.0);
	light.specular = vec3(0.0);

	vec3 a = vec3(0.5);
	
    vec3 ambient = light.ambient * a;
  	
    vec3 norm = normalize(fs_Normal);
    vec3 lightDir = normalize(light.position - fs_Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * diffuse);
    
    vec3 viewDir = normalize(u_CameraPosition - fs_Position);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light.specular * (spec * specular);  
        
    vec3 result = ambient + diffuse + specular;
    fs_FragColor = vec4(result, 1.0);
} 