#version 400

out vec4 fs_FragColor;

in vec2 fs_TexCoord;
in vec2 fs_Position;

uniform sampler2D u_IconTexture;

uniform vec4 u_Color;
uniform float u_Radius;
uniform vec4 u_Area;

float roundCorners(vec2 p, vec2 b, float r)
{
    return length(max(abs(p)-b+r,0.0))-r;
}


void main()
{
	vec4 color = texture(u_IconTexture, fs_TexCoord) * u_Color;
	
	if(u_Radius == 0.0)
	{
		fs_FragColor = color;
	}
	else
	{
		vec2 halfRes = 0.5 * u_Area.zw;
		vec2 r = u_Radius * halfRes;
		vec2 coords = (fs_Position.xy - u_Area.xy);

		float b = 1.0 - roundCorners(coords - halfRes, halfRes, abs(min(r.x, r.y)));
		fs_FragColor = vec4(color.xyz, color.a * smoothstep(0.0,1.0,b));
	}
	
	if(fs_FragColor.a == 0.0)
	{
		discard;
	}
}