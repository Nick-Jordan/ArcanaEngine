#version 400

#extension GL_ARB_conservative_depth : enable 
layout(depth_greater) out float gl_FragDepth;

layout(location = 0) out vec4 fs_FragColor;
layout(location = 1) out vec4 fs_EmissiveColor;

in vec3 fs_Position;
in vec2 fs_TexCoord;
in vec4 fs_Offset;

in float fs_HALF_FCOEF;
in float fs_LogZ;

uniform vec3 u_CameraPosition;
uniform vec3 u_WorldSunDir;


void main()
{

	fs_FragColor = vec4(0.5, 0.5, 0.5, 1.0);
	
	fs_EmissiveColor = vec4(0.0);

	#ifdef DEBUG_QUADTREE
    fs_FragColor.r += mod(dot(floor(fs_Offset.xy / fs_Offset.z + 0.5), vec2(1.0)), 2.0);
    #endif

    gl_FragDepth = log2(fs_LogZ) * fs_HALF_FCOEF;

}