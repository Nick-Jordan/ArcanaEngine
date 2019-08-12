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
		else if (_type == Vector2)
		{
			_vector = Vector4f::zero();
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
	
	MaterialAttribute::MaterialAttribute(const std::string& name, Vector2f value) : _name(name), _type(Vector2), _vector(Vector4f(value.x, value.y, 0.0f, 1.0))
	{
	}

	MaterialAttribute::MaterialAttribute(const std::string& name, Vector3f value) : _name(name), _type(Vector3), _vector(Vector4f(value.x, value.y, value.z, 1.0))
	{
	}

	MaterialAttribute::MaterialAttribute(const std::string& name, Vector4f value) : _name(name), _type(Vector4), _vector(value)
	{
	}

	MaterialAttribute::MaterialAttribute(const std::string& name, const MaterialFloatAttributeBinding& binding) : _name(name), _type(FloatBinding), _floatBinding(binding)
	{
	}

	MaterialAttribute::MaterialAttribute(const std::string& name, const MaterialVector2AttributeBinding& binding) : _name(name), _type(Vector2Binding), _vec2Binding(binding)
	{
	}

	MaterialAttribute::MaterialAttribute(const std::string& name, const MaterialVector3AttributeBinding& binding) : _name(name), _type(Vector3Binding), _vec3Binding(binding)
	{
	}

	MaterialAttribute::MaterialAttribute(const std::string& name, const MaterialVector4AttributeBinding& binding) : _name(name), _type(Vector4Binding), _vec4Binding(binding)
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
		else if (_type == Vector2 || _type == Vector3 || _type == Vector4)
		{
			_vector = attribute._vector;
		}
		else if (_type == FloatBinding)
		{
			_floatBinding = attribute._floatBinding;
		}
		else if (_type == Vector2Binding)
		{
			_vec2Binding = attribute._vec2Binding;
		}
		else if (_type == Vector3Binding)
		{
			_vec3Binding = attribute._vec3Binding;
		}
		else if (_type == Vector4Binding)
		{
			_vec4Binding = attribute._vec4Binding;
		}
	}

	MaterialAttribute::~MaterialAttribute()
	{
		if (_type == Texture)
		{
			AE_RELEASE(_texture);
		}
		else if (_type == Vector2Binding)
		{
			_vec2Binding.~BaseCallback();
		}
		else if (_type == Vector3Binding)
		{
			_vec3Binding.~BaseCallback();
		}
		else if (_type == Vector4Binding)
		{
			_vec4Binding.~BaseCallback();
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
	
	void MaterialAttribute::setValue(Vector2f value)
	{
		_type = Vector2;
		_vector = Vector4f(value.x, value.y, 0.0f, 1.0f);
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

	void MaterialAttribute::bindValue(const MaterialFloatAttributeBinding& binding)
	{
		_type = FloatBinding;
		_floatBinding = binding;
	}

	void MaterialAttribute::bindValue(const MaterialVector2AttributeBinding& binding)
	{
		_type = Vector2Binding;
		_vec2Binding = binding;
	}

	void MaterialAttribute::bindValue(const MaterialVector3AttributeBinding& binding)
	{
		_type = Vector3Binding;
		_vec3Binding = binding;
	}

	void MaterialAttribute::bindValue(const MaterialVector4AttributeBinding& binding)
	{
		_type = Vector4Binding;
		_vec4Binding = binding;
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

	Vector2f MaterialAttribute::getVector2Value() const
	{
		if (_type == Vector2)
		{
			return Vector2f(_vector.x, _vector.y);
		}

		return Vector2f::zero();
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

	float MaterialAttribute::getFloatBinding()
	{
		if (_type == Number)
		{
			return _floatBinding.executeIfBound();
		}

		return 0.0f;
	}

	Vector2f MaterialAttribute::getVector2Binding()
	{
		if (_type == Vector2Binding)
		{
			return _vec2Binding.executeIfBound();
		}

		return Vector2f::zero();
	}

	Vector3f MaterialAttribute::getVector3Binding()
	{
		if (_type == Vector3Binding)
		{
			return _vec3Binding.executeIfBound();
		}

		return Vector3f::zero();
	}

	Vector4f MaterialAttribute::getVector4Binding()
	{
		if (_type == Vector4Binding)
		{
			return _vec4Binding.executeIfBound();
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
		else if (_type == Vector2 || _type == Vector3 || _type == Vector4)
		{
			_vector = attr._vector;
		}
		else if (_type == FloatBinding)
		{
			_floatBinding = attr._floatBinding;
		}
		else if (_type == Vector2Binding)
		{
			_vec2Binding = attr._vec2Binding;
		}
		else if (_type == Vector3Binding)
		{
			_vec3Binding = attr._vec3Binding;

		}
		else if (_type == Vector4Binding)
		{
			_vec4Binding = attr._vec4Binding;
		}

		return *this;
	}
}