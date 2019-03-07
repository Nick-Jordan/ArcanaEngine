#version 400

layout(location = 0) out vec4 fs_FragColor;

uniform sampler2D u_ShadowDepthMap;

uniform vec3 u_LightDirection;

in vec3 fs_Position;
in vec3 fs_Normal;
in vec4 fs_PositionLightSpace;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(u_ShadowDepthMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;

    vec3 normal = normalize(fs_Normal);
    vec3 lightDir = normalize(u_LightDirection - fs_Position);//u_LightDirection;

    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(u_ShadowDepthMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(u_ShadowDepthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    if(projCoords.z > 1.0)
    {
        shadow = 0.0;
    }
        
    return shadow;
}

void main()
{
    float shadow = ShadowCalculation(fs_PositionLightSpace);                          
    fs_FragColor = vec4(vec3(shadow), 1.0);
}