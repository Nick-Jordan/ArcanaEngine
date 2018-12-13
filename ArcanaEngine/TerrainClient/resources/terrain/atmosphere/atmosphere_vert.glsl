#version 400

uniform vec3 u_WorldSunDir;

uniform mat4 cameraToWorld;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;

layout(location = 0) in vec3 vs_Position;

out vec3 fs_Direction;
out vec3 fs_RelativeDirection;
out vec2 test;

void main() {
	vec3 WSD = u_WorldSunDir;

	fs_Direction = (cameraToWorld * vec4((u_ProjectionMatrix * vec4(vs_Position.xy, 0.5, 1.0)).xyz, 0.0)).xyz;

	// construct a rotation that transforms sundir to (0,0,1);
	float theta = acos(WSD.z);
	float phi = atan(WSD.y, WSD.x);
	mat3 rz = mat3(cos(phi), -sin(phi), 0.0, sin(phi), cos(phi), 0.0, 0.0, 0.0, 1.0);
	mat3 ry = mat3(cos(theta), 0.0, sin(theta), 0.0, 1.0, 0.0, -sin(theta), 0.0, cos(theta));
	// apply this rotation to view dir to get relative viewdir
	fs_RelativeDirection = (ry * rz) * fs_Direction;

	gl_Position = vec4(vs_Position.xy, 0.9999999, 1.0);

	test = vs_Position.xy;
}
