float range(float x, float lo1, float hi1, float lo2, float hi2)
{
	float scale = (hi2 - lo2) / (hi1 - lo1);
    return (lo2 + ((x - lo1) * scale));
}