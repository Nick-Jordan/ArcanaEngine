#ifndef INSTANCE_BUFFER_H_
#define INSTANCE_BUFFER_H_

#include "GraphicsDefines.h"

#include "Array.h"
#include "VertexBuffer.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API InstanceBuffer
	{
	public:

		struct ARCANA_GRAPHICS_API AttributeBinding : public VertexBuffer::AttributeBinding
		{
			uint32 divisor;
		};

		InstanceBuffer(const VertexFormat& format, uint32 instanceCount = 0, bool dynamic = false, void* dataPointer = nullptr);

		~InstanceBuffer();


		const VertexFormat& getFormat() const;

		uint32 getInstanceCount() const;

		bool isDynamic() const;

		bool isBound() const;

		void setInstanceData(const void* dataPointer, uint32 start = 0, uint32 count = 0);

		bool bind(); //first vertex as argument?

		void unbind();

		void bindAttributeDivisors();

		void unbindAttributeDivisors();

		void* map();

		bool unmap();

	private:

		void initialize(const VertexFormat& format, void* dataPointer);

		void finalize();

	private:

		GLuint _vbo;
		uint32 _instanceCount;
		bool _dynamic;
		bool _bound;
		const VertexFormat _format;

		Array<AttributeBinding> _attributeBindings;
	};
}

#endif // !INSTANCE_BUFFER_H_