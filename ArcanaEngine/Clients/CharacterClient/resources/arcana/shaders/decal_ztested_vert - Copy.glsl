#version 400

layout(location = 0) in vec3 vs_Position;

out vec3 fs_WorldPosition;
out vec4 fs_Position;
out float fs_Depth;
out vec4 fs_PositionCS;
out vec4 fs_PositionVS;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;
uniform float u_FarClip;

float calculateDepth(vec4 viewPosition, float farClip);

void main()
{
	fs_WorldPosition = (u_ModelMatrix * vec4(vs_Position, 1.0)).xyz;
	vec4 viewPosition = u_ViewMatrix * vec4(fs_WorldPosition, 1.0);
	
	fs_Position = u_ProjectionMatrix * viewPosition;
	fs_Depth = calculateDepth(viewPosition, u_FarClip);
	fs_PositionCS = fs_Position;
	fs_PositionVS = viewPosition;
	
	gl_Position = fs_Position;
}

float calculateDepth(vec4 viewPosition, float farClip)
 {
	return -viewPosition.z / farClip;
}