#ifndef VERTEX_FORMAT_H_
#define VERTEX_FORMAT_H_

#include "GraphicsDefines.h"

#include "Types.h"
#include <vector>

namespace Arcana
{
	class ARCANA_GRAPHICS_API VertexFormat
	{
	public:
	
		enum Semantic
		{
			Position,
			Normal,
			Color,
			Tangent,
			Binormal,
			BlendWeights,
			BlendIndices,
			TexCoord0,
			TexCoord1,
			TexCoord2,
			TexCoord3,
			TexCoord4,
			TexCoord5,
			TexCoord6,
			LightmapUVs
		};
		
		class ARCANA_GRAPHICS_API Attribute
		{
		public:
		
			Attribute();
		
			Attribute(Semantic type, uint32 size);
			
			Semantic getType() const;
			
			uint32 getSize() const;
			
			bool operator==(const Attribute& c) const;
			
			bool operator!=(const Attribute& c) const;
		
		private:
		
			Semantic _type;
			
			uint32 _size;
		};
		
		
		VertexFormat();
		
		VertexFormat(uint32 numAttributes, const Attribute* attributes, uint32 indexOffset = 0);
		
		~VertexFormat();
		
		const Attribute& getAttribute(uint32 index) const;
		
		void addAttribute(const Attribute& attribute);
		
		uint32 getNumAttributes() const;
		
		uint32 getVertexSize() const;

		uint32 getIndexOffset() const;
		
		
		bool operator == (const VertexFormat& f) const;

		bool operator != (const VertexFormat& f) const;
		
	private:
	
		//switch to array
		std::vector<Attribute> _attributes;
		uint32 _vertexSize;
		uint32 _indexOffset;
	};
}

#endif // !VERTEX_FORMAT_H_