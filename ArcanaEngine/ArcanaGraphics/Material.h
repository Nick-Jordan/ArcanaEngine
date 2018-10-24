#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "GraphicsDefines.h"

#include "Array.h"
#include "Vector3.h"
#include "Vector4.h"
#include "GlobalObjectID.h"
#include "Technique.h"
#include <string>

namespace Arcana
{
	
	class ARCANA_GRAPHICS_API Material
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
			
			//Attribute(const std::string& name, texture);
			
			Attribute(const std::string& name, Vector3f value);
			
			Attribute(const std::string& name, Vector4f value);
			
			Attribute(const Attribute& attribute);
			
			~Attribute();
			
			
			void setType(Type type);
			
			Type getType() const;
			
			void setName(const std::string& name);
			
			const std::string& getName() const;
			
			
			void setValue(float value);
			
			//void setValue(texture);
			
			void setValue(Vector3f value);
			
			void setValue(Vector4f value);
			
			
			float getFloatValue() const;
			
			//texture getValue() const;
			
			Vector3f getVector3Value() const;
			
			Vector4f getVector4Value() const;


			Attribute& operator=(const Attribute& attr);
		
		private:
		
			Type _type;
			
			std::string _name;
			
			union
			{
				float _number;
				//texture
				Vector4f _vector;
			};
		};
		
		Material(const std::string& name);
		
		~Material();
	
	
		void addAttribute(const Attribute& attribute);
		
		void addAttribute(const std::string& name, float value);
		
		//void addAttribute(const std::string& name, texture);
		
		void addAttribute(const std::string& name, Vector3f value);
		
		void addAttribute(const std::string& name, Vector4f value);
		
		void removeAttribute(const std::string& name);
		
		Attribute* getAttribute(const std::string& name);
		
		
		void addTechnique(const Technique& technique);

		Technique* getTechnique(uint32 index);

		uint32 getTechniqueCount() const;

		Technique* getCurrentTechnique();

		void setCurrentTechnique(uint32 index);

		void setCurrentTechnique(const Technique& technique);


		const GlobalObjectID& getId() const;
		
			
		
		Attribute* getAlbedo();
		
		Attribute* getBaseColor();
		
		Attribute* getMetallic();
		
		Attribute* getRoughness();
		
		Attribute* getSpecular();
		
		Attribute* getHeight();
				
		Attribute* getAmbientOcclusion();
		
		Attribute* getOpacity();
		
		Attribute* getEmissive();
		
		Attribute* getIrradiance();
	
	private:
	
		Array<Attribute> _attributes;

		Array<Technique> _techniques;
		
		uint32 _currentTechnique;

		GlobalObjectID _id;
	};
}

#endif // !MATERIAL_H_