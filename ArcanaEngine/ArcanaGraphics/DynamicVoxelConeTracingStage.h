/*#ifndef DYNAMIC_VOXEL_CONE_TRACING_STAGE_H_
#define DYNAMIC_VOXEL_CONE_TRACING_STAGE_H_

#include "RenderStage.h"

#include "Shader.h"
#include "Texture.h"
#include "Framebuffer.h"

namespace Arcana
{
	class DynamicVoxelConeTracingStage : public RenderStage
	{
	public:

		DynamicVoxelConeTracingStage();

		~DynamicVoxelConeTracingStage();

		virtual void initialize() override;

		virtual void finalize() override;

		virtual void render() override;

		Texture* getVoxelTexture() const;

	private:

		void renderObjects();

	private:

		Framebuffer* _framebuffer;
		Shader _voxelizationShader;
		Texture* _voxelTexture;
		Texture* _framebufferTexture;
		uint32 _voxelTextureSize;

		bool _voxelizationQueued;
		bool _automaticallyVoxelize;
		uint32 _voxelizationSparsity;
		uint32 _ticksSinceLastVoxelization;
	};
}

#endif*/ // !DYNAMIC_VOXEL_CONE_TRACING_STAGE_H_

