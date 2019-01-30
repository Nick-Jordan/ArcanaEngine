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
		Meshes.empty();
	}

	void RenderStage::clearLights()
	{
		Lights.empty();
	}

	const GlobalObjectID& RenderStage::getId() const
	{
		return _identifier;
	}
}
