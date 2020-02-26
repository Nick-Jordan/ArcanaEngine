#version 400

#ifdef ATTRIB_POSITION
layout(location = ATTRIB_POSITION) in vec3 vs_Position;
out vec3 fs_Position;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;
#endif

#ifdef ATTRIB_NORMALS
layout(location = ATTRIB_NORMALS) in vec3 vs_Normal;
out vec3 fs_Normal;
uniform mat3 u_NormalMatrix;
#endif

#ifdef ATTRIB_COLOR
layout(location = ATTRIB_COLOR) in vec4 vs_Color;
out vec4 fs_Color;
#endif

#ifdef ATTRIB_TANGENT
layout(location = ATTRIB_TANGENT) in vec3 vs_Tangent;
#endif

#ifdef ATTRIB_BINORMAL
layout(location = ATTRIB_BINORMAL) in vec3 vs_Binormal;
#endif

#ifdef ATTRIB_TEXCOORD0
layout(location = ATTRIB_TEXCOORD0) in vec2 vs_TexCoord0;
out vec2 fs_TexCoord0;
#endif

#ifdef ATTRIB_TEXCOORD1
layout(location = ATTRIB_TEXCOORD1) in vec2 vs_TexCoord1;
out vec2 fs_TexCoord1;
#endif

#ifdef ATTRIB_TEXCOORD2
layout(location = ATTRIB_TEXCOORD2) in vec2 vs_TexCoord2;
out vec2 fs_TexCoord2;
#endif

#ifdef ATTRIB_TEXCOORD3
layout(location = ATTRIB_TEXCOORD3) in vec2 vs_TexCoord3;
out vec2 fs_TexCoord3;
#endif

#ifdef ATTRIB_TEXCOORD4
layout(location = ATTRIB_TEXCOORD4) in vec2 vs_TexCoord4;
out vec2 fs_TexCoord4;
#endif

#ifdef ATTRIB_TEXCOORD5
layout(location = ATTRIB_TEXCOORD5) in vec2 vs_TexCoord5;
out vec2 fs_TexCoord5;
#endif

#ifdef ATTRIB_TEXCOORD6
layout(location = ATTRIB_TEXCOORD6) in vec2 vs_TexCoord6;
out vec2 fs_TexCoord6;
#endif

#ifdef ATTRIB_TEXCOORD7
layout(location = ATTRIB_TEXCOORD7) in vec2 vs_TexCoord7;
out vec2 fs_TexCoord7;
#endif

void main()
{
	#ifdef ATTRIB_NORMALS
	fs_Normal = normalize(u_NormalMatrix * vs_Normal);
	#endif
	
	#ifdef ATTRIB_TEXCOORD0
	fs_TexCoord0 = vs_TexCoord0;
	#endif

	#ifdef ATTRIB_POSITION
	fs_Position = (u_ModelMatrix * vec4(vs_Position, 1.0)).xyz;
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(fs_Position, 1.0);
	#endif
}