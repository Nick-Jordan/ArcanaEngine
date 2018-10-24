#ifndef VERTEX_BUFFER_H_
#define VERTEX_BUFFER_H_

#include "GraphicsDefines.h"

#include "Array.h"
#include "VertexFormat.h"
#include "../Dependencies/include/opengl/include.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API VertexBuffer
	{
	public:
	
		struct AttributeBinding
		{
			bool enabled;
			GLuint index;
			int32 size;
			GLenum type;
			bool normalized;
			uint32 stride;
			void* pointer;
		};
		
		VertexBuffer(const VertexFormat& vertexFormat, uint32 vertexCount = 0, bool dynamic = false, void* vertexPointer = nullptr);
		
		~VertexBuffer();
		
		
		const VertexFormat& getVertexFormat() const;

		uint32 getVertexCount() const;
		
		bool isDynamic() const;
		
		bool isBound() const;
		
		void setVertexData(const void* vertexData, uint32 vertexStart = 0, uint32 vertexCount = 0);
		
		bool bind(); //first vertex as argument?
		
		void unbind();
		
		void bindVertexAttributes();
		
		void unbindVertexAttributes();
		
		void* map();
		
		bool unmap();

	private:
	
		void initialize(const VertexFormat& vertexFormat, void* vertexPointer);
		
		void finalize();
	
	private:
	
		GLuint _vbo;
		uint32 _vertexCount;
		bool _dynamic;
		bool _bound;
		const VertexFormat _format;
		
		Array<AttributeBinding> _attributeBindings;
	};
}

#endif // !VERTEX_BUFFER_H_