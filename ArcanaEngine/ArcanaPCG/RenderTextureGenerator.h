#ifndef RENDER_TEXTURE_GENERATOR_H_
#define RENDER_TEXTURE_GENERATOR_H_

#include "ProceduralGenerator.h"
#include "RenderTextureProceduralStep.h"

namespace Arcana
{
	class RenderTextureGenerator : public ProceduralGenerator<Texture, TextureProceduralParameters>
	{
	public:

		virtual void setupGenerationSteps() override
		{
			SyncSteps.add(new RenderTextureProceduralStep());
		};
	};
}

#endif // !RENDER_TEXTURE_GENERATOR_H_