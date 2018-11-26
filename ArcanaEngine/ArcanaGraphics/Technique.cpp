#include "Technique.h"

#include "Material.h"

#include "ResourceManager.h"
#include "ResourceCreator.h"

namespace Arcana
{

	Technique::Technique() : Object("Technique"), _numPasses(0), _passes(nullptr), _needsMaterialAttributes(true)
	{

	}

	Technique::Technique(uint32 numPasses) : Object("Technique"), _numPasses(numPasses), _passes(nullptr), _needsMaterialAttributes(true)
	{
		_passes = new Shader[_numPasses];
	}

	Technique::Technique(const Shader& shader) : Object("Technique"), _numPasses(1), _passes(nullptr), _needsMaterialAttributes(true)
	{
		_passes = new Shader[1];
		_passes[0] = shader;
	}

	Technique::Technique(const Technique& technique) : Object("Technique"), _numPasses(technique._numPasses), _needsMaterialAttributes(technique._needsMaterialAttributes)
	{
		_passes = new Shader[_numPasses];

		memcpy(_passes, technique._passes, _numPasses * sizeof(Shader));
	}


	Technique::~Technique()
	{
		AE_DELETE_ARRAY(_passes);
	}

	uint32 Technique::getPassCount() const
	{
		return _numPasses;
	}

	Shader* Technique::getPass(uint32 index)
	{
		if (index < _numPasses)
		{
			return &_passes[index];
		}
		return nullptr;
	}

	void Technique::setPass(uint32 index, const Shader& shader)
	{
		if (index < _numPasses)
		{
			_passes[index] = shader;
		}
	}

	bool Technique::needsMaterialAttributes() const
	{
		return _needsMaterialAttributes;
	}

	void Technique::needsMaterialAttributes(bool needsMaterialAttributes)
	{
		_needsMaterialAttributes = needsMaterialAttributes;
	}

	bool Technique::operator==(const Technique& t) const
	{
		if (t._numPasses != _numPasses)
			return false;

		for (uint32 i = 0; i < _numPasses; i++)
		{
			if (_passes[i].getId() != t._passes[i].getId())
			{
				return false;
			}
		}

		return true;
	}

	bool Technique::operator != (const Technique& t) const
	{
		return !(*this == t);
	}

	Technique& Technique::operator=(const Technique& technique)
	{
		_numPasses = technique._numPasses;

		_passes = new Shader[_numPasses];

		memcpy(_passes, technique._passes, _numPasses * sizeof(Shader));

		return *this;
	}

	class TechniqueResource : public ResourceCreator<Technique>
	{
	public:

		TechniqueResource(const std::string& name, const std::string& type, const ResourceData& data)
			: ResourceCreator<Technique>(name, type, data)
		{
			uint32 numPasses = data.getUint32Parameter("num_passes");
			bool needsMaterial = data.getBoolParameter("needs_material_attributes");

			_numPasses = numPasses;

			if (numPasses > 0)
			{
				_passes = new Shader[numPasses];

				needsMaterialAttributes(needsMaterial);

				uint32 count = 0;

				std::vector<ResourceDataPoint>::const_iterator iter;
				for (iter = data.getDataPoints().begin(); iter != data.getDataPoints().end(); iter++)
				{
					const ResourceDataPoint& dataPoint = *iter;

					if (dataPoint.hasResourceData)
					{
						if (dataPoint.name == "pass" || dataPoint.name == "shader")
						{
							const ResourceData& dataPointResourceData = dataPoint.resourceData;

							std::string shaderName = name + "_shader_" + std::to_string(count);

							Shader* shader = ResourceManager::instance().buildResource<Shader>(shaderName, dataPoint.name, dataPointResourceData);
							shader->reference();

							setPass(count++, *shader);

							shader->release();
						}
					}
				}
			}
		}
	};

	Resource::Type<TechniqueResource> techniqueResource("technique");
}
