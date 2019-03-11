#include "MaterialAttribute.h"

namespace Arcana
{
	MaterialAttribute::MaterialAttribute() : _type(NUM_MATERIAL_ATTRIBUTE_TYPES), _name("default")
	{
	}

	MaterialAttribute::MaterialAttribute(const std::string& name, Type type) : _name(name), _type(type)
	{
		if (_type == Number)
		{
			_number = 0.0f;
		}
		else if (_type == Texture)
		{
			_texture = nullptr;
		}
		else if (_type == Vector3)
		{
			_vector = Vector4f::zero();
		}
		else if (_type == Vector4)
		{
			_vector = Vector4f::zero();
			_vector.w = 1.0f;
		}
	}

	MaterialAttribute::MaterialAttribute(const std::string& name, float value) : _name(name), _type(Number), _number(value)
	{
	}

	MaterialAttribute::MaterialAttribute(const std::string& name, class Texture* value) : _name(name), _type(Texture), _texture(value), _unit(0), _dirtyBind(true)
	{
		_texture->reference();
	}

	MaterialAttribute::MaterialAttribute(const std::string& name, Vector3f value) : _name(name), _type(Vector3), _vector(Vector4f(value.x, value.y, value.z, 1.0))
	{
	}

	MaterialAttribute::MaterialAttribute(const std::string& name, Vector4f value) : _name(name), _type(Vector4), _vector(value)
	{
	}

	MaterialAttribute::MaterialAttribute(const MaterialAttribute& attribute) : _name(attribute._name), _type(attribute._type)
	{
		if (_type == Number)
		{
			_number = attribute._number;
		}
		else if (_type == Texture)
		{
			_texture = attribute._texture;
			_texture->reference();
			_unit = attribute._unit;
			_dirtyBind = true;
		}
		else if (_type == Vector3 || _type == Vector4)
		{
			_vector = attribute._vector;
		}
	}

	MaterialAttribute::~MaterialAttribute()
	{
		if (_type == Texture)
		{
			AE_RELEASE(_texture);
		}
	}


	void MaterialAttribute::setType(Type type)
	{
		_type = type;
	}

	MaterialAttribute::Type MaterialAttribute::getType() const
	{
		return _type;
	}

	void MaterialAttribute::setName(const std::string& name)
	{
		_name = name;
	}

	const std::string& MaterialAttribute::getName() const
	{
		return _name;
	}


	void MaterialAttribute::setValue(float value)
	{
		_type = Number;
		_number = value;
	}

	void MaterialAttribute::setValue(class Texture* value)
	{
		_type = Texture;
		_texture = value;
		_dirtyBind = true;
		_texture->reference();
	}

	void MaterialAttribute::setValue(Vector3f value)
	{
		_type = Vector3;
		_vector = Vector4f(value.x, value.y, value.z, 1.0f);
	}

	void MaterialAttribute::setValue(Vector4f value)
	{
		_type = Vector4;
		_vector = value;
	}


	float MaterialAttribute::getFloatValue() const
	{
		if (_type == Number)
		{
			return _number;
		}

		return 0.0f;
	}

	class Texture* MaterialAttribute::getTextureValue() const
	{
		if (_type == Texture)
		{
			return _texture;
		}

		return nullptr;
	}

	Vector3f MaterialAttribute::getVector3Value() const
	{
		if (_type == Vector3)
		{
			return Vector3f(_vector.x, _vector.y, _vector.z);
		}

		return Vector3f::zero();
	}

	Vector4f MaterialAttribute::getVector4Value() const
	{
		if (_type == Vector4)
		{
			return _vector;
		}

		return Vector4f::zero();
	}

	int32 MaterialAttribute::getTextureUnit() const
	{
		if (_type == Texture)
		{
			return _unit;
		}

		return 0;
	}

	void MaterialAttribute::setTextureUnit(int32 unit)
	{
		if (_type == Texture)
		{
			_unit = unit;
		}
	}

	bool MaterialAttribute::isTextureBindDirty() const
	{
		if (_type == Texture)
		{
			return _dirtyBind;
		}

		return false;
	}

	void MaterialAttribute::setTextureBindDirty(bool dirty)
	{
		if (_type == Texture)
		{
			_dirtyBind = dirty;
		}
	}

	MaterialAttribute& MaterialAttribute::operator=(const MaterialAttribute& attr)
	{
		_type = attr._type;
		_name = attr._name;

		if (_type == Number)
		{
			_number = attr._number;
		}
		else if (_type == Texture)
		{
			_texture = attr._texture;
			_unit = attr._unit;
			_dirtyBind = true;
		}
		else if (_type == Vector3 || _type == Vector4)
		{
			_vector = attr._vector;
		}

		return *this;
	}
}