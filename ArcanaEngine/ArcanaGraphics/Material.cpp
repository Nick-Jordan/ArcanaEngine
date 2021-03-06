#include "Material.h"

#include "Types.h"
#include "ArcanaLog.h"

#include "ResourceManager.h"
#include "ResourceCreator.h"

#include "StringUtils.h"

namespace Arcana
{
	Material::Material() : Object("Material"), _id("material"), _currentTechnique(0)
	{

	}

	Material::Material(const std::string& name) : Object("Material"), _id(name), _currentTechnique(0)
	{
	}

	Material::Material(const Material& material)
	{
		//copy
	}

	Material& Material::operator=(const Material& material)
	{
		//assign
		return *this;
	}

	Material::~Material()
	{
		for (auto iter = _techniques.createIterator(); iter; iter++)
		{
			AE_RELEASE(*iter);
		}

		_cleanShaders.clear();
	}


	void Material::addAttribute(const MaterialAttribute& attribute, uint32 techniqueIndex)
	{
		Technique* technique = getTechnique(techniqueIndex);

		if (technique)
		{
			LOGF(Info, CoreEngine, "Attribute %s added to technique %d", attribute.getName().c_str(), techniqueIndex);

			technique->addAttribute(attribute);
			_cleanShaders.clear();//?
		}
		else
		{
			_earlyAttributes.push_back(MakePair(techniqueIndex, attribute));
		}
	}

	void Material::addAttribute(const std::string& name, float value, uint32 techniqueIndex)
	{
		Technique* technique = getTechnique(techniqueIndex);

		if (technique)
		{
			technique->addAttribute(name, value);
			_cleanShaders.clear();
		}
		else
		{
			_earlyAttributes.push_back(MakePair(techniqueIndex, MaterialAttribute(name, value)));
		}
	}

	void Material::addAttribute(const std::string& name, Texture* value, uint32 techniqueIndex)
	{
		Technique* technique = getTechnique(techniqueIndex);

		if (technique)
		{
			technique->addAttribute(name, value);
		}
		else
		{
			_earlyAttributes.push_back(MakePair(techniqueIndex, MaterialAttribute(name, value)));
		}
	}

	void Material::addAttribute(const std::string& name, Vector2f value, uint32 techniqueIndex)
	{
		Technique* technique = getTechnique(techniqueIndex);

		if (technique)
		{
			technique->addAttribute(name, value);
			_cleanShaders.clear();//?
		}
		else
		{
			_earlyAttributes.push_back(MakePair(techniqueIndex, MaterialAttribute(name, value)));
		}
	}

	void Material::addAttribute(const std::string& name, Vector3f value, uint32 techniqueIndex)
	{
		Technique* technique = getTechnique(techniqueIndex);

		if (technique)
		{
			technique->addAttribute(name, value);
			_cleanShaders.clear();//?
		}
		else
		{
			_earlyAttributes.push_back(MakePair(techniqueIndex, MaterialAttribute(name, value)));
		}
	}

	void Material::addAttribute(const std::string& name, Vector4f value, uint32 techniqueIndex)
	{
		Technique* technique = getTechnique(techniqueIndex);

		if (technique)
		{
			technique->addAttribute(name, value);
			_cleanShaders.clear();//?
		}
		else
		{
			_earlyAttributes.push_back(MakePair(techniqueIndex, MaterialAttribute(name, value)));
		}
	}

	void Material::addAttribute(const std::string& name, LinearColor value, bool useTransparency, uint32 techniqueIndex)
	{
		Technique* technique = getTechnique(techniqueIndex);

		if (technique)
		{
			technique->addAttribute(name, value, useTransparency);
			_cleanShaders.clear();//?
		}
		else
		{
			if (useTransparency)
			{
				Vector4f v = value.toVector4();
				_earlyAttributes.push_back(MakePair(techniqueIndex, MaterialAttribute(name, v)));
			}
			else
			{
				Vector3f v = value.toVector3();
				_earlyAttributes.push_back(MakePair(techniqueIndex, MaterialAttribute(name, v)));
			}
		}
	}

	void Material::addAttribute(const std::string& name, Color value, bool useTransparency, uint32 techniqueIndex)
	{
		addAttribute(name, value.asLinear(), useTransparency, techniqueIndex);
	}

	void Material::bindAttribute(const std::string& name, const MaterialFloatAttributeBinding& binding, uint32 techniqueIndex)
	{
		Technique* technique = getTechnique(techniqueIndex);

		if (technique)
		{
			technique->bindAttribute(name, binding);
			_cleanShaders.clear();//?
		}
	}

	void Material::bindAttribute(const std::string& name, const MaterialVector2AttributeBinding& binding, uint32 techniqueIndex)
	{
		Technique* technique = getTechnique(techniqueIndex);

		if (technique)
		{
			technique->bindAttribute(name, binding);
			_cleanShaders.clear();//?
		}
	}

	void Material::bindAttribute(const std::string& name, const MaterialVector3AttributeBinding& binding, uint32 techniqueIndex)
	{
		Technique* technique = getTechnique(techniqueIndex);

		if (technique)
		{
			technique->bindAttribute(name, binding);
			_cleanShaders.clear();//?
		}
	}

	void Material::bindAttribute(const std::string& name, const MaterialVector4AttributeBinding& binding, uint32 techniqueIndex)
	{
		Technique* technique = getTechnique(techniqueIndex);

		if (technique)
		{
			technique->bindAttribute(name, binding);
			_cleanShaders.clear();//?
		}
	}

	void Material::removeAttribute(const std::string& name, uint32 techniqueIndex)
	{
		Technique* technique = getTechnique(techniqueIndex);

		if (technique)
		{
			technique->removeAttribute(name);
		}
	}

	MaterialAttribute* Material::getAttribute(const std::string& name, uint32 techniqueIndex)
	{
		Technique* technique = getTechnique(techniqueIndex);

		if (technique)
		{
			return technique->getAttribute(name);
		}

		return nullptr;
	}

	void Material::addTechnique(Technique* technique)
	{
		if (technique)
		{
			technique->reference();

			uint32 currentIndex = _techniques.size();

			for (auto i = _earlyAttributes.begin(); i != _earlyAttributes.end();)
			{
				auto pair = *i;

				if (pair.key == currentIndex)
				{
					technique->addAttribute(pair.value);
					i = _earlyAttributes.erase(i);
				}
				else
				{
					i++;
				}
			}

			_techniques.add(technique);
		}
	}

	Technique* Material::getTechnique(uint32 index)
	{
		if (index < _techniques.size())
		{
			return _techniques[index];
		}

		return nullptr;
	}

	uint32 Material::getTechniqueCount() const
	{
		return _techniques.size();
	}

	Technique* Material::getCurrentTechnique()
	{
		if (_currentTechnique < _techniques.size())
		{
			return _techniques[_currentTechnique];
		}

		return nullptr;
	}

	void Material::setCurrentTechnique(uint32 index)
	{
		_currentTechnique = index;
		if (_currentTechnique >= _techniques.size())
		{
			LOGF(Warning, CoreEngine, "Current technique set to invalid index, %d", _currentTechnique);
		}
	}

	void Material::setCurrentTechnique(Technique* technique)
	{
		if (!_techniques.contains(technique))
		{
			technique->reference();
			_techniques.add(technique);
			_currentTechnique = _techniques.size() - 1;
		}
		else
		{
			_currentTechnique = _techniques.find(technique);
		}
	}

	bool Material::usesTexture(const Texture* texture)
	{
		if (texture)
		{
			return getCurrentTechnique()->_attributes.textureAttributes.containsByPredicate([&](const MaterialAttribute& attr) { return attr.getTextureValue() == texture; });
		}

		return false;
	}

	void Material::passMaterialAttributes(Shader* shader, Technique* technique)
	{
		if (!shader || !technique)
			return;

		for (auto i = technique->_attributes.textureAttributes.createIterator(); i; i++)
		{
			MaterialAttribute& attr = *i;

			Uniform uniform = shader->getUniform(attr.getName());

			//GLint params;
			//glGetIntegerv(GL_TEXTURE_BINDING_2D, &params);
			//LOGF(Info, CoreEngine, "Binding2D: %d, id: %d, unit: %d", params, attr.getTextureValue()->getId(), attr.getTextureUnit());
			//LOGF(Info, CoreEngine, "uniform: %s", attr.getName().c_str());

			//if (uniform)
			{
				//if (attr.isTextureBindDirty())
				{
					uniform.setValue(attr.getTextureUnit());
					attr.setTextureBindDirty(false);
				}
			}
		}

		if (!_cleanShaders.contains(shader))
		{
			for (auto i = technique->_attributes.attributes.createIterator(); i; i++)
			{
				MaterialAttribute& attr = *i;

				if (attr.getType() == MaterialAttribute::Texture)
					continue;

				Uniform uniform = shader->getUniform(attr.getName());

				//if (uniform)
				{
					if (attr.getType() == MaterialAttribute::Number)
					{
						uniform.setValue(attr.getFloatValue());
					}
					else if (attr.getType() == MaterialAttribute::Vector2)
					{
						uniform.setValue(attr.getVector2Value());
					}
					else if (attr.getType() == MaterialAttribute::Vector3)
					{
						uniform.setValue(attr.getVector3Value());
					}
					else if (attr.getType() == MaterialAttribute::Vector4)
					{
						uniform.setValue(attr.getVector4Value());
					}
					else if (attr.getType() == MaterialAttribute::FloatBinding)
					{
						uniform.setValue(attr.getFloatBinding());
					}
					else if (attr.getType() == MaterialAttribute::Vector2Binding)
					{
						uniform.setValue(attr.getVector2Binding());
					}
					else if (attr.getType() == MaterialAttribute::Vector3Binding)
					{
						uniform.setValue(attr.getVector3Binding());
					}
					else if (attr.getType() == MaterialAttribute::Vector4Binding)
					{
						uniform.setValue(attr.getVector4Binding());
					}
				}
			}

			//FIX THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			//_cleanShaders.add(shader);
		}
	}

	void Material::bindMaterialTextures(Technique* technique)
	{
		if (!technique)
			return;

		for (auto i = technique->_attributes.textureAttributes.createIterator(); i; i++)
		{
			MaterialAttribute& attr = *i;

			int32 unit = attr.getTextureValue()->bind(this);

			if (unit != attr.getTextureUnit())
			{
				attr.setTextureBindDirty(true);
			}

			attr.setTextureUnit(unit);
		}
	}

	const GlobalObjectID& Material::getId() const
	{
		return _id;
	}


	MaterialAttribute* Material::getAlbedo(uint32 techniqueIndex)
	{
		MaterialAttribute* attr = getAttribute("albedo", techniqueIndex);

		if (attr)
		{
			return attr;
		}

		return getAttribute("baseColor", techniqueIndex);
	}

	MaterialAttribute* Material::getBaseColor(uint32 techniqueIndex)
	{
		MaterialAttribute* attr = getAttribute("baseColor", techniqueIndex);

		if (attr)
		{
			return attr;
		}

		return getAttribute("albedo", techniqueIndex);
	}

	MaterialAttribute* Material::getMetallic(uint32 techniqueIndex)
	{
		return getAttribute("metallic", techniqueIndex);
	}

	MaterialAttribute* Material::getRoughness(uint32 techniqueIndex)
	{
		return getAttribute("roughness", techniqueIndex);
	}

	MaterialAttribute* Material::getSpecular(uint32 techniqueIndex)
	{
		return getAttribute("specular", techniqueIndex);
	}

	MaterialAttribute* Material::getHeight(uint32 techniqueIndex)
	{
		return getAttribute("height", techniqueIndex);
	}

	MaterialAttribute* Material::getAmbientOcclusion(uint32 techniqueIndex)
	{
		return getAttribute("ambient_occlusion", techniqueIndex);
	}

	MaterialAttribute* Material::getOpacity(uint32 techniqueIndex)
	{
		return getAttribute("opacity", techniqueIndex);
	}

	MaterialAttribute* Material::getEmissive(uint32 techniqueIndex)
	{
		return getAttribute("emissive", techniqueIndex);
	}

	class MaterialResource : public ResourceCreator<Material>
	{
	public:

		MaterialResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data, Scheduler* dependencyScheduler)
			: ResourceCreator<Material>(id, type, data, dependencyScheduler)
		{
			//setName(name);

			uint32 techniqueCount = 0;

			for (auto iter = data.getDataPoints().begin(); iter != data.getDataPoints().end(); iter++)
			{
				const ResourceDataPoint& dataPoint = *iter;

				MaterialAttribute attribute;
				attribute.setName(dataPoint.Name);

				uint32 techniqueIndex = dataPoint.getUint32Attribute("index");

				if (dataPoint.Type == Types::Float)
				{
					attribute.setValue(dataPoint.FloatData);
				}
				//temp
				else if (dataPoint.Type == Types::Vec2f)
				{
					std::string s = dataPoint.StringData;
					Vector2f vec;

					size_t pos = s.find(",");
					vec.x = stof(s.substr(0, pos));
					s.erase(0, pos + 1);
					pos = s.find(",");
					vec.y = stof(s.substr(0, pos));
					s.erase(0, pos + 1);

					attribute.setValue(vec);
				}
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
				else if (dataPoint.Type == Types::Color)
				{
					std::string s = dataPoint.StringData;
					Color color;

					size_t pos = s.find(",");
					color.R = stoul(s.substr(0, pos));
					s.erase(0, pos + 1);
					pos = s.find(",");
					color.G = stoul(s.substr(0, pos));
					s.erase(0, pos + 1);
					pos = s.find(",");
					color.B = stoul(s.substr(0, pos));
					s.erase(0, pos + 1);

					attribute.setValue(color.asLinear().toVector3());
				}

				addAttribute(attribute, techniqueIndex);
			}

			//for (auto iter = data.getAdditionalData().begin(); iter != data.getAdditionalData().end(); iter++)
			for (int32 i = 0; i < data.getAdditionalData().size(); i++)
			{
				auto dataPoint = data.getAdditionalData()[i];

				if (dataPoint.key == "technique")
				{
					const ResourceData& dataPointResourceData = dataPoint.value;

					std::string techniqueName = id.getName() + "_technique_" + std::to_string(techniqueCount++);

					LoadResourceTask<Technique>* task = ResourceManager::instance().buildResource<Technique>(GlobalObjectID(techniqueName), dataPoint.key, dataPointResourceData, dependencyScheduler);

					if (task)
					{
						task->wait();
						techniqueTasks.add(MakePair(task, dataPointResourceData.getBoolAttribute("current")));
					}
				}
			}

			for (auto iter = data.getResourceDependencies().begin(); iter != data.getResourceDependencies().end(); iter++)
			{
				auto dataPoint = *iter;

				if (StringUtils::startsWith(dataPoint.Type, "texture"))
				{
					uint32 techniqueIndex = dataPoint.getUint32Attribute("index");

					LoadResourceTask<Texture>* task = ResourceManager::instance().loadResource<Texture>(data.getResourceDependency(dataPoint.Name), dependencyScheduler);

					if (task)
					{
						task->wait();
						textureTasks.add(MakePair(task, MakePair(dataPoint.Name, techniqueIndex)));
					}
				}
			}
		}

		virtual void syncInitialize() override
		{
			for (auto i = techniqueTasks.createConstIterator(); i; i++)
			{
				KeyValuePair<LoadResourceTask<Technique>*, bool> task = *i;

				Technique* technique = task.key->get();
				if (technique)
				{
					technique->reference();
					if (task.value)
					{
						setCurrentTechnique(technique);
					}
					else
					{
						addTechnique(technique);
					}
					technique->release();
				}
			}

			for (auto i = textureTasks.createConstIterator(); i; i++)
			{
				auto task = *i;

				MaterialAttribute attribute;
				attribute.setName(task.value.key);

				Texture* texture = task.key->get();
				if (texture)
				{
					attribute.setValue(texture);
					addAttribute(attribute, task.value.value);
				}
			}
		}

	private:

		Array<KeyValuePair<LoadResourceTask<Technique>*, bool>> techniqueTasks;
		Array<KeyValuePair<LoadResourceTask<Texture>*, KeyValuePair<std::string, uint32>>> textureTasks;
	};

	Resource::Type<MaterialResource, true> materialResource("material");
}