#ifndef IMAGE_MANAGER_H_
#define IMAGE_MANAGER_H_

#include "GraphicsDefines.h"

#include "Types.h"
#include "Vector2.h"
#include "FileInputStream.h"
#include "MemoryInputStream.h"
#include <vector>
#include <string>

namespace Arcana
{
	enum ImageFormat
	{
		UnknownFormat = -1,

		RGB = 3,
		RGBA = 4
	};

	class ARCANA_GRAPHICS_API ImageManager
	{
	public:

		static bool createFromFile(const std::string& filename, std::vector<uint8>& pixels, Vector2i& size, ImageFormat& format);

		static bool createFromData(const void* data, int64 dataSize, std::vector<uint8>& pixels, Vector2i& size, ImageFormat& format);

		static bool createFromInputStream(InputStream& input, std::vector<uint8>& pixels, Vector2i& size, ImageFormat& format);

		static bool saveImage(const std::string& filename, const std::vector<uint8>& pixels, const Vector2i& size);

	private:

		//static bool saveJPG(const std::string& filename, const std::vector<PixelType>& pixels, uint32 width, uint32 height);

		static int32 read(void* user, ANSICHAR* data, int32 size);
		static void skip(void* user, int32 size);
		static int32 eof(void* user);
	};
}

#endif // !IMAGE_MANAGER_H_