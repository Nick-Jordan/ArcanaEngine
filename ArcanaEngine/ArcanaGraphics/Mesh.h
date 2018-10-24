#ifndef MESH_H_
#define MESH_H_

#include "GraphicsDefines.h"

#include "Types.h"
#include "VertexBuffer.h"

namespace Arcana
{
	
	//MeshSegment
	//MeshComponent
	//MeshSection
	//MeshIndexComponent

	class ARCANA_GRAPHICS_API Mesh
	{
	public:

		enum Primitive
		{
			Triangles = GL_TRIANGLES
		};
		
		Mesh(const VertexFormat& vertexFormat, Primitive primitive);
		
		virtual ~Mesh();
		
		const VertexFormat& getVertexFormat() const;
		
		void setPrimitive(Primitive primitive);
		
		Primitive getPrimitive() const;
		
		uint32 getVertexSize() const;
		
		void setVertexBuffer(VertexBuffer& vertexBuffer);
		
		VertexBuffer* getVertexBuffer();
		
		int32 getNumVertices() const;
		
		bool isDynamic() const;
		
		uint32 getNumIndexComponents() const;

		//void addIndexComponent();
		
		//MeshIndexComponent* getIndexComponent(uint32 index);
			
	private:

		VertexFormat _vertexFormat;
		Primitive _primitive;
		VertexBuffer* _vertexBuffer;
	};

}

#endif // !MESH_H_
