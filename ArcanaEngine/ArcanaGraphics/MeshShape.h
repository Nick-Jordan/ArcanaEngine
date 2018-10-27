#ifndef MESH_SHAPE_H_
#define MESH_SHAPE_H_

#include "GraphicsDefines.h"

#include "Shape.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API MeshShape : public Shape
	{
	public:

		MeshShape(Mesh* mesh);

		virtual ~MeshShape();
	};

}

#endif // !MESH_SHAPE_H_

