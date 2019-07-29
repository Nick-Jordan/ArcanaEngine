#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "GraphicsDefines.h"

#include "Array.h"
#include "GlobalObjectID.h"
#include "Texture.h"
#include "Technique.h"

#include <string>

namespace Arcana
{
	class ARCANA_GRAPHICS_API Material : public Object
	{
	public:
		
		Material();

		Material(const std::string& name);
		
		~Material();
	
	
		void addAttribute(const MaterialAttribute& attribute, uint32 techniqueIndex = 0);
		
		void addAttribute(const std::string& name, float value, uint32 techniqueIndex = 0);
		
		void addAttribute(const std::string& name, Texture* value, uint32 techniqueIndex = 0);

		void addAttribute(const std::string& name, Vector2f value, uint32 techniqueIndex = 0);

		void addAttribute(const std::string& name, Vector3f value, uint32 techniqueIndex = 0);
		
		void addAttribute(const std::string& name, Vector4f value, uint32 techniqueIndex = 0);

		void bindAttribute(const std::string& name, const MaterialFloatAttributeBinding& binding, uint32 techniqueIndex = 0);
		//bind texture attrib?
		void bindAttribute(const std::string& name, const MaterialVector2AttributeBinding& binding, uint32 techniqueIndex = 0);

		void bindAttribute(const std::string& name, const MaterialVector3AttributeBinding& binding, uint32 techniqueIndex = 0);

		void bindAttribute(const std::string& name, const MaterialVector4AttributeBinding& binding, uint32 techniqueIndex = 0);
		
		void removeAttribute(const std::string& name, uint32 techniqueIndex = 0);
		
		MaterialAttribute* getAttribute(const std::string& name, uint32 techniqueIndex = 0);
		
		
		void addTechnique(Technique* technique);

		virtual Technique* getTechnique(uint32 index);

		uint32 getTechniqueCount() const;

		Technique* getCurrentTechnique();

		void setCurrentTechnique(uint32 index);

		void setCurrentTechnique(Technique* technique);

		bool usesTexture(const Texture* texture);

		void passMaterialAttributes(Shader* shader, Technique* technique);

		void bindMaterialTextures(Technique* technique);

		const GlobalObjectID& getId() const;
		
			
		
		MaterialAttribute* getAlbedo(uint32 techniqueIndex = 0);  //vec3
		
		MaterialAttribute* getBaseColor(uint32 techniqueIndex = 0);  //vec3 (same as albedo)
		
		MaterialAttribute* getMetallic(uint32 techniqueIndex = 0);  //float
		
		MaterialAttribute* getRoughness(uint32 techniqueIndex = 0);  //float
		
		MaterialAttribute* getSpecular(uint32 techniqueIndex = 0);  //float
		
		MaterialAttribute* getHeight(uint32 techniqueIndex = 0);  //float
				
		MaterialAttribute* getAmbientOcclusion(uint32 techniqueIndex = 0);  //float
		
		MaterialAttribute* getOpacity(uint32 techniqueIndex = 0);  //float
		
		MaterialAttribute* getEmissive(uint32 techniqueIndex = 0);  //float
			
	private:
	
		Array<Shader*> _cleanShaders;

		Array<Technique*> _techniques;
		
		uint32 _currentTechnique;

		GlobalObjectID _id;
	};
}

#endif // !MATERIAL_H_