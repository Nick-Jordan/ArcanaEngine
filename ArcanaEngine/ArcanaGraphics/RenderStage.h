#ifndef RENDER_STAGE_H_
#define RENDER_STAGE_H_

#include "GraphicsDefines.h"

#include "GlobalObjectID.h"
#include "MeshRenderContext.h"
#include "RenderLight.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API RenderStage
	{
	public:

		RenderStage(std::string identifier);

		~RenderStage();

		virtual void initialize() = 0;

		virtual void finalize() = 0;

		virtual void render() = 0;

		void addMesh(const MeshRenderContext& meshRenderContext);

		void addLight(const RenderLight& light);

		void clearMeshes();

		void clearLights();

		const GlobalObjectID& getId() const;

	protected:

		Array<MeshRenderContext> Meshes;

		Array<RenderLight> Lights;

	private:

		GlobalObjectID _identifier;
	};
}

#endif // !RENDER_STAGE_H_