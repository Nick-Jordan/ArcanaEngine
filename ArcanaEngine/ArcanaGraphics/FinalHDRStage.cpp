#include "FinalHDRStage.h"

#include "ObjectRenderer.h"

namespace Arcana
{

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

		_finalHDRShader.createProgram(Shader::Vertex, "resources/quad_vert.glsl");
		_finalHDRShader.createProgram(Shader::Fragment, "resources/final_hdr_frag.glsl");
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
			_finalHDRShader.getUniform("u_FinalColor")->setValue(unit);

			if (_emissiveTexture)
			{
				unit = _emissiveTexture->bind();
				_finalHDRShader.getUniform("u_EmissiveColor")->setValue(unit);
			}

			_finalHDRShader.getUniform("u_Exposure")->setValue(1.0f);

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
