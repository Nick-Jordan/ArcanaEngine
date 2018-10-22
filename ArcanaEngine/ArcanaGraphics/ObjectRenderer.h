#ifndef OBJECT_RENDERER_H_
#define OBJECT_RENDERER_H_

#include "GraphicsDefines.h"

#include "MeshRenderContext.h"

namespace Arcana
{

	class ARCANA_GRAPHICS_API ObjectRenderer
	{
	public:
		
		ObjectRenderer();

		~ObjectRenderer();


		void render();// Camera& camera, RenderTarget* renderTarget);

		void queueMesh(MeshRenderContext& context);

	private:

		bool _updateMeshQueue;
		
		std::vector<MeshRenderContext> _queuedMeshes;
	};

}

#endif // !OBJECT_RENDERER_H_

