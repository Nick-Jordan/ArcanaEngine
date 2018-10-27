#include "RenderMeshActor.h"

#include "MeshShape.h"

namespace Arcana
{

	RenderMeshActor::RenderMeshActor(Mesh* mesh)
	{
		Shape* shape = new MeshShape(mesh);
		setShape(shape);
	}


	RenderMeshActor::~RenderMeshActor()
	{
	}

}