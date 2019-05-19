#version 400

out vec4 fs_FragColor;

in vec4 fs_Color;
in vec4 fs_SecondaryColor;
in vec4 fs_StateData;
in vec4 fs_ObjectPositionSize;
in vec4 fs_TexCoords;
in vec4 fs_ClipRect;
in vec3 fs_Position;
flat in int fs_Type;

float roundCorners(vec2 p, vec2 b, float r)
{
    return length(max(abs(p)-b+r,0.0))-r;
}

bool contains(vec4 rect, vec2 point)
{
		return point.x >= rect.x && point.x <= rect.x + rect.z
			&& point.y >= rect.y && point.y <= rect.y + rect.w;
}

vec4 boxGradientColor();

void main()
{
	if(fs_ClipRect.x != -1 || fs_ClipRect.y != -1 || fs_ClipRect.z != -1 || fs_ClipRect.w != -1)
	{
		if(!contains(fs_ClipRect, fs_Position.xy))
			discard;
	}

	if(fs_Type == 0 && fs_StateData.y == -1.0) //rounded rectangle
	{	
		vec4 color = fs_Color;
	
		if(fs_StateData.z > 0.0)
		{
			color = boxGradientColor();
		}
	
		if(fs_StateData.x == 0.0)
		{
			fs_FragColor = color;
			return;
		}

		vec2 halfRes = 0.5 * fs_ObjectPositionSize.zw;
		vec2 r = fs_StateData.x * halfRes;
		vec2 coords = (fs_Position.xy - fs_ObjectPositionSize.xy);

		float b = 1.0 - roundCorners(coords - halfRes, halfRes, abs(min(r.x, r.y)));
		fs_FragColor = vec4(color.xyz, color.a * smoothstep(0.0,1.0,b));
	}
	else if(fs_Type == 0 && fs_StateData.y >= 0.0) // stroke rounded rectangle
	{
		if(fs_StateData.x == 0.0)
		{
			fs_FragColor = fs_Color;
			return;
		}
		
		vec2 halfRes = 0.5 * fs_ObjectPositionSize.zw;
		vec2 r = fs_StateData.x * halfRes;
		vec2 coords = (fs_Position.xy - fs_ObjectPositionSize.xy);

		float b = roundCorners(coords - halfRes, halfRes, abs(min(r.x, r.y)));
		float d = 1.0 - abs(b) + fs_StateData.y;//smoothstep(fs_StateData.y, fs_StateData.y+0.01, abs(b));
		fs_FragColor = vec4(fs_Color.xyz, fs_Color.a * d * smoothstep(0.0,1.0,1-b));
	}
}

vec2 closestPoint(vec2 center, vec2 bounds, vec2 P)
{
	bool insideX = center.x-bounds.x < P.x && P.x < center.x+bounds.x;
	bool insideY = center.y-bounds.y < P.y && P.y < center.y+bounds.y;
	bool pointInsideRectangle = insideX && insideY;

	if (!pointInsideRectangle)
	{
    	return vec2(max(center.x-bounds.x, min(P.x, center.x + bounds.x)), max(center.y-bounds.y, min(P.y, center.y + bounds.y)));
	}

	vec2 distanceToPositiveBounds = center+bounds - P;
	vec2 distanceToNegativeBounds = -(center-bounds - P);

	float smallestX = min(distanceToPositiveBounds.x, distanceToNegativeBounds.x);
	float smallestY = min(distanceToPositiveBounds.y, distanceToNegativeBounds.y);
	float smallestDistance = min(smallestX, smallestY);

	if (smallestDistance == distanceToPositiveBounds.x)
	{
	    return vec2(center.x+bounds.x, P.y);
	}
	else if (smallestDistance == distanceToNegativeBounds.x)
	{
	    return vec2(center.x-bounds.x, P.y);
	}
	else if (smallestDistance == distanceToPositiveBounds.y)
	{
	    return vec2(P.x, center.y+bounds.y);
	}

	return vec2(P.x, center.y-bounds.y);
}

vec4 boxGradientColor()
{
	vec4 bounds = fs_TexCoords;

	vec2 point = closestPoint(bounds.xy + bounds.zw/2, bounds.zw * fs_StateData.z, fs_Position.xy);

	float d = clamp(distance(point / bounds.zw, fs_Position.xy / bounds.zw), 0.0, 1.0);

	return d * fs_Color + (1-d) * fs_SecondaryColor;//vec4(d);//mix(fs_SecondaryColor, fs_Color, d);
}