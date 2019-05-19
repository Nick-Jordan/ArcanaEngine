#version 400

in vec4 fs_Position;

uniform vec3 u_LightPosition;
uniform float u_FarPlane;

void main()
{
    float lightDistance = length(fs_Position.xyz - u_LightPosition);
    
    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / u_FarPlane;
    
    // write this as modified depth
    gl_FragDepth = lightDistance;
}