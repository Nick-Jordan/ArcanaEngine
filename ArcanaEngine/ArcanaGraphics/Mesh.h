#ifndef MESH_H_
#define MESH_H_

#include "GraphicsDefines.h"

#include "VertexBuffer.h"
#include "Object.h"

namespace Arcana
{
	
	//MeshSegment
	//MeshComponent
	//MeshSection
	//MeshIndexComponent

	class ARCANA_GRAPHICS_API Mesh : public Object
	{
	public:

		enum Primitive
		{
			Points = GL_POINTS,
			Lines = GL_LINES,
			LineStrip = GL_LINE_STRIP,
			LineLoop = GL_LINE_LOOP,
			Polygon = GL_POLYGON,
			Quads = GL_QUADS,
			QuadStrip = GL_QUAD_STRIP,
			Triangles = GL_TRIANGLES,
			TriangleStrip = GL_TRIANGLE_STRIP,
			TriangleFan = GL_TRIANGLE_FAN
		};
		
		Mesh(const VertexFormat& vertexFormat, Primitive primitive);
		
		virtual ~Mesh();
		
		const VertexFormat& getVertexFormat() const;
		
		void setPrimitive(Primitive primitive);
		
		Primitive getPrimitive() const;
		
		uint32 getVertexSize() const;
		
		VertexBuffer* setVertexBuffer(const VertexFormat& vertexFormat, uint32 vertexCount = 0, bool dynamic = false, void* vertexPointer = nullptr);
		
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
