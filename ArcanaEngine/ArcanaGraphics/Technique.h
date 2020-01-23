#ifndef TECHNIQUE_H_
#define TECHNIQUE_H_

#include "GraphicsDefines.h"

#include "Types.h"
#include "Shader.h"
#include "MaterialAttribute.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API Technique : public Object
	{
		friend class Material;

	public:
		
		Technique();

		Technique(uint32 numPasses);

		Technique(const Shader& shader);

		Technique(const Technique& technique);

		~Technique();

		uint32 getPassCount() const;

		Shader* getPass(uint32 index);

		void setPass(uint32 index, const Shader& pass);

		void addAttribute(const MaterialAttribute& attribute);

		void addAttribute(const std::string& name, float value);

		void addAttribute(const std::string& name, Texture* value);

		void addAttribute(const std::string& name, Vector2f value);

		void addAttribute(const std::string& name, Vector3f value);

		void addAttribute(const std::string& name, Vector4f value);

		void addAttribute(const std::string& name, LinearColor value, bool useTransparency = true);

		void addAttribute(const std::string& name, Color value, bool useTransparency = true);

		void bindAttribute(const std::string& name, const MaterialFloatAttributeBinding& binding);

		void bindAttribute(const std::string& name, const MaterialVector2AttributeBinding& binding);

		void bindAttribute(const std::string& name, const MaterialVector3AttributeBinding& binding);

		void bindAttribute(const std::string& name, const MaterialVector4AttributeBinding& binding);

		void removeAttribute(const std::string& name);

		MaterialAttribute* getAttribute(const std::string& name);

		bool operator == (const Technique& t) const;

		bool operator != (const Technique& t) const;

		Technique& operator=(const Technique& technique);

	protected:

		Array<Shader> _passes;

		uint32 _numPasses;

		MaterialAttributeContainer _attributes;
	};

}

#endif // !TECHNIQUE_H_

