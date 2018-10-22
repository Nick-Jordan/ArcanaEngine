#include "ObjectRenderer.h"

namespace Arcana
{
	ObjectRenderer::ObjectRenderer() : _updateMeshQueue(false)
	{

	}

	ObjectRenderer::~ObjectRenderer()
	{

	}


	void ObjectRenderer::render()// Camera& camera, RenderTarget* renderTarget)
	{
		if (_updateMeshQueue)
		{
			std::sort(_queuedMeshes.begin(), _queuedMeshes.end(), 
				[](const MeshRenderContext& a, const MeshRenderContext& b) 
					{ 
						return a.material->getId().getId() > b.material->getId().getId(); 
					}
			);
			_updateMeshQueue = false;
		}
	}

	void ObjectRenderer::queueMesh(MeshRenderContext& context)
	{
		if (context.isValid())
		{
			_queuedMeshes.push_back(context);
		}

		_updateMeshQueue = true;
	}
}