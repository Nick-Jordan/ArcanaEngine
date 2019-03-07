#ifndef DEFERRED_LIGHTING_STAGE_H_
#define DEFERRED_LIGHTING_STAGE_H_

#include "GraphicsDefines.h"

#include "RenderStage.h"

#include "Texture.h"
#include "DynamicDirectionalShadowStage.h"
#include "DynamicPointShadowStage.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API DeferredLightingStage : public RenderStage
	{
	public:

		DeferredLightingStage();

		~DeferredLightingStage();

		virtual void initialize() override;

		virtual void finalize() override;

		virtual void render() override;


		void setCameraPosition(const Vector3d& cameraPosition);

		void useGBufferTexture(const std::string& samplerName, Texture* texture);

		DirectionalShadow shadow;
		PointShadow shadowPoint;

	private:

		static void passDirectionalShadow(uint32 index, Shader& shader, const DirectionalShadow& shadow);

		static void passPointShadow(uint32 index, Shader& shader, const PointShadow& shadow);

	private:

		Shader _lightingShader;
		Vector3d _cameraPosition;
		std::vector<std::pair<std::string, Texture*>> _gbufferTextures;
	};
}

#endif // !DEFERRED_LIGHTING_STAGE_H_

