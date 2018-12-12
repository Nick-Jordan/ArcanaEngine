
#include "ArcanaLog.h"

namespace Arcana
{
	template<typename PixelType>
	Image<PixelType>::Image() : _format(UnknownFormat), _size(Vector2i::zero())
	{

	}

	template<typename PixelType>
	Image<PixelType>::~Image()
	{
		_bytes.clear();
	}

	template<typename PixelType>
	bool Image<PixelType>::init(ImageFormat format, uint32 width, uint32 height, const Vector4<PixelType>& color) //color class?
	{
		if (!width || !height)
		{
			_size = Vector2i::zero();
			_bytes.clear();
			return false;
		}

		_format = format;
		_size = Vector2i(width, height);

		std::size_t total = width * height * format;
		_bytes.resize(total);

		PixelType* begin = &_bytes[0];
		PixelType* end = begin + _bytes.size();
		while (begin < end)
		{
			*begin++ = color.x;
			*begin++ = color.y;
			*begin++ = color.z;

			if (format == RGBA)
			{
				*begin++ = color.w;
			}
		}
	}

	template<typename PixelType>
	bool Image<PixelType>::init(ImageFormat format, uint32 width, uint32 height, const PixelType* pixels)
	{
		if (!pixels || !width || !height)
		{
			_size = Vector2i::zero();
			_bytes.clear();
			return false;
		}

		_format = format;
		_size = Vector2i(width, height);

		std::size_t total = width * height * format;
		_bytes.resize(total);
		std::memcpy(&_bytes[0], pixels, total);

		return true;
	}

	template<typename PixelType>
	bool Image<PixelType>::init(const std::string& file)
	{
		if (AreTypesEqual<PixelType, uint8>::Value)
		{
			return ImageManager::createFromFile(file, _bytes, _size, _format);
		}

		LOG(Error, CoreEngine, "Attempting to load an image from file with PixelType other than uint8");
		return false;
	}

	template<typename PixelType>
	bool Image<PixelType>::init(InputStream& input)
	{
		if (AreTypesEqual<PixelType, uint8>::Value)
		{
			return ImageManager::createFromInputStream(input, _bytes, _size, _format);
		}

		LOG(Error, CoreEngine, "Attempting to load an image from an input stream with PixelType other than uint8");
		return false;
	}

	template<typename PixelType>
	bool Image<PixelType>::init(const void* data, uint64 size)
	{
		if (AreTypesEqual<PixelType, uint8>::Value)
		{
			return ImageManager::createFromData(data, size, _bytes, _size, _format);
		}

		LOG(Error, CoreEngine, "Attempting to load an image from memory with PixelType other than uint8");
		return false;
	}

	template<typename PixelType>
	bool Image<PixelType>::save(const std::string& file)
	{
		if (AreTypesEqual<PixelType, uint8>::Value)
		{
			return ImageManager::saveImage(file, _bytes, _size);
		}

		LOG(Error, CoreEngine, "Attempting to save an image with PixelType other than uint8");
		return false;
	}

	template<typename PixelType>
	ImageFormat Image<PixelType>::getFormat() const
	{
		return _format;
	}

	template<typename PixelType>
	const std::vector<PixelType>& Image<PixelType>::getPixels() const
	{
		return _bytes;
	}

	template<typename PixelType>
	const PixelType* Image<PixelType>::getPixelsPtr() const
	{
		return &_bytes[0];
	}

	template<typename PixelType>
	const Vector2i& Image<PixelType>::getSize() const
	{
		return _size;
	}

	template<typename PixelType>
	uint32 Image<PixelType>::getWidth() const
	{
		return _size.x;
	}

	template<typename PixelType>
	uint32 Image<PixelType>::getHeight() const
	{
		return _size.y;
	}

	template<typename PixelType>
	void Image<PixelType>::setPixel(uint32 x, uint32 y, const Vector4<PixelType>& color) //color class?
	{
		if (x < _size.x && y < _size.y)
		{
			PixelType* pixel = &_bytes[(x + y * _size.x) * _format];
			*pixel++ = color.x;
			*pixel++ = color.y;
			*pixel++ = color.z;
			if (_format == RGBA)
			{
				*pixel++ = color.w;
			}
		}
	}

	template<typename PixelType>
	Vector4<PixelType> Image<PixelType>::getPixel(uint32 x, uint32 y) const
	{
		if (x < _size.x && y < _size.y)
		{
			const PixelType* pixel = &_bytes[(x + y * _size.x) * _format];

			return Vector4<PixelType>(pixel[0], pixel[1], pixel[2], _format == RGBA ? pixel[3] : (PixelType)1);
		}

		return Vector4<PixelType>::zero();
	}

	template<typename PixelType>
	void Image<PixelType>::flip(FlipAxis axis)
	{
		if (!_bytes.empty())
		{
			if (axis == Horizontal)
			{
				std::size_t rowSize = _size.x * 4;

				for (std::size_t y = 0; y < _size.y; ++y)
				{
					std::vector<uint8>::iterator left = _bytes.begin() + y * rowSize;
					std::vector<uint8>::iterator right = _bytes.begin() + (y + 1) * rowSize - 4;

					for (std::size_t x = 0; x < _size.x / 2; ++x)
					{
						std::swap_ranges(left, left + 4, right);

						left += 4;
						right -= 4;
					}
				}
			}
			else
			{
				std::size_t rowSize = _size.x * 4;

				std::vector<uint8>::iterator top = _bytes.begin();
				std::vector<uint8>::iterator bottom = _bytes.end() - rowSize;

				for (std::size_t y = 0; y < _size.y / 2; ++y)
				{
					std::swap_ranges(top, top + rowSize, bottom);

					top += rowSize;
					bottom -= rowSize;
				}
			}
		}
	}
}
