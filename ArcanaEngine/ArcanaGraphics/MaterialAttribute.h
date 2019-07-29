#ifndef MATERIAL_ATTRIBUTE_H_
#define MATERIAL_ATTRIBUTE_H_

#include "GraphicsDefines.h"

#include "Array.h"
#include "Vector3.h"
#include "Vector4.h"
#include <string>
#include "Texture.h"
#include "Types.h"

namespace Arcana
{
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

			NUM_MATERIAL_ATTRIBUTE_TYPES
		};

		MaterialAttribute();

		MaterialAttribute(const std::string& name, Type type);

		MaterialAttribute(const std::string& name, float value);

		MaterialAttribute(const std::string& name, class Texture* texture);

		MaterialAttribute(const std::string& name, Vector2f value);

		MaterialAttribute(const std::string& name, Vector3f value);

		MaterialAttribute(const std::string& name, Vector4f value);

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


		float getFloatValue() const;

		class Texture* getTextureValue() const;

		Vector2f getVector2Value() const;

		Vector3f getVector3Value() const;

		Vector4f getVector4Value() const;

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
		};
	};

	struct ARCANA_GRAPHICS_API MaterialAttributeContainer
	{
		Array<MaterialAttribute> textureAttributes;
		Array<MaterialAttribute> attributes;
	};
}

#endif // !MATERIAL_ATTRIBUTE_H_
