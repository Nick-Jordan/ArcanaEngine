#version 400

out vec4 fs_FragColor;

in vec4 fs_Color;
in vec4 fs_SecondaryColor;
in vec4 fs_ObjectPositionSize;
in vec2 fs_TexCoords;
in vec3 fs_Position;
in float fs_Radius;
in vec4 fs_Gradient;

uniform sampler1D u_LinearGradient;
uniform sampler1D u_BilinearGradient;
uniform sampler2D u_BoxGradient;
uniform sampler2D u_RoundedBoxGradient;

float roundCorners(vec2 p, vec2 b, float r)
{
    return length(max(abs(p)-b+r,0.0))-r;
}

void main()
{
	vec4 color;
	if(fs_Gradient.x > 0.0)
	{
		float coord = fs_TexCoords.y;
		if(fs_Gradient.x == 2)
		{
			coord = fs_TexCoords.x;
		}
		color = mix(fs_Color, fs_SecondaryColor, texture(u_LinearGradient, coord).x);
	}
	else if(fs_Gradient.y > 0.0)
	{
		float coord = fs_TexCoords.y;
		if(fs_Gradient.y == 2)
		{
			coord = fs_TexCoords.x;
		}
		color = mix(fs_Color, fs_SecondaryColor, 1.0 - texture(u_BilinearGradient, coord).x);
	}
	else if(fs_Gradient.z > 0.0)
	{
		color = mix(fs_Color, fs_SecondaryColor, texture(u_BoxGradient, fs_TexCoords).x);
	}
	else if(fs_Gradient.w > 0.0)
	{
		vec4 s = texture(u_RoundedBoxGradient, fs_TexCoords);
		
		if(s.a > 0.0)
		{
			color = mix(fs_Color, fs_SecondaryColor, 1.0 - s.x);
		}
		else
		{
			color = vec4(0.0);//do something
		}
	}
	else
	{
		color = fs_Color;
	}
	
	if(fs_Radius == 0.0)
	{
		fs_FragColor = color;
	}
	else
	{
		vec2 halfRes = 0.5 * fs_ObjectPositionSize.zw;
		vec2 r = fs_Radius * halfRes;
		vec2 coords = (fs_Position.xy - fs_ObjectPositionSize.xy);

		float b = 1.0 - roundCorners(coords - halfRes, halfRes, abs(min(r.x, r.y)));
		fs_FragColor = vec4(color.xyz, color.a * smoothstep(0.0,1.0,b));
	}
	
	if(fs_FragColor.a == 0.0)
	{
		discard;
	}
}