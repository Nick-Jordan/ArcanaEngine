#ifndef PROCEDURAL_TEXTURE_H_
#define PROCEDURAL_TEXTURE_H_

#include "GraphicsDefines.h"

#include "TextureCacheObject.h"
#include "TextureCacheObjectParameters.h"

namespace Arcana
{
	
	class ARCANA_GRAPHICS_API ProceduralTexture : public TextureCacheObject
	{
	public:
	
		class ARCANA_GRAPHICS_API Parameters
		{

		};

		ProceduralTexture();
		
		virtual ~ProceduralTexture();
		
		virtual Texture* buildTexture() override;
		
		ProceduralTexture* create(const Parameters& params, const TextureCacheObjectParameters& textureParams);
	
	};
}

#endif // !PROCEDURAL_TEXTURE_H_