#version 400

layout(location = 0) out vec4 fs_FragColor;

uniform sampler2D u_Texture0;

//#ifdef DISTANCE_FIELD
//uniform vec2 u_cutoff;
//#endif

in vec2 fs_TexCoords;
in vec4 fs_Color;


void main()
{ 
    #ifdef DISTANCE_FIELD
    
	vec2 cutoff = vec2(1.0);

    fs_FragColor = fs_Color;
    float distance = texture(u_Texture0, fs_TexCoords).r;
    float smoothing = fwidth(distance);
    float alpha = smoothstep(0.5 - smoothing * cutoff.x, 0.5 + smoothing * cutoff.y, distance);
    fs_FragColor.a = alpha * fs_Color.a;
    
    #else
    
    fs_FragColor = fs_Color; 
	fs_FragColor.a = texture(u_Texture0, fs_TexCoords).r * fs_Color.a;

    #endif
	
	if(fs_FragColor.a == 0.0) discard;
}