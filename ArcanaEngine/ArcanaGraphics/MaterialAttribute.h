#ifndef MATERIAL_ATTRIBUTE_H_
#define MATERIAL_ATTRIBUTE_H_

#include "GraphicsDefines.h"

#include "Array.h"
#include "Vector3.h"
#include "Vector4.h"
#include <string>
#include "Texture.h"
#include "Types.h"
#include "Callback.h"

namespace Arcana
{
	REGISTER_CALLBACK_RETURN_TYPE(MaterialFloatAttributeBinding, float);
	REGISTER_CALLBACK_RETURN_TYPE(MaterialVector2AttributeBinding, Vector2f);
	REGISTER_CALLBACK_RETURN_TYPE(MaterialVector3AttributeBinding, Vector3f);
	REGISTER_CALLBACK_RETURN_TYPE(MaterialVector4AttributeBinding, Vector4f);

	class ARCANA_GRAPHICS_API MaterialAttribute
	{
	public:

		enum Type
		{
			Number,
			Texture,
			Vector2,
			Vector3,
			Vector4,
			FloatBinding,
			Vector2Binding,
			Vector3Binding,
			Vector4Binding,

			NUM_MATERIAL_ATTRIBUTE_TYPES
		};

		MaterialAttribute();

		MaterialAttribute(const std::string& name, Type type);

		MaterialAttribute(const std::string& name, float value);
		MaterialAttribute(const std::string& name, class Texture* texture);
		MaterialAttribute(const std::string& name, Vector2f value);
		MaterialAttribute(const std::string& name, Vector3f value);
		MaterialAttribute(const std::string& name, Vector4f value);

		MaterialAttribute(const std::string& name, const MaterialFloatAttributeBinding& binding);
		MaterialAttribute(const std::string& name, const MaterialVector2AttributeBinding& binding);
		MaterialAttribute(const std::string& name, const MaterialVector3AttributeBinding& binding);
		MaterialAttribute(const std::string& name, const MaterialVector4AttributeBinding& binding);

		MaterialAttribute(const MaterialAttribute& attribute);

		~MaterialAttribute();


		void setType(Type type);

		Type getType() const;

		void setName(const std::string& name);

		const std::string& getName() const;


		void setValue(float value);

		void setValue(class Texture* texture);

		void setValue(Vector2f value);

		void setValue(Vector3f value);

		void setValue(Vector4f value);

		void bindValue(const MaterialFloatAttributeBinding& binding);

		void bindValue(const MaterialVector2AttributeBinding& binding);

		void bindValue(const MaterialVector3AttributeBinding& binding);

		void bindValue(const MaterialVector4AttributeBinding& binding);


		float getFloatValue() const;

		class Texture* getTextureValue() const;

		Vector2f getVector2Value() const;

		Vector3f getVector3Value() const;

		Vector4f getVector4Value() const;

		float getFloatBinding();

		Vector2f getVector2Binding();

		Vector3f getVector3Binding();

		Vector4f getVector4Binding();

		int32 getTextureUnit() const;

		void setTextureUnit(int32 unit);

		bool isTextureBindDirty() const;

		void setTextureBindDirty(bool dirty);

		MaterialAttribute& operator=(const MaterialAttribute& attr);

	private:

		Type _type;

		std::string _name;

		int32 _unit;
		bool _dirtyBind;

		union
		{
			float _number;
			class Texture* _texture;
			Vector4f _vector;
			MaterialFloatAttributeBinding _floatBinding;
			MaterialVector2AttributeBinding _vec2Binding;
			MaterialVector3AttributeBinding _vec3Binding;
			MaterialVector4AttributeBinding _vec4Binding;
		};
	};

	struct ARCANA_GRAPHICS_API MaterialAttributeContainer
	{
		Array<MaterialAttribute> textureAttributes;
		Array<MaterialAttribute> attributes;
	};
}

#endif // !MATERIAL_ATTRIBUTE_H_
