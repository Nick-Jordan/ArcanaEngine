#ifndef BLOOM_CALCULATION_STAGE_H_
#define BLOOM_CALCULATION_STAGE_H_

#include "GraphicsDefines.h"

#include "RenderStage.h"

#include "Framebuffer.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API BloomCalculationStage : public RenderStage
	{
	public:

		BloomCalculationStage();

		~BloomCalculationStage();

		virtual void initialize() override;

		virtual void finalize() override;

		virtual void render() override;

		Texture* getEmissiveColorBuffer() const;

		void useEmissiveTexture(Texture* texture);

	private:

		Framebuffer* _pingpongFBO[2];
		Texture* _pingpongColorBuffer[2];
		Texture* _firstEmissiveTexture;
		Shader _blurShader;
		bool _horizontal;
	};
}

#endif // !BLOOM_CALCULATION_STAGE_H_

