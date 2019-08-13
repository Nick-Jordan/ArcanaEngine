#include "RenderStage.h"

namespace Arcana
{

	RenderStage::RenderStage(std::string identifier) : _identifier(identifier)
	{
	}


	RenderStage::~RenderStage()
	{
	}

	void RenderStage::addMesh(const MeshRenderContext& meshRenderContext)
	{
		Meshes.add(meshRenderContext);
	}

	void RenderStage::addLight(const RenderLight& light)
	{
		Lights.add(light);
	}

	void RenderStage::clearMeshes()
	{
		Meshes.clear();
	}

	void RenderStage::clearLights()
	{
		Lights.clear();
	}

	const GlobalObjectID& RenderStage::getId() const
	{
		return _identifier;
	}
}
