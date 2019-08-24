float range(float x, float lo1, float hi1, float lo2, float hi2)
{
	float scale = (hi2 - lo2) / (hi1 - lo1);
    return (lo2 + ((x - lo1) * scale));
}

vec2 range(vec2 x, vec2 lo1, vec2 hi1, vec2 lo2, vec2 hi2)
{
	vec2 scale = (hi2 - lo2) / (hi1 - lo1);
    return (lo2 + ((x - lo1) * scale));
}