#ifndef RENDER_MESH_ACTOR_H_
#define RENDER_MESH_ACTOR_H_

#include "SceneDefines.h"

#include "Actor.h"

namespace Arcana
{

	class ARCANA_SCENE_API RenderMeshActor : public Actor
	{
	public:

		RenderMeshActor(Mesh* mesh);
		virtual ~RenderMeshActor();
	};

}

#endif // !RENDER_MESH_ACTOR_H_

