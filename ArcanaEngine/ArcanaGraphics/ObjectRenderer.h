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

		void initialize();

		void render();// Camera& camera, RenderTarget* renderTarget);

		void queueMesh(const MeshRenderContext& context);

		//test
		int32 numQueued;

	private:
		
		std::vector<MeshRenderContext> _queuedMeshes;
	};

}

#endif // !OBJECT_RENDERER_H_

