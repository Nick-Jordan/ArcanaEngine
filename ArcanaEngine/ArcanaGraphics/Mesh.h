#ifndef MESH_H_
#define MESH_H_

#include "GraphicsDefines.h"

#include "InstanceBuffer.h"
#include "Object.h"

namespace Arcana
{
	
	//MeshSegment
	//MeshComponent
	//MeshSection
	class ARCANA_GRAPHICS_API MeshIndexComponent;

	class ARCANA_GRAPHICS_API Mesh : public Object
	{
	public:

		class ARCANA_GRAPHICS_API InstanceProperties
		{	
			friend class Mesh;

		public:
		
			InstanceProperties();

			bool isInstanced() const;

			int32 getNumInstances() const;

		public:

			bool _instanced;
			int32 _numInstances;
		};

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

		MeshIndexComponent* addIndexComponent(Mesh::Primitive primitive);

		void addIndexComponent(MeshIndexComponent* component);
		
		MeshIndexComponent* getIndexComponent(uint32 index);

		InstanceBuffer* setInstanceBuffer(const VertexFormat& format, uint32 instanceCount = 0, bool dynamic = false, void* dataPointer = nullptr);

		InstanceBuffer* getInstanceBuffer();

		InstanceProperties& getInstanceProperties();
			
	private:

		VertexFormat _vertexFormat;
		Primitive _primitive;
		VertexBuffer* _vertexBuffer;

		InstanceBuffer* _instanceBuffer;
		InstanceProperties _instanceProperties;

		Array<MeshIndexComponent*> _indexComponents;
	};

}

#endif // !MESH_H_
