#include "FTLGlobalIlluminationTexture.h"

namespace Arcana
{

	FTLGlobalIlluminationTexture::FTLGlobalIlluminationTexture() : _texture(nullptr)
	{
	}

	FTLGlobalIlluminationTexture::FTLGlobalIlluminationTexture(Texture* data, AxisAlignedBoundingBoxf boundingBox) : _texture(nullptr)
	{
		set(data, boundingBox);
	}

	FTLGlobalIlluminationTexture::FTLGlobalIlluminationTexture(const FTLGlobalIlluminationTexture& copy)
	{
		set(copy._texture, copy._boundingBox);
	}

	FTLGlobalIlluminationTexture::~FTLGlobalIlluminationTexture()
	{
		AE_RELEASE(_texture);
	}

	Texture* FTLGlobalIlluminationTexture::getData() const
	{
		return _texture;
	}

	const AxisAlignedBoundingBoxf& FTLGlobalIlluminationTexture::getBoundingBox() const
	{
		return _boundingBox;
	}

	void FTLGlobalIlluminationTexture::set(Texture* data, AxisAlignedBoundingBoxf boundingBox)
	{
		AE_RELEASE(_texture);
		_texture = data;
		AE_REFERENCE(_texture);

		_boundingBox = boundingBox;
	}

	FTLGlobalIlluminationTexture& FTLGlobalIlluminationTexture::operator=(const FTLGlobalIlluminationTexture& copy)
	{
		set(copy._texture, copy._boundingBox);

		return *this;
	}
}
