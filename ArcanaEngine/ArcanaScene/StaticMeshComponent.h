#ifndef STATIC_MESH_COMPONENT_H_
#define STATIC_MESH_COMPONENT_H_

#include "SceneDefines.h"

#include "MeshComponent.h"

namespace Arcana
{

	class ARCANA_SCENE_API StaticMeshComponent : public MeshComponent
	{
	public:

		StaticMeshComponent();

		virtual ~StaticMeshComponent();
	};

}

#endif // !STATIC_MESH_COMPONENT_H_

