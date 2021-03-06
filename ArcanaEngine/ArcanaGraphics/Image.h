#ifndef IMAGE_H_
#define IMAGE_H_

#include "Object.h"
#include "Color.h"

#include "ImageManager.h"
#include "TypeTraits.h"

#include <algorithm>

namespace Arcana
{
	template<typename PixelType>
	class Image : public Object
	{
	public:

		enum FlipAxis
		{
			Horizontal,
			Vertical
		};

		Image();

		virtual ~Image();

		bool init(ImageFormat format, uint32 width, uint32 height, const Vector4<PixelType>& color); //color class?

		bool init(ImageFormat format, uint32 width, uint32 height, const Color& color); //color class?

		bool init(ImageFormat format, uint32 width, uint32 height, const PixelType* pixels);

		bool init(const std::string& file);

		bool init(InputStream& input);

		bool init(const void* data, uint64 size);

		bool save(const std::string& file);
		
		ImageFormat getFormat() const;

		const std::vector<PixelType>& getPixels() const;

		const PixelType* getPixelsPtr() const;

		const Vector2i& getSize() const;

		uint32 getWidth() const;

		uint32 getHeight() const;

		void setPixel(uint32 x, uint32 y, const Vector4<PixelType>& color);

		void setPixel(uint32 x, uint32 y, const Color& color);

		Vector4<PixelType> getPixel(uint32 x, uint32 y) const;

		Color getPixelColor(uint32 x, uint32 y) const;

		void flip(FlipAxis axis);

	private:

		ImageFormat _format;

		Vector2i _size;

		std::vector<PixelType> _bytes;
	};

}

#include "Image.inl"

#endif // !IMAGE_H_
