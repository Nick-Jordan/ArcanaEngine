#include "ImageManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

#include "ArcanaLog.h"

/*extern "C"
{
#include "jpeg/jpeglib.h"
#include "jpeg/jerror.h"
}*/

namespace Arcana
{

	bool ImageManager::createFromFile(const std::string& filename, std::vector<uint8>& pixels, Vector2i& size, ImageFormat& format)
	{

		pixels.clear();

		int32 width = 0;
		int32 height = 0;
		int32 channels = 0;
		uint8* ptr = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);

		if (ptr)
		{
			size = Vector2i(width, height);

			if (width && height)
			{
				pixels.resize(width * height * 4);
				memcpy(&pixels[0], ptr, pixels.size());
			}

			stbi_image_free(ptr);

			format = ImageFormat::RGBA;

			return true;
		}
		else
		{
			LOGF(Error, CoreEngine, "Failed to load image \"%s\". Reason: %s", filename.c_str(), stbi_failure_reason());
			return false;
		}



	}

	bool ImageManager::createFromData(const void* data, int64 dataSize, std::vector<uint8>& pixels, Vector2i& size, ImageFormat& format)
	{

		if (data && dataSize)
		{
			pixels.clear();

			int32 width = 0;
			int32 height = 0;
			int32 channels = 0;
			const uint8* buffer = static_cast<const uint8*>(data);
			uint8* ptr = stbi_load_from_memory(buffer, static_cast<int32>(dataSize), &width, &height, &channels, STBI_rgb_alpha);

			if (ptr)
			{
				size = Vector2i(width, height);

				if (width && height)
				{
					pixels.resize(width * height * 4);
					memcpy(&pixels[0], ptr, pixels.size());
				}

				stbi_image_free(ptr);

				format = ImageFormat::RGBA;

				return true;
			}
			else
			{
				LOGF(Error, CoreEngine, "Failed to load image from memory. Reason: %s", stbi_failure_reason());
				return false;
			}
		}
		else
		{
			LOG(Error, CoreEngine, "Failed to load image from memory, data was nullptr");
			return false;
		}
	}

	bool ImageManager::createFromInputStream(InputStream& input, std::vector<uint8>& pixels, Vector2i& size, ImageFormat& format)
	{

		pixels.clear();

		input.seek(0);

		stbi_io_callbacks callbacks;
		callbacks.read = &read;
		callbacks.skip = &skip;
		callbacks.eof = &eof;

		int32 width = 0;
		int32 height = 0;
		int32 channels = 0;
		uint8* ptr = stbi_load_from_callbacks(&callbacks, &input, &width, &height, &channels, STBI_rgb_alpha);

		if (ptr)
		{
			size = Vector2i(width, height);

			if (width && height)
			{
				pixels.resize(width * height * 4);
				memcpy(&pixels[0], ptr, pixels.size());
			}

			stbi_image_free(ptr);

			return true;
		}
		else
		{
			LOGF(Error, CoreEngine, "Failed to load image from stream. Reason: %s", stbi_failure_reason());
			return false;
		}

	}

	bool ImageManager::saveImage(const std::string& filename, const std::vector<uint8>& pixels, const Vector2i& size)
	{

		if (!pixels.empty() && (size.x > 0) && (size.y > 0))
		{
			const std::size_t dot = filename.find_last_of('.');
			const std::string extension = dot != std::string::npos ? filename.substr(dot + 1) : ""; //to_lowercase(filename.substr(dot + 1))

			if (extension == "bmp")
			{
				if (stbi_write_bmp(filename.c_str(), size.x, size.y, 4, &pixels[0]))
					return true;
			}
			else if (extension == "tga")
			{
				if (stbi_write_tga(filename.c_str(), size.x, size.y, 4, &pixels[0]))
					return true;
			}
			else if (extension == "png")
			{
				if (stbi_write_png(filename.c_str(), size.x, size.y, 4, &pixels[0], 0))
					return true;
			}
			else if (extension == "jpg" || extension == "jpeg")
			{
				//if (saveJPG(filename, pixels, size.x, size.y))
				//	return true;

				return false;
			}
		}

		LOGF(Error, CoreEngine, "Failed to save image \"%s\"", filename.c_str());
		return false;
	}

	/*template<typename PixelType>
	bool ImageManager<PixelType>::saveJPG(const std::string& filename, const std::vector<PixelType>& pixels, uint32 width, uint32 height)
	{
	FILE* file = fopen(filename.c_str(), "wb");
	if (!file)
	{
	return false;
	}

	jpeg_compress_struct compressInfos;
	jpeg_error_mgr errorManager;
	compressInfos.err = jpeg_std_error(&errorManager);

	jpeg_create_compress(&compressInfos);
	compressInfos.image_width = width;
	compressInfos.image_height = height;
	compressInfos.input_components = 3;
	compressInfos.in_color_space = JCS_RGB;
	jpeg_stdio_dest(&compressInfos, file);
	jpeg_set_defaults(&compressInfos);
	jpeg_set_quality(&compressInfos, 90, TRUE);

	std::vector<uint8> buffer(width * height * 3);
	for (std::size_t i = 0; i < width * height; ++i)
	{
	buffer[i * 3 + 0] = pixels[i * 4 + 0];
	buffer[i * 3 + 1] = pixels[i * 4 + 1];
	buffer[i * 3 + 2] = pixels[i * 4 + 2];
	}
	uint8* ptr = &buffer[0];

	jpeg_start_compress(&compressInfos, TRUE);

	while (compressInfos.next_scanline < compressInfos.image_height)
	{
	JSAMPROW rawPointer = ptr + (compressInfos.next_scanline * width * 3);
	jpeg_write_scanlines(&compressInfos, &rawPointer, 1);
	}

	jpeg_finish_compress(&compressInfos);
	jpeg_destroy_compress(&compressInfos);

	fclose(file);

	return true;
	}*/

	int32 ImageManager::read(void* user, ANSICHAR* data, int32 size)
	{
		InputStream* stream = static_cast<InputStream*>(user);
		return static_cast<int32>(stream->read(data, size));
	}
	void ImageManager::skip(void* user, int32 size)
	{
		InputStream* stream = static_cast<InputStream*>(user);
		stream->seek(stream->tell() + size);
	}
	int32 ImageManager::eof(void* user)
	{
		InputStream* stream = static_cast<InputStream*>(user);
		return stream->tell() >= stream->size();
	}
}