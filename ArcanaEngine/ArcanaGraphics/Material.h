#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "GraphicsDefines.h"

#include "Array.h"
#include "Vector3.h"
#include "Vector4.h"
#include "GlobalObjectID.h"
#include "Technique.h"
#include "Texture.h"
#include <string>

namespace Arcana
{
	
	class ARCANA_GRAPHICS_API Material : public Object
	{
	public:
	
		class ARCANA_GRAPHICS_API Attribute
		{
		public:
		
			enum Type
			{
				Number,
				Texture,
				Vector3,
				Vector4,
				
				NUM_MATERIAL_ATTRIBUTE_TYPES
			};
			
			Attribute();
			
			Attribute(const std::string& name, Type type);
			
			Attribute(const std::string& name, float value);
			
			Attribute(const std::string& name, class Texture* texture);
			
			Attribute(const std::string& name, Vector3f value);
			
			Attribute(const std::string& name, Vector4f value);
			
			Attribute(const Attribute& attribute);
			
			~Attribute();
			
			
			void setType(Type type);
			
			Type getType() const;
			
			void setName(const std::string& name);
			
			const std::string& getName() const;
			
			
			void setValue(float value);
			
			void setValue(class Texture* texture);
			
			void setValue(Vector3f value);
			
			void setValue(Vector4f value);
			
			
			float getFloatValue() const;
			
			class Texture* getTextureValue() const;
			
			Vector3f getVector3Value() const;
			
			Vector4f getVector4Value() const;

			int32 getTextureUnit() const;

			void setTextureUnit(int32 unit);

			bool isTextureBindDirty() const;

			void setTextureBindDirty(bool dirty);

			Attribute& operator=(const Attribute& attr);
		
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

		struct ARCANA_GRAPHICS_API AttributeContainer
		{
			Array<Attribute> textureAttributes;
			Array<Attribute> attributes;
		};
		
		Material();

		Material(const std::string& name);
		
		~Material();
	
	
		void addAttribute(const Attribute& attribute, uint32 techniqueIndex = 0);
		
		void addAttribute(const std::string& name, float value, uint32 techniqueIndex = 0);
		
		void addAttribute(const std::string& name, Texture* value, uint32 techniqueIndex = 0);
		
		void addAttribute(const std::string& name, Vector3f value, uint32 techniqueIndex = 0);
		
		void addAttribute(const std::string& name, Vector4f value, uint32 techniqueIndex = 0);
		
		void removeAttribute(const std::string& name, uint32 techniqueIndex = 0);
		
		Attribute* getAttribute(const std::string& name, uint32 techniqueIndex = 0);
		
		
		void addTechnique(Technique* technique);

		Technique* getTechnique(uint32 index);

		uint32 getTechniqueCount() const;

		Technique* getCurrentTechnique();

		void setCurrentTechnique(uint32 index);

		void setCurrentTechnique(Technique* technique);

		bool usesTexture(const Texture* texture);

		void passMaterialAttributes(Shader* shader, Technique* technique);

		void bindMaterialTextures(Technique* technique);

		const GlobalObjectID& getId() const;
		
			
		
		Attribute* getAlbedo(uint32 techniqueIndex = 0);  //vec3
		
		Attribute* getBaseColor(uint32 techniqueIndex = 0);  //vec3 (same as albedo)
		
		Attribute* getMetallic(uint32 techniqueIndex = 0);  //float
		
		Attribute* getRoughness(uint32 techniqueIndex = 0);  //float
		
		Attribute* getSpecular(uint32 techniqueIndex = 0);  //float
		
		Attribute* getHeight(uint32 techniqueIndex = 0);  //float
				
		Attribute* getAmbientOcclusion(uint32 techniqueIndex = 0);  //float
		
		Attribute* getOpacity(uint32 techniqueIndex = 0);  //float
		
		Attribute* getEmissive(uint32 techniqueIndex = 0);  //float
			
	private:
	
		Array<Shader*> _cleanShaders;

		std::map<Technique*, AttributeContainer> _attributes;

		Array<Technique*> _techniques;
		
		uint32 _currentTechnique;

		GlobalObjectID _id;
	};
}

#endif // !MATERIAL_H_