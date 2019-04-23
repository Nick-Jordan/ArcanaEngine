#version 400

out vec4 fs_FragColor;

in vec4 fs_Color;
in vec4 fs_StateData;
in vec4 fs_ObjectPositionSize;
in vec3 fs_Position;
flat in int fs_Type;

float roundCorners(vec2 p, vec2 b, float r)
{
    return length(max(abs(p)-b+r,0.0))-r;
}

void main()
{
	if(fs_Type == 0 && fs_StateData.y == -1.0) //rounded rectangle
	{
		vec2 halfRes = 0.5 * fs_ObjectPositionSize.zw;
		vec2 r = fs_StateData.x * halfRes;
		vec2 coords = (fs_Position.xy - fs_ObjectPositionSize.xy);

		float b = 1.0 - roundCorners(coords - halfRes, halfRes, abs(min(r.x, r.y)));
		fs_FragColor = vec4(fs_Color.xyz, fs_Color.a * smoothstep(0.0,1.0,b));
	}
	else if(fs_Type == 0 && fs_StateData.y >= 0.0) // stroke rounded rectangle
	{
		vec2 halfRes = 0.5 * fs_ObjectPositionSize.zw;
		vec2 r = fs_StateData.x * halfRes;
		vec2 coords = (fs_Position.xy - fs_ObjectPositionSize.xy);

		float b = roundCorners(coords - halfRes, halfRes, abs(min(r.x, r.y)));
		float d = 1.0 - smoothstep(fs_StateData.y, fs_StateData.y+0.01, abs(b));
		fs_FragColor = vec4(fs_Color.xyz, fs_Color.a * d);
	}
}