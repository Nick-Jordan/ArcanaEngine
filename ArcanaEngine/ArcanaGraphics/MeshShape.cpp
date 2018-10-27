#include "MeshShape.h"

namespace Arcana
{

	MeshShape::MeshShape(Mesh* mesh) : Shape()
	{
		setMesh(mesh);
	}


	MeshShape::~MeshShape()
	{
	}

}
