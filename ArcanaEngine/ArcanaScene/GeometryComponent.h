#ifndef GEOMETRY_COMPONENT_H_
#define GEOMETRY_COMPONENT_H_

#include "SceneDefines.h"

#include "SceneComponent.h"

#include "Material.h"
#include "RenderState.h"
#include "ObjectRenderer.h"

namespace Arcana
{
	class ARCANA_SCENE_API GeometryComponent : public SceneComponent
	{
	public:

		GeometryComponent();

		virtual ~GeometryComponent();

		bool hasRenderObject() const;

		void render(ObjectRenderer& renderer, Matrix4f view, Matrix4f projection);



		Material* test;
		Mesh* mesh;
		RenderState testRenderState;
	};

}

#endif // !GEOMETRY_COMPONENT_H_

