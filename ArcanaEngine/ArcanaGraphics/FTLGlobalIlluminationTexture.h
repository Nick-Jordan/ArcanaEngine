#ifndef FTL_GLOBAL_ILLUMINATION_TEXTURE_H_
#define FTL_GLOBAL_ILLUMINATION_TEXTURE_H_

#include "GraphicsDefines.h"

#include "Texture.h"
#include "AxisAlignedBoundingBox.h"

namespace Arcana
{

	class ARCANA_GRAPHICS_API FTLGlobalIlluminationTexture
	{
	public:

		FTLGlobalIlluminationTexture();

		FTLGlobalIlluminationTexture(Texture* data, AxisAlignedBoundingBoxf boundingBox);

		FTLGlobalIlluminationTexture(const FTLGlobalIlluminationTexture& copy);

		~FTLGlobalIlluminationTexture();

		Texture* getData() const;

		const AxisAlignedBoundingBoxf& getBoundingBox() const;

		void set(Texture* data, AxisAlignedBoundingBoxf boundingBox);

		FTLGlobalIlluminationTexture& operator=(const FTLGlobalIlluminationTexture& copy);

	private:

		Texture* _texture;

		AxisAlignedBoundingBoxf _boundingBox;
	};

}

#endif // !FTL_GLOBAL_ILLUMINATION_TEXTURE_H_

