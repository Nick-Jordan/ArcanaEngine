#include "Material.h"

#include "Types.h"
#include "ArcanaLog.h"

#include "ResourceManager.h"
#include "ResourceCreator.h"

namespace Arcana
{
	Material::Material() : Object("Material"), _id("material"), _currentTechnique(0)
	{

	}

	Material::Material(const std::string& name) : Object("Material"), _id(name), _currentTechnique(0)
	{
	}
		
	Material::~Material()
	{
		for (auto iter = _techniques.createIterator(); iter; iter++)
		{
			AE_RELEASE(*iter);
		}

		_cleanShaders.empty();
	}
	
	
	void Material::addAttribute(const Attribute& attribute)
	{
		_attributes.add(attribute);
		_cleanShaders.empty();

		if (attribute.getType() == Attribute::Texture)
		{
			_textureAttributes.add(attribute);
		}
	}
		
	void Material::addAttribute(const std::string& name, float value)
	{
		_attributes.add(Attribute(name, value));
		_cleanShaders.empty();
	}

	void Material::addAttribute(const std::string& name, Texture* value)
	{
		_attributes.add(Attribute(name, value));
		_textureAttributes.add(Attribute(name, value));
	}
				
	void Material::addAttribute(const std::string& name, Vector3f value)
	{
		_attributes.add(Attribute(name, value));
		_cleanShaders.empty();
	}
		
	void Material::addAttribute(const std::string& name, Vector4f value)
	{
		_attributes.add(Attribute(name, value));
		_cleanShaders.empty();
	}
		
	void Material::removeAttribute(const std::string& name)
	{
		int32 index = _attributes.indexOfByPredicate([=](Attribute attr)  {return attr.getName() == name; });

		if(index != -1)
		{
			_attributes.removeAt(index);
		}
	}
	
	Material::Attribute* Material::getAttribute(const std::string& name)
	{
		Attribute* attribute = _attributes.findByPredicate([=](Attribute attr) {return attr.getName() == name; });

		return attribute;
	}

	void Material::addTechnique(Technique* technique)
	{
		technique->reference();
		_techniques.add(technique);
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

	bool Material::usesTexture(const Texture* texture) const
	{
		if (texture)
		{
			return _textureAttributes.containsByPredicate([&](const Attribute& attr) { return attr.getTextureValue() == texture; });
		}

		return false;
	}

	void Material::passMaterialAttributes(Shader* shader)
	{
		for (auto i = _textureAttributes.createIterator(); i; i++)
		{
			Attribute& attr = *i;

			SmartPtr<Uniform> uniform = shader->getUniform(attr.getName());

			GLint params;
			glGetIntegerv(GL_TEXTURE_BINDING_2D, &params);
			LOGF(Info, CoreEngine, "Binding2D: %d, id: %d, unit: %d", params, attr.getTextureValue()->getId(), attr.getTextureUnit());
			LOGF(Info, CoreEngine, "uniform: %p", uniform);

			if (uniform)
			{
				if (attr.isTextureBindDirty())
				{
					uniform->setValue(attr.getTextureUnit());
					attr.setTextureBindDirty(false);
				}
			}
		}

		if (!_cleanShaders.contains(shader))
		{
			for (auto i = _attributes.createConstIterator(); i; i++)
			{
				const Attribute& attr = *i;

				if (attr.getType() == Attribute::Texture)
					continue;

				SmartPtr<Uniform> uniform = shader->getUniform(attr.getName());

				if (uniform)
				{
					if (attr.getType() == Attribute::Number)
					{
						uniform->setValue(attr.getFloatValue());
					}
					else if (attr.getType() == Attribute::Vector3)
					{
						uniform->setValue(attr.getVector3Value());
					}
					else if (attr.getType() == Attribute::Vector4)
					{
						uniform->setValue(attr.getVector4Value());
					}
				}
			}

			_cleanShaders.add(shader);
		}
	}

	void Material::bindMaterialTextures()
	{
		for (auto i = _textureAttributes.createIterator(); i; i++)
		{
			Attribute& attr = *i;

			uint32 unit = attr.getTextureValue()->bind(this);

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
	
		
	Material::Attribute* Material::getAlbedo()
	{
		return getAttribute("albedo");
	}
		
	Material::Attribute* Material::getBaseColor()
	{
		return getAttribute("albedo");
	}
	
	Material::Attribute* Material::getMetallic()
	{
		return getAttribute("metallic");
	}
		
	Material::Attribute* Material::getRoughness()
	{
		return getAttribute("roughness");
	}
		
	Material::Attribute* Material::getSpecular()
	{
		return getAttribute("specular");
	}
	
	Material::Attribute* Material::getHeight()
	{
		return getAttribute("height");
	}
				
	Material::Attribute* Material::getAmbientOcclusion()
	{
		return getAttribute("ambient_occlusion");
	}
		
	Material::Attribute* Material::getOpacity()
	{
		return getAttribute("opacity");
	}
		
	Material::Attribute* Material::getEmissive()
	{
		return getAttribute("emissive");
	}
	
	
	//Attribute
	
	Material::Attribute::Attribute() : _type(NUM_MATERIAL_ATTRIBUTE_TYPES), _name("default")
	{
	}
			
	Material::Attribute::Attribute(const std::string& name, Type type) : _name(name), _type(type)
	{
		if(_type == Number)
		{
			_number = 0.0f;
		}
		else if(_type == Texture)
		{
			_texture = nullptr;
		}
		else if(_type == Vector3)
		{
			_vector = Vector4f::zero();
		}
		else if(_type == Vector4)
		{
			_vector = Vector4f::zero();
			_vector.w = 1.0f;
		}
	}
			
	Material::Attribute::Attribute(const std::string& name, float value) : _name(name), _type(Number), _number(value)
	{
	}
			
	Material::Attribute::Attribute(const std::string& name, class Texture* value) : _name(name), _type(Texture), _texture(value), _unit(0), _dirtyBind(true)
	{
		_texture->reference();
	}
			
	Material::Attribute::Attribute(const std::string& name, Vector3f value) : _name(name), _type(Vector3), _vector(Vector4f(value.x, value.y, value.z, 1.0))
	{
	}
			
	Material::Attribute::Attribute(const std::string& name, Vector4f value) : _name(name), _type(Vector4), _vector(value)
	{
	}
			
	Material::Attribute::Attribute(const Attribute& attribute) : _name(attribute._name), _type(attribute._type)
	{
		if(_type == Number)
		{
			_number = attribute._number;
		}
		else if(_type == Texture)
		{
			_texture = attribute._texture;
			_texture->reference();
			_unit = attribute._unit;
			_dirtyBind = true;
		}
		else if(_type == Vector3 || _type == Vector4)
		{
			_vector = attribute._vector;
		}
	}
			
	Material::Attribute::~Attribute()
	{
		if (_type == Texture)
		{
			AE_RELEASE(_texture);
		}
	}
			
			
	void Material::Attribute::setType(Type type)
	{
		_type = type;
	}
			
	Material::Attribute::Type Material::Attribute::getType() const
	{
		return _type;
	}
	
	void Material::Attribute::setName(const std::string& name)
	{
		_name = name;
	}
			
	const std::string& Material::Attribute::getName() const
	{
		return _name;
	}
			
			
	void Material::Attribute::setValue(float value)
	{
		_type = Number;
		_number = value;
	}
			
	void Material::Attribute::setValue(class Texture* value)
	{
		_type = Texture;
		_texture = value;
		_dirtyBind = true;
		_texture->reference();
	}
			
	void Material::Attribute::setValue(Vector3f value)
	{
		_type = Vector3;
		_vector = Vector4f(value.x, value.y, value.z, 1.0f);
	}
			
	void Material::Attribute::setValue(Vector4f value)
	{
		_type = Vector4;
		_vector = value;
	}
			
			
	float Material::Attribute::getFloatValue() const
	{
		if(_type == Number)
		{
			return _number;
		}
		
		return 0.0f;
	}
			
	class Texture* Material::Attribute::getTextureValue() const
	{
		if (_type == Texture)
		{
			return _texture;
		}

		return nullptr;
	}
			
	Vector3f Material::Attribute::getVector3Value() const
	{
		if(_type == Vector3)
		{
			return Vector3f(_vector.x, _vector.y, _vector.z);
		}
		
		return Vector3f::zero();
	}
			
	Vector4f Material::Attribute::getVector4Value() const
	{
		if(_type == Vector4)
		{
			return _vector;
		}
		
		return Vector4f::zero();
	}
			
	uint32 Material::Attribute::getTextureUnit() const
	{
		if (_type == Texture)
		{
			return _unit;
		}

		return 0;
	}

	void Material::Attribute::setTextureUnit(uint32 unit)
	{
		if (_type == Texture)
		{
			_unit = unit;
		}
	}

	bool Material::Attribute::isTextureBindDirty() const
	{
		if (_type == Texture)
		{
			return _dirtyBind;
		}

		return false;
	}

	void Material::Attribute::setTextureBindDirty(bool dirty)
	{
		if (_type == Texture)
		{
			_dirtyBind = dirty;
		}
	}
			
	Material::Attribute& Material::Attribute::operator=(const Attribute& attr)
	{
		_type = attr._type;
		_name = attr._name;
		
		if(_type == Number)
		{
			_number = attr._number;
		}
		else if(_type == Texture)
		{
			_texture = attr._texture;
			_unit = attr._unit;
			_dirtyBind = true;
		}
		else if(_type == Vector3 || _type == Vector4)
		{
			_vector = attr._vector;
		}
		
		return *this;
	}

	class MaterialResource : public ResourceCreator<Material>
	{
	public:

		MaterialResource(const std::string& name, const std::string& type, const ResourceData& data)
			: ResourceCreator<Material>(name, type, data)
		{
			//setName(name);

			uint32 techniqueCount = 0;

			std::vector<ResourceDataPoint>::const_iterator iter;
			for (iter = data.getDataPoints().begin(); iter != data.getDataPoints().end(); iter++)
			{
				const ResourceDataPoint& dataPoint = *iter;

				if (!dataPoint.hasResourceData)
				{
					Material::Attribute attribute;
					attribute.setName(dataPoint.name);

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
						Texture* texture = ResourceManager::instance().loadResource<Texture>(dataPoint.stringData);
						if (texture)
						{
							attribute.setValue(texture);
						}
					}

					addAttribute(attribute);
				}
				else
				{
					if (dataPoint.name == "technique")
					{
						const ResourceData& dataPointResourceData = dataPoint.resourceData;

						std::string techniqueName = name + "_technique_" + std::to_string(techniqueCount++);

						Technique* technique = ResourceManager::instance().buildResource<Technique>(techniqueName, dataPoint.name, dataPointResourceData);
						technique->reference();
						if (dataPoint.getBoolAttribute("current"))
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
			}
		}
	};

	Resource::Type<MaterialResource> materialResource("material");
}