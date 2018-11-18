#ifndef MESH_COMPONENT_H_
#define MESH_COMPONENT_H_

#include "SceneDefines.h"

#include "GeometryComponent.h"

namespace Arcana
{

	class ARCANA_SCENE_API MeshComponent : public GeometryComponent
	{
	public:

		MeshComponent();

		virtual ~MeshComponent();
	};

}

#endif // !MESH_COMPONENT_H_

