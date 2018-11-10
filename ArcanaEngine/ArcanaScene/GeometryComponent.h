#ifndef GEOMETRY_COMPONENT_H_
#define GEOMETRY_COMPONENT_H_

#include "SceneDefines.h"

#include "SceneComponent.h"

#include "Material.h"
#include "Texture.h"
#include "RenderState.h"
#include "ObjectRenderer.h"

//test texture cache
#include "TextureCache.h"

namespace Arcana
{
	class ARCANA_SCENE_API GeometryComponent : public SceneComponent
	{
	public:

		GeometryComponent();

		virtual ~GeometryComponent();

		bool hasRenderObject() const;

		void render(ObjectRenderer& renderer, Matrix4f view, Matrix4f projection);



		//test texture cache
		static TextureCache CachedTextures;

		Material* test;
		Mesh* mesh;
		Texture* testTexture;
		RenderState testRenderState;
	};

}

#endif // !GEOMETRY_COMPONENT_H_

