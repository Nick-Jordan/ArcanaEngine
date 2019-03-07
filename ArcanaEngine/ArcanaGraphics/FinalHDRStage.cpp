#include "FinalHDRStage.h"

#include "ObjectRenderer.h"
#include "GlobalShaders.h"

namespace Arcana
{
	float FinalHDRStage::Exposure = 1.0f;


	FinalHDRStage::FinalHDRStage() : RenderStage("FinalHDRStage")
	{
	}


	FinalHDRStage::~FinalHDRStage()
	{
	}

	void FinalHDRStage::initialize()
	{
		_hdrTexture = nullptr;
		_emissiveTexture = nullptr;

		_finalHDRShader = *GlobalShaders::get(GlobalShaders::FinalHDR);
	}

	void FinalHDRStage::finalize()
	{
		_hdrTexture = nullptr;
		_emissiveTexture = nullptr;
	}

	void FinalHDRStage::render()
	{
		if (_hdrTexture)
		{
			_finalHDRShader.bind();

			int32 unit = _hdrTexture->bind();
			_finalHDRShader.getUniform("u_FinalColor").setValue(unit);

			if (_emissiveTexture)
			{
				unit = _emissiveTexture->bind();
				_finalHDRShader.getUniform("u_EmissiveColor").setValue(unit);
			}

			_finalHDRShader.getUniform("u_Exposure").setValue(Exposure); //TEMPORARY SOLUTION ---- ALLOW USER TO CHANGE THIS

			ObjectRenderer::drawQuad();

			_finalHDRShader.unbind();
		}

		_hdrTexture = nullptr;
		_emissiveTexture = nullptr;
	}

	void FinalHDRStage::useHDRTexture(Texture* texture)
	{
		_hdrTexture = texture;
	}

	void FinalHDRStage::useEmissiveTexture(Texture* texture)
	{
		_emissiveTexture = texture;
	}
}
