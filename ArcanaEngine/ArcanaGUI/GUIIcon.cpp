#include "GUIIcon.h"

namespace Arcana
{
	GUIIcon::GUIIcon(const std::string& path) : _path(path), _texture(nullptr)
	{
		initialize();
	}

	GUIIcon::~GUIIcon()
	{
		AE_RELEASE(_texture);
	}

	const std::string& GUIIcon::getPath() const
	{
		return _path;
	}

	Texture* GUIIcon::getTexture() const
	{
		return _texture;
	}

	uint32 GUIIcon::getWidth() const
	{
		return (uint32)_size.x;
	}

	uint32 GUIIcon::getHeight() const
	{
		return (uint32)_size.y;
	}

	void GUIIcon::initialize()
	{
		Image<uint8> image;
		if (!image.init(_path))
			return;

		Texture::Parameters params;
		params.setMinFilter(TextureFilter::Linear);
		params.setMagFilter(TextureFilter::Linear);

		_size = Vector2i(image.getWidth(), image.getHeight());

		_texture = Texture::create2D(Texture::RGBA, image.getWidth(), image.getHeight(), Texture::RGBA8, Texture::UnsignedByte, image.getPixelsPtr(), params);
	}
}