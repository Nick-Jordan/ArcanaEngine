#include "MaterialMap.h"

namespace Arcana
{

	MaterialMap::MaterialMap() : Material()
	{
	}

	MaterialMap::MaterialMap(const std::string& name) : Material(name)
	{

	}

	MaterialMap::~MaterialMap()
	{
		_techniqueMapping.clear();
	}

	void MaterialMap::addTechniqueMapping(uint32 componentIndex, uint32 techniqueIndex)
	{
		if (techniqueIndex < Material::getTechniqueCount())
		{
			_techniqueMapping[componentIndex] = techniqueIndex;
		}
	}

	Technique* MaterialMap::getTechnique(uint32 index)
	{
		std::map<uint32, uint32>::iterator iter = _techniqueMapping.find(index);
		if (iter != _techniqueMapping.end())
		{
			index = (*iter).second;
		}

		return Material::getTechnique(index);
	}
}
