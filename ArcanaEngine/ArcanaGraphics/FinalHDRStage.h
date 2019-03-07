#ifndef FINAL_HDR_STAGE_H_
#define FINAL_HDR_STAGE_H_

#include "GraphicsDefines.h"

#include "RenderStage.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API FinalHDRStage : public RenderStage
	{
	public:

		FinalHDRStage();

		~FinalHDRStage();

		virtual void initialize() override;

		virtual void finalize() override;

		virtual void render() override;


		void useHDRTexture(Texture* texture);

		void useEmissiveTexture(Texture* texture);

		//test
		static float Exposure;

	private:

		Texture* _hdrTexture;
		Texture* _emissiveTexture;

		Shader _finalHDRShader;
	};
}

#endif // !FINAL_HDR_STAGE_H_

