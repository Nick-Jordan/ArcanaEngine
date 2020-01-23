#include "Technique.h"

#include "ResourceManager.h"
#include "ResourceCreator.h"

#include "Array.h"

#include "StringUtils.h"

namespace Arcana
{
	Technique::Technique() : Object("Technique"), _numPasses(0)// , _passes(nullptr)
	{

	}

	Technique::Technique(uint32 numPasses) : Object("Technique"), _numPasses(numPasses)//, _passes(nullptr)
	{
		//_passes = new Shader[_numPasses];
		_passes.addDefaulted(numPasses);
	}

	Technique::Technique(const Shader& shader) : Object("Technique"), _numPasses(1)//, _passes(nullptr)
	{
		//_passes = new Shader[1];
		//_passes[0] = shader;

		_passes.addDefaulted(1);
		_passes[0] = shader;
	}

	Technique::Technique(const Technique& technique) : Object("Technique"), _numPasses(technique._numPasses)
	{
		//_passes = new Shader[_numPasses];

		_attributes.attributes = _attributes.attributes;
		_attributes.textureAttributes = _attributes.textureAttributes;

		//memcpy(_passes, technique._passes, _numPasses * sizeof(Shader));

		_passes.addDefaulted(_numPasses);
		for (int32 i = 0; i < _numPasses; i++)
		{
			_passes[i] = technique._passes[i];
		}
	}


	Technique::~Technique()
	{
		//AE_DELETE_ARRAY(_passes);
		//_passes.clear();
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
		/*if (_numPasses == 0 && _passes == nullptr)
		{
			_numPasses = index + 1;
			_passes = new Shader[_numPasses];
		}*/

		if (_numPasses == 0)
		{
			_numPasses = index + 1;
			_passes.addDefaulted(_numPasses);
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

	void Technique::addAttribute(const std::string& name, Vector2f value)
	{
		_attributes.attributes.add(MaterialAttribute(name, value));
	}

	void Technique::addAttribute(const std::string& name, Vector3f value)
	{
		_attributes.attributes.add(MaterialAttribute(name, value));
	}

	void Technique::addAttribute(const std::string& name, Vector4f value)
	{
		_attributes.attributes.add(MaterialAttribute(name, value));
	}

	void Technique::addAttribute(const std::string& name, LinearColor value, bool useTransparency)
	{
		if (useTransparency)
		{
			Vector4f v = value.toVector4();
			_attributes.attributes.add(MaterialAttribute(name, v));
		}
		else
		{
			Vector3f v = value.toVector3();
			_attributes.attributes.add(MaterialAttribute(name, v));
		}
	}

	void Technique::addAttribute(const std::string& name, Color value, bool useTransparency)
	{
		addAttribute(name, value.asLinear(), useTransparency);
	}

	void Technique::bindAttribute(const std::string& name, const MaterialFloatAttributeBinding& binding)
	{
		_attributes.attributes.add(MaterialAttribute(name, binding));
	}

	void Technique::bindAttribute(const std::string& name, const MaterialVector2AttributeBinding& binding)
	{
		_attributes.attributes.add(MaterialAttribute(name, binding));
	}

	void Technique::bindAttribute(const std::string& name, const MaterialVector3AttributeBinding& binding)
	{
		LOGF(Warning, CoreEngine, "binding instance references: %d", binding._callbackInstance->referenceCount());
		_attributes.attributes.add(MaterialAttribute(name, binding));
	}

	void Technique::bindAttribute(const std::string& name, const MaterialVector4AttributeBinding& binding)
	{
		_attributes.attributes.add(MaterialAttribute(name, binding));
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

		//_passes = new Shader[_numPasses];

		//memcpy(_passes, technique._passes, _numPasses * sizeof(Shader));

		_passes.addDefaulted(_numPasses);
		for (int32 i = 0; i < _numPasses; i++)
		{
			_passes[i] = technique._passes[i];
		}

		return *this;
	}

	class TechniqueResource : public ResourceCreator<Technique>
	{
	public:

		TechniqueResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data)
			: ResourceCreator<Technique>(id, type, data)
		{
			uint32 numPasses = data.getUint32Parameter("numPasses");

			_numPasses = numPasses > 0 ? numPasses : 1;

			//_passes = new Shader[numPasses];
			_passes.addDefaulted(_numPasses);

			uint32 count = 0;

			for (auto iter = data.getDataPoints().begin(); iter != data.getDataPoints().end(); iter++)
			{
				const ResourceDataPoint& dataPoint = *iter;

				MaterialAttribute attribute;
				attribute.setName(dataPoint.Name);

				if (dataPoint.Type == Types::Float)
				{
					attribute.setValue(dataPoint.FloatData);
				}
				//temp
				else if (dataPoint.Type == Types::Vec3f)
				{
					std::string s = dataPoint.StringData;
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
				else if (dataPoint.Type == Types::Vec4f)
				{
					std::string s = dataPoint.StringData;
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

				addAttribute(attribute);
			}

			for (auto iter = data.getAdditionalData().begin(); iter != data.getAdditionalData().end(); iter++)
			{
				auto dataPoint = *iter;

				if (dataPoint.key == "pass" || dataPoint.key == "shader")
				{
					const ResourceData& dataPointResourceData = dataPoint.value;

					std::string shaderName = id.getName() + "_shader_" + std::to_string(count);

					LoadResourceTask<Shader>* task = ResourceManager::instance().buildResource<Shader>(GlobalObjectID(shaderName), dataPoint.key, dataPointResourceData);
					
					if (task)
					{
						task->wait();
						shaderTasks.add(task);
					}
				}
				/*else  TEXTURE ??????
				{
					const ResourceData& dataPointResourceData = dataPoint.second;

					/*Texture* texture = ResourceManager::instance().buildResource<Texture>(dataPoint.first, dataPoint.first, dataPointResourceData);
					

					MaterialAttribute attribute;
					attribute.setName(dataPoint.first);

					/*if (texture)
					{
						attribute.setValue(texture);
					}
				}*/
			}

			for (auto iter = data.getResourceDependencies().begin(); iter != data.getResourceDependencies().end(); iter++)
			{
				auto res = *iter;

				if (StringUtils::startsWith(res.Type, "texture"))
				{
					LoadResourceTask<Texture>* task = ResourceManager::instance().loadResource<Texture>(data.getResourceDependency(res.Name));

					if (task)
					{
						task->wait();
						textureTasks.add(MakePair(task, res.Name));
					}
				}
				else if (res.Type == "pass" || res.Type == "shader")
				{
					LoadResourceTask<Shader>* task = ResourceManager::instance().loadResource<Shader>(data.getResourceDependency(res.Name));

					if (task)
					{
						task->wait();
						shaderTasks.add(task);
					}
				}
			}
		}

		virtual void syncInitialize() override
		{
			//SHADER ID NOT CORRECT WHEN OBJECT ISN'T DISPLAYING
			//ID BECOMES JUNK

			Shader* shader = new Shader();
			shader->createProgram(Shader::Vertex, "resources/cube_vert.glsl");
			shader->createProgram(Shader::Fragment, "resources/ftl_cube_frag.glsl");

			shader->reference();
			setPass(0, *shader);
			shader->release();

			//fix shader loading

			/*uint32 count = 0;
			for (auto i = shaderTasks.createConstIterator(); i; i++)
			{
				auto task = *i;

				Shader* shader = task->get();
				
				if (shader)
				{
					shader->reference();
					setPass(count++, *shader);
					shader->release();
				}
			}*/

			for (auto i = textureTasks.createConstIterator(); i; i++)
			{
				auto task = *i;

				Texture* texture = task.key->get();
				if (texture)
				{
					addAttribute(task.value, texture);
				}
			}
		}

	private:

		Array<LoadResourceTask<Shader>*> shaderTasks;
		Array<KeyValuePair<LoadResourceTask<Texture>*, std::string>> textureTasks;
	};

	Resource::Type<TechniqueResource, true> techniqueResource("technique");
}
