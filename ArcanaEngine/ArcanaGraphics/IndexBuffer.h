#ifndef INDEX_BUFFER_H_
#define INDEX_BUFFER_H_

#include "GraphicsDefines.h"

#include "opengl/include.h"
#include "Types.h"

namespace Arcana
{
	
	class ARCANA_GRAPHICS_API IndexBuffer
	{
	public:
	
		enum Format
		{
			Index8 = GL_UNSIGNED_BYTE,
			Index16 = GL_UNSIGNED_SHORT,
			Index32 = GL_UNSIGNED_INT
		};
		
		IndexBuffer(Format indexFormat, uint32 indexCount = 0, bool dynamic = false, void* indexPointer = nullptr);
		
		~IndexBuffer();
		
		
		Format getIndexFormat() const;

		uint32 getIndexCount() const;
		
		bool isDynamic() const;
		
		bool isBound() const;
		
		void setIndexData(const void* indexData, uint32 indexStart = 0, uint32 indexCount = 0);
		
		bool bind(); //first index as argument?
		
		void unbind();
		
		void* map();
		
		bool unmap();
	
	private:
	
		static int32 getFormatSize(Format format);
	
		void initialize(Format format, void* indexPointer);
		
		void finalize();
	
	private:
	
		GLuint _ibo;
		uint32 _indexCount;
		bool _dynamic;
		bool _bound;
		Format _format;
	};
	
}

#endif // !INDEX_BUFFER_H_