struct IndirectLightData
{
	sampler3D data;
	vec3 boundsMin;
	vec3 boundsMax;	
};

uniform IndirectLightData u_IndirectLightData;

vec3 getIndirectLight(IndirectLightData lightData, vec3 position)
{
//????????????
	lightData.boundsMax = vec3(10.0);
	lightData.boundsMin = vec3(-10.0);

	if(position.x > lightData.boundsMax.x
		|| position.y > lightData.boundsMax.y
		|| position.z > lightData.boundsMax.z
		|| position.x < lightData.boundsMin.x
		|| position.y < lightData.boundsMin.y
		|| position.z < lightData.boundsMin.z)
	{
		return vec3(0.0);
	}

	position.x = range(position.x, lightData.boundsMin.x, lightData.boundsMax.x, 0.0, 1.0);
	position.y = range(position.y, lightData.boundsMin.y, lightData.boundsMax.y, 0.0, 1.0);
	position.z = range(position.z, lightData.boundsMin.z, lightData.boundsMax.z, 0.0, 1.0);

	return texture(lightData.data, position).rgb;
}