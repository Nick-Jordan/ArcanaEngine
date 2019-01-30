#ifndef FINAL_HDR_STAGE_H_
#define FINAL_HDR_STAGE_H_

#include "RenderStage.h"

namespace Arcana
{
	class FinalHDRStage : public RenderStage
	{
	public:

		FinalHDRStage();

		~FinalHDRStage();

		virtual void initialize() override;

		virtual void finalize() override;

		virtual void render() override;


		void useHDRTexture(Texture* texture);

		void useEmissiveTexture(Texture* texture);

	private:

		Texture* _hdrTexture;
		Texture* _emissiveTexture;

		Shader _finalHDRShader;
	};
}

#endif // !FINAL_HDR_STAGE_H_

