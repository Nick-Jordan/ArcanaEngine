#include "Technique.h"

#include "ResourceManager.h"
#include "ResourceCreator.h"

namespace Arcana
{

	Technique::Technique() : Object("Technique"), _numPasses(0), _passes(nullptr)
	{

	}

	Technique::Technique(uint32 numPasses) : Object("Technique"), _numPasses(numPasses), _passes(nullptr)
	{
		_passes = new Shader[_numPasses];
	}

	Technique::Technique(const Shader& shader) : Object("Technique"), _numPasses(1), _passes(nullptr)
	{
		_passes = new Shader[1];
		_passes[0] = shader;
	}

	Technique::Technique(const Technique& technique) : Object("Technique"), _numPasses(technique._numPasses)
	{
		_passes = new Shader[_numPasses];

		_attributes.attributes = _attributes.attributes;
		_attributes.textureAttributes = _attributes.textureAttributes;

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
		if (_numPasses == 0 && _passes == nullptr)
		{
			_numPasses = index + 1;
			_passes = new Shader[_numPasses];
		}

		if (index < _numPasses)
		{
			_passes[index] = shader;
		}
	}

	void Technique::addAttribute(const MaterialAttribute& attribute)
	{
		_attributes.attributes.add(attribute);

		if (attribute.getType() == MaterialAttribute::Texture)
		{
			_attributes.textureAttributes.add(attribute);
		}
	}

	void Technique::addAttribute(const std::string& name, float value)
	{
		_attributes.attributes.add(MaterialAttribute(name, value));
	}

	void Technique::addAttribute(const std::string& name, Texture* value)
	{
		_attributes.attributes.add(MaterialAttribute(name, value));
		_attributes.textureAttributes.add(MaterialAttribute(name, value));
	}

	void Technique::addAttribute(const std::string& name, Vector3f value)
	{
		_attributes.attributes.add(MaterialAttribute(name, value));
	}

	void Technique::addAttribute(const std::string& name, Vector4f value)
	{
		_attributes.attributes.add(MaterialAttribute(name, value));
	}

	void Technique::removeAttribute(const std::string& name)
	{
		int32 index = _attributes.attributes.indexOfByPredicate([=](MaterialAttribute attr) {return attr.getName() == name; });

		if (index != -1)
		{
			_attributes.attributes.removeAt(index);
		}
	}

	MaterialAttribute* Technique::getAttribute(const std::string& name)
	{
		MaterialAttribute* attribute = _attributes.attributes.findByPredicate([=](MaterialAttribute attr) {return attr.getName() == name; });

		return attribute;
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

			_numPasses = numPasses;

			if (numPasses > 0)
			{
				_passes = new Shader[numPasses];

			}
			uint32 count = 0;

			std::vector<ResourceDataPoint>::const_iterator iter;
			for (iter = data.getDataPoints().begin(); iter != data.getDataPoints().end(); iter++)
			{
				const ResourceDataPoint& dataPoint = *iter;

				if (dataPoint.hasResourceData)
				{
					if (numPasses > 0)
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
				else
				{
					MaterialAttribute attribute;
					attribute.setName(dataPoint.name);

					LOGF(Info, CoreEngine, "name: %s", attribute.getName().c_str());

					if (dataPoint.type == Types::Float)
					{
						attribute.setValue(dataPoint.floatData);
					}
					else if (dataPoint.type == Types::Vec3f)
					{
						std::string s = dataPoint.stringData;
						Vector3f vec;

						size_t pos = s.find(",");
						vec.x = stof(s.substr(0, pos));
						s.erase(0, pos + 1);
						pos = s.find(",");
						vec.y = stof(s.substr(0, pos));
						s.erase(0, pos + 1);
						pos = s.find(",");
						vec.z = stof(s.substr(0, pos));
						s.erase(0, pos + 1);

						attribute.setValue(vec);
					}
					else if (dataPoint.type == Types::Vec4f)
					{
						std::string s = dataPoint.stringData;
						Vector4f vec;

						size_t pos = s.find(",");
						vec.x = stof(s.substr(0, pos));
						s.erase(0, pos + 1);
						pos = s.find(",");
						vec.y = stof(s.substr(0, pos));
						s.erase(0, pos + 1);
						pos = s.find(",");
						vec.z = stof(s.substr(0, pos));
						s.erase(0, pos + 1);
						pos = s.find(",");
						vec.w = stof(s.substr(0, pos));
						s.erase(0, pos + 1);

						attribute.setValue(vec);
					}
					else if (dataPoint.isResourceDependency)
					{
						Texture* texture = nullptr;
						if (data._inDatabase)
						{
							texture = ResourceManager::instance().loadResource<Texture>(dataPoint.stringData);
						}
						else
						{
							texture = ResourceManager::instance().loadResource<Texture>(data._file, dataPoint.stringData);
						}

						if (texture)
						{
							attribute.setValue(texture);
						}
					}

					addAttribute(attribute);
				}
			}
		}
	};

	Resource::Type<TechniqueResource> techniqueResource("technique");
}
