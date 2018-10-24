#include "Material.h"

#include "Types.h"

namespace Arcana
{
	Material::Material(const std::string& name) : _id(name), _currentTechnique(0)
	{
	}
		
	Material::~Material()
	{
	}
	
	
	void Material::addAttribute(const Attribute& attribute)
	{
		_attributes.add(attribute);
	}
		
	void Material::addAttribute(const std::string& name, float value)
	{
		_attributes.add(Attribute(name, value));
	}
		
	//void addAttribute(const std::string& name, texture);
		
	void Material::addAttribute(const std::string& name, Vector3f value)
	{
		_attributes.add(Attribute(name, value));
	}
		
	void Material::addAttribute(const std::string& name, Vector4f value)
	{
		_attributes.add(Attribute(name, value));
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

	void Material::addTechnique(const Technique& technique)
	{
		_techniques.add(technique);
	}

	Technique* Material::getTechnique(uint32 index)
	{
		if (index < _techniques.size())
		{
			return &_techniques[index];
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
			return &_techniques[_currentTechnique];
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

	void Material::setCurrentTechnique(const Technique& technique)
	{
		if (!_techniques.contains(technique))
		{
			_techniques.add(technique);
			_currentTechnique = _techniques.size() - 1;
		}
		else
		{
			_currentTechnique = _techniques.find(technique);
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
		
	Material::Attribute* Material::getIrradiance()
	{
		return getAttribute("irradiance");
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
			//
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
			
			//Material::Attribute::Attribute(const std::string& name, texture);
			
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
			//
		}
		else if(_type == Vector3 || _type == Vector4)
		{
			_vector = attribute._vector;
		}
	}
			
	Material::Attribute::~Attribute()
	{
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
			
	//void Material::Attribute::setValue(texture);
			
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
			
	//texture getValue() const;
			
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
			//
		}
		else if(_type == Vector3 || _type == Vector4)
		{
			_vector = attr._vector;
		}
		
		return *this;
	}
}