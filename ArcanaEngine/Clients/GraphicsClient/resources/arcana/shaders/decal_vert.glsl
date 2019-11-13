#version 400

layout(location = 0) in vec3 vs_Position;
layout(location = 1) in vec4 vs_Color;
layout(location = 2) in vec4 vs_TexCoords;
layout(location = 3) in mat4 vs_Transform;

out vec3 fs_WorldPosition;
out vec4 fs_Position;
out float fs_Depth;
out vec4 fs_PositionCS;
out vec4 fs_PositionVS;
out vec3 fs_Color;
out float fs_Opacity;
out vec4 fs_TexCoords;
out mat4 fs_InverseTransform;
out vec3 fs_Direction;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
//uniform mat4 u_ModelMatrix;  //keep actor transform??
uniform float u_FarClip;

float calculateDepth(vec4 viewPosition, float farClip);

void main()
{
	fs_WorldPosition = (vs_Transform * vec4(vs_Position, 1.0)).xyz;
	vec4 viewPosition = u_ViewMatrix * vec4(fs_WorldPosition, 1.0);
	
	fs_Position = u_ProjectionMatrix * viewPosition;
	fs_Depth = calculateDepth(fs_Position, u_FarClip);
	fs_PositionCS = fs_Position;
	fs_PositionVS = viewPosition;
	fs_InverseTransform = inverse(vs_Transform);
	fs_Color = vs_Color.xyz;
	fs_Opacity = clamp(vs_Color.w, 0.0, 1.0);
	fs_TexCoords = vs_TexCoords;
	
	fs_Direction = normalize(vs_Transform[2].xyz);
	
	gl_Position = fs_Position;
}

float calculateDepth(vec4 position, float farClip)
 {
	return position.z / position.w;
}