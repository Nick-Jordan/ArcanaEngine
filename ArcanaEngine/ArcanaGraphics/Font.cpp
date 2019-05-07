#include "Font.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Arcana
{
	INITIALIZE_CATEGORY(Arcana, FontLog);

	std::vector<Font*> Font::__fontCache;

	Font::Font()
		: _format(Bitmap), _style(Plain), _size(0), _spacing(0.0f),
		_glyphs(nullptr), _glyphCount(0), _texture(nullptr), _renderer(nullptr)
	{

	}

	Font::Font(const Font& copy)
	{

	}

	Font& Font::operator=(const Font&)
	{
		return *this;
	}

	Font::~Font()
	{
		std::vector<Font*>::iterator itr = std::find(__fontCache.begin(), __fontCache.end(), this);
		if (itr != __fontCache.end())
		{
			__fontCache.erase(itr);
		}

		AE_DELETE(_renderer);
		AE_DELETE_ARRAY(_glyphs);
		AE_RELEASE(_texture);

		for (size_t i = 0, count = _sizes.size(); i < count; ++i)
		{
			AE_RELEASE(_sizes[i]);
		}
	}

	Font* Font::load(const std::string& path, const std::string& id, std::vector<uint32>& fontSizes)
	{
		FT_Library library;
		FT_Error error = FT_Init_FreeType(&library);
		if (error)
		{
			LOGF(Error, FontLog, "FT_Init_FreeType error: %d", error);
			return nullptr;
		}

		FT_Face face;
		error = FT_New_Face(library, path.c_str(), 0, &face);
		if (error)
		{
			LOGF(Error, FontLog, "FT_New_Face error: %d", error);
			return nullptr;
		}


		std::vector<Font*> fonts;

		Font* masterFont = NULL;

		for (size_t fontIndex = 0, count = fontSizes.size(); fontIndex < count; ++fontIndex)
		{
			uint32 fontSize = fontSizes[fontIndex];

			Glyph* glyphArray = new Glyph[127 - 32];

			int32 rowSize = 0;
			int32 glyphSize = 0;
			int32 actualfontHeight = 0;

			FT_Int32 loadFlags = FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT;

			for (uint32 requestedSize = fontSize; requestedSize > 0; --requestedSize)
			{
				error = FT_Set_Char_Size(face, 0, requestedSize * 64, 0, 0);
				if (error)
				{
					LOGF(Error, FontLog, "FT_Set_Pixel_Sizes error: %d", error);
					return nullptr;
				}



				rowSize = 0;
				glyphSize = 0;
				actualfontHeight = 0;

				for (uint8 ascii = 32; ascii < 127; ++ascii)
				{
					error = FT_Load_Char(face, ascii, loadFlags);
					if (error)
					{
						LOGF(Error, FontLog, "FT_Load_Char error: %d", error);
					}

					int32 bitmapRows = face->glyph->bitmap.rows;
					actualfontHeight = (actualfontHeight < bitmapRows) ? bitmapRows : actualfontHeight;

					if (face->glyph->bitmap.rows > face->glyph->bitmap_top)
					{
						bitmapRows += (face->glyph->bitmap.rows - face->glyph->bitmap_top);
					}
					rowSize = (rowSize < bitmapRows) ? bitmapRows : rowSize;
				}

				if (rowSize <= (int32)fontSize)
				{
					glyphSize = rowSize;
					rowSize = fontSize;
					break;
				}
			}

			if (face->glyph == nullptr || glyphSize == 0)
			{
				LOGF(Error, FontLog, "Cannot generate a font of the requested size: %d", fontSize);
				return nullptr;
			}

			rowSize += 4;

			int32 penX = 0;
			int32 penY = 0;
			int32 row = 0;

			double powerOf2 = 2;
			uint32 imageWidth = 0;
			uint32 imageHeight = 0;
			bool textureSizeFound = false;

			int32 advance;
			int32 i;

			while (textureSizeFound == false)
			{
				imageWidth = (uint32)pow(2.0, powerOf2);
				imageHeight = (uint32)pow(2.0, powerOf2);
				penX = 0;
				penY = 0;
				row = 0;

				i = 0;
				for (uint8 ascii = 32; ascii < 127; ++ascii)
				{
					error = FT_Load_Char(face, ascii, loadFlags);
					if (error)
					{
						LOGF(Error, FontLog, "FT_Load_Char error: %d", error);
					}
					int32 glyphWidth = face->glyph->bitmap.pitch;
					int32 glyphHeight = face->glyph->bitmap.rows;

					advance = glyphWidth + 4;

					if ((penX + advance) > (int32)imageWidth)
					{
						penX = 0;
						row += 1;
						penY = row * rowSize;
						if (penY + rowSize > (int32)imageHeight)
						{
							powerOf2++;
							break;
						}
					}

					penY += (actualfontHeight - glyphHeight) + (glyphHeight - face->glyph->bitmap_top);

					penX += advance;
					penY = row * rowSize;

					if (ascii == (127 - 1))
					{
						textureSizeFound = true;
					}
					i++;
				}
			}

			powerOf2 = 1;
			for (;;)
			{
				if ((penY + rowSize) >= pow(2.0, powerOf2))
				{
					powerOf2++;
				}
				else
				{
					imageHeight = (int32)pow(2.0, powerOf2);
					break;
				}
			}

			uint8* imageBuffer = (uint8*)malloc(imageWidth * imageHeight);
			memset(imageBuffer, 0, imageWidth * imageHeight);

			penX = 1;
			penY = 0;
			row = 0;
			i = 0;
			for (uint8 ascii = 32; ascii < 127; ++ascii)
			{
				error = FT_Load_Char(face, ascii, loadFlags);
				if (error)
				{
					LOGF(Error, FontLog, "FT_Load_Char error: %d", error);
				}

				uint8* glyphBuffer = face->glyph->bitmap.buffer;

				int32 glyphWidth = face->glyph->bitmap.pitch;
				int32 glyphHeight = face->glyph->bitmap.rows;

				advance = glyphWidth + 4;

				if ((penX + advance) > (int32)imageWidth)
				{
					penX = 1;
					row += 1;
					penY = row * rowSize;
					if (penY + rowSize > (int32)imageHeight)
					{
						free(imageBuffer);
						LOG(Error, FontLog, "Image size exceeded!");
						return nullptr;
					}
				}

				penY += (actualfontHeight - glyphHeight) + (glyphHeight - face->glyph->bitmap_top);

				drawBitmap(imageBuffer, penX, penY, imageWidth, glyphBuffer, glyphWidth, glyphHeight);

				penY = row * rowSize;

				glyphArray[i].code = ascii;
				glyphArray[i].width = advance - 4;
				glyphArray[i].bearingX = face->glyph->metrics.horiBearingX >> 6;
				glyphArray[i].advance = face->glyph->metrics.horiAdvance >> 6;

				glyphArray[i].uvs[0] = (float)penX / (float)imageWidth;
				glyphArray[i].uvs[1] = (float)penY / (float)imageHeight;
				glyphArray[i].uvs[2] = (float)(penX + advance - 4) / (float)imageWidth;
				glyphArray[i].uvs[3] = (float)(penY + rowSize - 4) / (float)imageHeight;

				penX += advance;
				i++;
			}

			std::string family = face->family_name;

			Texture::Parameters params;
			params.setMinFilter(TextureFilter::Linear);
			params.setMagFilter(TextureFilter::Linear);

			Texture* texture = Texture::create2D(Texture::Red, imageWidth, imageWidth, Texture::R8, Texture::UnsignedByte, imageBuffer, params, true);

			AE_DELETE_ARRAY(imageBuffer);

			if (texture == nullptr)
			{
				LOGF(Info, FontLog, "Failed to create texture for font '%s'.", id);
				AE_DELETE_ARRAY(glyphArray);
				return nullptr;
			}

			Font* font = Font::load(family, Font::Plain, fontSize, glyphArray, 127 - 32, texture, Font::Bitmap);
			fonts.push_back(font);

			AE_DELETE_ARRAY(glyphArray);

			AE_RELEASE(texture);

			if (font)
			{
				font->_path = path;
				font->_id = id;

				if (masterFont)
				{
					masterFont->_sizes.push_back(font);
				}
				else
				{
					masterFont = font;
				}
			}
		}

		FT_Done_Face(face);
		FT_Done_FreeType(library);


		return masterFont;
	}

	Font* Font::load(const std::string& path, const char* id)
	{
		GlobalObjectID guid = GlobalObjectID(std::string(id));

		for (size_t i = 0, count = __fontCache.size(); i < count; ++i)
		{
			Font* f = __fontCache[i];
			if (f && f->_path == path && (id == nullptr || (guid == f->_id)))
			{
				f->reference();
				return f;
			}
		}

		Font* font = load(path, id, std::vector<uint32>{24});

		if (font)
		{
			__fontCache.push_back(font);
		}

		return font;
	}

	Font* Font::load(uint32 mainSize, const std::string& path, const char* id)
	{
		GlobalObjectID guid = GlobalObjectID(std::string(id));

		for (size_t i = 0, count = __fontCache.size(); i < count; ++i)
		{
			Font* f = __fontCache[i];
			if (f && f->_path == path && (id == nullptr || (guid == f->_id)))
			{
				f->reference();
				return f;
			}
		}

		Font* font = load(path, id, std::vector<uint32>{mainSize});

		if (font)
		{
			__fontCache.push_back(font);
		}

		return font;
	}


	uint32 Font::getSize(uint32 index) const
	{
		if (index <= _sizes.size())
		{
			return index == 0 ? _size : _sizes[index - 1]->_size;
		}
		return 0;
	}

	uint32 Font::getSizeCount() const
	{
		return _sizes.size() + 1;
	}

	Font::Format Font::getFormat()
	{
		return _format;
	}

	bool Font::isCharacterSupported(int32 character) const
	{
		int32 glyphIndex = character - 32;
		return (glyphIndex >= 0 && glyphIndex < (int32)_glyphCount);
	}

	void Font::start()
	{
		if (_renderer->isStarted())
			return;

		_renderer->start();
	}

	void Font::drawText(const std::string& text, int32 x, int32 y, const Color& color, uint32 size, bool rightToLeft)
	{
		if (_size == 0)
			return;

		if (size == 0)
		{
			size = _size;
		}
		else
		{
			Font* f = findClosestSize(size);
			if (f != this)
			{
				f->drawText(text, x, y, color, size, rightToLeft);
				return;
			}
		}

		//lazyStart();

		float scale = (float)size / _size;
		int32 spacing = (int32)(size * _spacing);
		const char* cursor = nullptr;

		if (rightToLeft)
		{
			cursor = text.c_str();
		}

		int xPos = x, yPos = y;
		bool done = false;

		while (!done)
		{
			size_t length;
			size_t startIndex;
			int iteration;
			if (rightToLeft)
			{
				char delimiter = cursor[0];
				while (!done &&
					(delimiter == ' ' ||
						delimiter == '\t' ||
						delimiter == '\r' ||
						delimiter == '\n' ||
						delimiter == 0))
				{
					switch (delimiter)
					{
					case ' ':
						xPos += _glyphs[0].advance;
						break;
					case '\r':
					case '\n':
						yPos += size;
						xPos = x;
						break;
					case '\t':
						xPos += _glyphs[0].advance * 4;
						break;
					case 0:
						done = true;
						break;
					}

					if (!done)
					{
						++cursor;
						delimiter = cursor[0];
					}
				}

				length = strcspn(cursor, "\r\n");
				startIndex = length - 1;
				iteration = -1;
			}
			else
			{
				length = text.size();
				startIndex = 0;
				iteration = 1;
			}

			AE_ASSERT(_glyphs);
			AE_ASSERT(_renderer);
			for (size_t i = startIndex; i < length; i += (size_t)iteration)
			{
				char c = 0;
				if (rightToLeft)
				{
					c = cursor[i];
				}
				else
				{
					c = text[i];
				}

				switch (c)
				{
				case ' ':
					xPos += _glyphs[0].advance;
					break;
				case '\r':
				case '\n':
					yPos += size;
					xPos = x;
					break;
				case '\t':
					xPos += _glyphs[0].advance * 4;
					break;
				default:
					int index = c - 32;
					if (index >= 0 && index < (int)_glyphCount)
					{
						Glyph& g = _glyphs[index];

						if (getFormat() == DistanceField)
						{
							//_batch->getMaterial()->getParameter(0, "u_cutoff").setValue(Vector2f(1.0, 1.0));
						}

						_renderer->draw(xPos + (int32)(g.bearingX * scale), yPos, g.width * scale, size, g.uvs[0], g.uvs[1], g.uvs[2], g.uvs[3], color);
						xPos += floor(g.advance * scale + spacing);
						break;
					}
					break;
				}
			}

			if (rightToLeft)
			{
				cursor += length;
			}
			else
			{
				done = true;
			}
		}
	}

	void Font::drawText(const std::string& text, int32 x, int32 y, float red, float green, float blue, float alpha, uint32 size, bool rightToLeft)
	{
		drawText(text, x, y, LinearColor(red, green, blue, alpha).toColor(false), size, rightToLeft);
	}

	void Font::drawText(const std::string& text, const Rectf& area, const Color& color, uint32 size,
		Justify justify, bool wrap, bool rightToLeft,
		const Rectf& clip)
	{
		if (_size == 0)
			return;

		if (size == 0)
		{
			size = _size;
		}
		else
		{
			Font* f = findClosestSize(size);
			if (f != this)
			{
				f->drawText(text, area, color, size, justify, wrap, rightToLeft, clip);
				return;
			}
		}

		//lazyStart();

		float scale = (float)size / _size;
		int32 spacing = (int32)(size * _spacing);
		int32 yPos = area.getTop();
		const float areaHeight = area.getSize().y - size;
		std::vector<int32> xPositions;
		std::vector<uint32> lineLengths;

		getMeasurementInfo(text, area, size, justify, wrap, rightToLeft, xPositions, yPos, lineLengths);

		int32 xPos = area.getLeft();
		std::vector<int>::const_iterator xPositionsIt = xPositions.begin();
		if (xPositionsIt != xPositions.end())
		{
			xPos = *xPositionsIt++;
		}

		const char* token = text.c_str();
		int32 iteration = 1;
		uint32 lineLength;
		uint32 currentLineLength = 0;
		const char* lineStart = "";
		std::vector<uint32>::const_iterator lineLengthsIt;
		if (rightToLeft)
		{
			lineStart = token;
			lineLengthsIt = lineLengths.begin();
			lineLength = *lineLengthsIt++;
			token += lineLength - 1;
			iteration = -1;
		}

		while (token[0] != 0)
		{
			if (!handleDelimiters(&token, size, iteration, area.getLeft(), &xPos, &yPos, &currentLineLength, &xPositionsIt, xPositions.end()))
			{
				break;
			}

			bool truncated = false;
			uint32 tokenLength;
			uint32 tokenWidth;
			uint32 startIndex;
			if (rightToLeft)
			{
				tokenLength = getReversedTokenLength(token, text.c_str());
				currentLineLength += tokenLength;
				token -= (tokenLength - 1);
				tokenWidth = getTokenWidth(token, tokenLength, size, scale);
				iteration = -1;
				startIndex = tokenLength - 1;
			}
			else
			{
				tokenLength = (uint32)strcspn(token, " \r\n\t");
				tokenWidth = getTokenWidth(token, tokenLength, size, scale);
				iteration = 1;
				startIndex = 0;
			}

			if (wrap && (xPos + (int32)tokenWidth > area.getLeft() + area.getSize().x || (rightToLeft && currentLineLength > lineLength)))
			{
				yPos += (int32)size;
				currentLineLength = tokenLength;

				if (xPositionsIt != xPositions.end())
				{
					xPos = *xPositionsIt++;
				}
				else
				{
					xPos = area.getLeft();
				}
			}

			bool draw = true;
			if (yPos < static_cast<int32>(area.getTop() - size))
			{
				draw = false;
			}
			else if (yPos > area.getTop() + areaHeight)
			{
				break;
			}

			AE_ASSERT(_glyphs);
			AE_ASSERT(_renderer);
			for (int32 i = startIndex; i < (int32)tokenLength && i >= 0; i += iteration)
			{
				uint8 c = token[i];
				int32 glyphIndex = c - 32;

				if (glyphIndex >= 0 && glyphIndex < (int32)_glyphCount)
				{
					Glyph& g = _glyphs[glyphIndex];

					if (xPos + (int32)(g.advance*scale) > area.getLeft() + area.getSize().x)
					{
						truncated = true;
						break;
					}
					else if (xPos >= (int32)area.getLeft())
					{
						if (draw)
						{
							if (getFormat() == DistanceField)
							{
								//_batch->getMaterial()->getParameter(0, "u_cutoff").setValue(Vector2f::one());
							}
							if (!clip.isEmpty() && clip.getLeft() != 0 && clip.getTop() != 0)
							{
								_renderer->draw(xPos + (int32)(g.bearingX * scale), yPos, g.width * scale, size, g.uvs[0], g.uvs[1], g.uvs[2], g.uvs[3], color, clip);
							}
							else
							{
								_renderer->draw(xPos + (int32)(g.bearingX * scale), yPos, g.width * scale, size, g.uvs[0], g.uvs[1], g.uvs[2], g.uvs[3], color);
							}
						}
					}
					xPos += (int32)(g.advance)*scale + spacing;
				}
			}

			if (!truncated)
			{
				if (rightToLeft)
				{
					if (token == lineStart)
					{
						token += lineLength;

						if (!handleDelimiters(&token, size, 1, area.getLeft(), &xPos, &yPos, &currentLineLength, &xPositionsIt, xPositions.end()))
						{
							break;
						}

						if (lineLengthsIt != lineLengths.end())
						{
							lineLength = *lineLengthsIt++;
						}
						lineStart = token;
						token += lineLength - 1;
					}
					else
					{
						token--;
					}
				}
				else
				{
					token += tokenLength;
				}
			}
			else
			{
				if (rightToLeft)
				{
					token = lineStart + lineLength;

					if (!handleDelimiters(&token, size, 1, area.getLeft(), &xPos, &yPos, &currentLineLength, &xPositionsIt, xPositions.end()))
					{
						break;
					}

					if (lineLengthsIt != lineLengths.end())
					{
						lineLength = *lineLengthsIt++;
					}
					lineStart = token;
					token += lineLength - 1;
				}
				else
				{
					size_t tokenLength = strcspn(token, "\n");

					if (tokenLength > 0)
					{
						token += tokenLength;
					}
				}
			}
		}
	}

	void Font::finish()
	{
		if (_renderer->isStarted())
			_renderer->finish();

		for (size_t i = 0, count = _sizes.size(); i < count; ++i)
		{
			SpriteImmediateRenderer* r = _sizes[i]->_renderer;
			if (r->isStarted())
				r->finish();
		}
	}

	void Font::measureText(const std::string& text, uint32 size, uint32& widthOut, uint32& heightOut)
	{
		if (_size == 0)
			return;

		if (size == 0)
		{
			size = _size;
		}
		else
		{
			Font* f = findClosestSize(size);
			if (f != this)
			{
				f->measureText(text, size, widthOut, heightOut);
				return;
			}
		}

		const size_t length = text.size();
		if (length == 0)
		{
			widthOut = 0;
			heightOut = 0;
			return;
		}

		float scale = (float)size / _size;
		const char* token = text.c_str();

		widthOut = 0;
		heightOut = size;

		while (token[0] != 0)
		{
			while (token[0] == '\n')
			{
				heightOut += size;
				++token;
			}

			uint32 tokenLength = (uint32)strcspn(token, "\n");
			uint32 tokenWidth = getTokenWidth(token, tokenLength, size, scale);
			if (tokenWidth > widthOut)
			{
				widthOut = tokenWidth;
			}

			token += tokenLength;
		}
	}

	void Font::measureText(const std::string&  text, const Rectf& clip, uint32 size, Rectf& out,
		Justify justify, bool wrap, bool ignoreClip)
	{
		if (_size == 0)
			return;

		if (size == 0)
		{
			size = _size;
		}
		else
		{
			Font* f = findClosestSize(size);
			if (f != this)
			{
				f->measureText(text, clip, size, out, justify, wrap, ignoreClip);
				return;
			}
		}

		if (text.size() == 0)
		{
			out = Rectf(0, 0, 0, 0);
			return;
		}

		float scale = (float)size / _size;
		Justify vAlign = static_cast<Justify>(justify & 0xF0);
		if (vAlign == 0)
		{
			vAlign = AlignTop;
		}

		Justify hAlign = static_cast<Justify>(justify & 0x0F);
		if (hAlign == 0)
		{
			hAlign = AlignLeft;
		}

		const char* token = text.c_str();
		std::vector<bool> emptyLines;
		std::vector<Vector2f> lines;

		uint32 lineWidth = 0;
		int yPos = clip.getTop() + size;
		const float viewportHeight = clip.getSize().y;

		if (wrap)
		{
			uint32 delimWidth = 0;
			bool reachedEOF = false;
			while (token[0] != 0)
			{
				char delimiter = token[0];
				while (delimiter == ' ' ||
					delimiter == '\t' ||
					delimiter == '\r' ||
					delimiter == '\n' ||
					delimiter == 0)
				{
					switch (delimiter)
					{
					case ' ':
						delimWidth += _glyphs[0].advance;
						break;
					case '\r':
					case '\n':

						yPos += size;

						if (lineWidth > 0)
						{
							int hWhitespace = clip.getSize().x - lineWidth;
							int xPos = clip.getLeft();
							if (hAlign == AlignHorizontalCenter)
							{
								xPos += hWhitespace / 2;
							}
							else if (hAlign == AlignRight)
							{
								xPos += hWhitespace;
							}

							emptyLines.push_back(false);
							lines.push_back(Vector2f(xPos, lineWidth));
						}
						else
						{
							emptyLines.push_back(true);
							lines.push_back(Vector2f(FLT_MAX, 0));
						}

						lineWidth = 0;
						delimWidth = 0;
						break;
					case '\t':
						delimWidth += _glyphs[0].advance * 4;
						break;
					case 0:
						reachedEOF = true;
						break;
					}

					if (reachedEOF)
					{
						break;
					}

					token++;
					delimiter = token[0];
				}

				if (reachedEOF)
				{
					break;
				}

				uint32 tokenLength = (uint32)strcspn(token, " \r\n\t");
				uint32 tokenWidth = getTokenWidth(token, tokenLength, size, scale);

				if (lineWidth + tokenWidth + delimWidth > clip.getSize().x)
				{
					yPos += size;

					int hWhitespace = clip.getSize().x - lineWidth;
					int xPos = clip.getLeft();
					if (hAlign == AlignHorizontalCenter)
					{
						xPos += hWhitespace / 2;
					}
					else if (hAlign == AlignRight)
					{
						xPos += hWhitespace;
					}

					emptyLines.push_back(false);
					lines.push_back(Vector2f(xPos, lineWidth));
					lineWidth = 0;
				}
				else
				{
					lineWidth += delimWidth;
				}

				delimWidth = 0;
				lineWidth += tokenWidth;
				token += tokenLength;
			}
		}
		else
		{
			int emptyLinesCount = 0;
			while (token[0] != 0)
			{
				bool nextLine = true;
				while (token[0] == '\n')
				{
					if (nextLine)
					{
						yPos += size * (emptyLinesCount + 1);
						nextLine = false;
						emptyLinesCount = 0;
						emptyLines.push_back(false);
					}
					else
					{
						++emptyLinesCount;
						emptyLines.push_back(true);
						lines.push_back(Vector2f(FLT_MAX, 0));
					}

					token++;
				}

				uint32 tokenLength = (uint32)strcspn(token, "\n");
				lineWidth = getTokenWidth(token, tokenLength, size, scale);

				int xPos = clip.getLeft();
				int hWhitespace = clip.getSize().x - lineWidth;
				if (hAlign == AlignHorizontalCenter)
				{
					xPos += hWhitespace / 2;
				}
				else if (hAlign == AlignRight)
				{
					xPos += hWhitespace;
				}

				lines.push_back(Vector2f(xPos, lineWidth));

				token += tokenLength;
			}

			yPos += size;
		}

		if (wrap)
		{
			int hWhitespace = clip.getSize().x - lineWidth;
			int xPos = clip.getLeft();
			if (hAlign == AlignHorizontalCenter)
			{
				xPos += hWhitespace / 2;
			}
			else if (hAlign == AlignRight)
			{
				xPos += hWhitespace;
			}

			lines.push_back(Vector2f(xPos, lineWidth));
		}

		int32 x = INT_MAX;
		int32 y = clip.getTop();
		uint32 width = 0;
		int height = yPos - clip.getTop();

		int vWhitespace = viewportHeight - height;
		if (vAlign == AlignVerticalCenter)
		{
			y += vWhitespace / 2;
		}
		else if (vAlign == AlignBottom)
		{
			y += vWhitespace;
		}

		int clippedTop = 0;
		int clippedBottom = 0;
		if (!ignoreClip)
		{
			if (y >= clip.getTop())
			{
				clippedBottom = (height - viewportHeight) / size + 1;
				if (clippedBottom > 0)
				{
					size_t emptyIndex = emptyLines.size() - clippedBottom;
					while (emptyIndex < emptyLines.size() && emptyLines[emptyIndex] == true)
					{
						height -= size;
						emptyIndex++;
					}

					height -= size * clippedBottom;
				}
				else
				{
					clippedBottom = 0;
				}
			}
			else
			{
				clippedTop = (clip.getTop() - y) / size + 1;
				if (clippedTop < 0)
				{
					clippedTop = 0;
				}

				size_t emptyIndex = clippedTop;
				while (emptyIndex < emptyLines.size() && emptyLines[emptyIndex] == true)
				{
					y += size;
					height -= size;
					emptyIndex++;
				}

				if (vAlign == AlignVerticalCenter)
				{
					clippedBottom = (height - viewportHeight + vWhitespace / 2 + 0.01) / size + 1;
					if (clippedBottom > 0)
					{
						emptyIndex = emptyLines.size() - clippedBottom;
						while (emptyIndex < emptyLines.size() && emptyLines[emptyIndex] == true)
						{
							height -= size;
							emptyIndex++;
						}

						height -= size * clippedBottom;
					}
					else
					{
						clippedBottom = 0;
					}
				}

				y = y + size * clippedTop;
				height = height - size * clippedTop;
			}
		}

		for (int i = clippedTop; i < (int)lines.size() - clippedBottom; ++i)
		{
			if (lines[i].x < x)
			{
				x = lines[i].x;
			}
			if (lines[i].y > width)
			{
				width = lines[i].y;
			}
		}

		if (!ignoreClip)
		{
			out = Rectf(
				(x >= clip.getLeft()) ? x : clip.getLeft(),
				(y >= clip.getTop()) ? y : clip.getTop(),
				(width <= clip.getSize().x) ? width : clip.getSize().x,
				(height <= viewportHeight) ? height : viewportHeight
			);
		}
		else
		{
			out = Rectf(x, y, width, height);
		}
	}

	float Font::getCharacterSpacing() const
	{
		return _spacing;
	}

	void Font::setCharacterSpacing(float spacing)
	{
		_spacing = spacing;
	}

	int32 Font::getIndexAtLocation(const std::string& text, const Rectf& clip, uint32 size, const Vector2f& inLocation,
		Vector2f& outLocation, Justify justify, bool wrap, bool rightToLeft)
	{
		return getIndexOrLocation(text, clip, size, inLocation, outLocation, -1, justify, wrap, rightToLeft);
	}

	void Font::getLocationAtIndex(const std::string&  text, const Rectf& clip, uint32 size, Vector2f& outLocation,
		const uint32 destIndex, Justify justify, bool wrap,
		bool rightToLeft)
	{
		getIndexOrLocation(text, clip, size, outLocation, outLocation, (const int32)destIndex, justify, wrap, rightToLeft);;
	}

	SpriteImmediateRenderer* Font::getSpriteImmediateRenderer(uint32 size) const
	{
		if (size == 0)
			return _renderer;

		return const_cast<Font*>(this)->findClosestSize(size)->_renderer;
	}

	Texture* Font::getGlyphTexture() const
	{
		return _texture;
	}

	void Font::drawBitmap(uint8* dstBitmap, int32 x, int32 y, int32 dstWidth, uint8* srcBitmap, int32 srcWidth, int32 srcHeight)
	{
		dstBitmap += (x + (y * dstWidth));

		for (int i = 0; i < srcHeight; ++i)
		{
			memcpy(dstBitmap, srcBitmap, srcWidth);
			srcBitmap += srcWidth;
			dstBitmap += dstWidth;
		}
	}

	Font* Font::load(const std::string& family, Style style, uint32 size, Glyph* glyphs, int32 glyphCount, Texture* texture, Font::Format format)
	{
		if (!glyphs || !texture)
			return nullptr;

		SpriteImmediateRenderer* renderer = new SpriteImmediateRenderer(128, texture);

		//Sampler::Parameters samplerParams = batch->getSampler()->getParameters();
		//samplerParams.wrapS(Texture::CLAMP).wrapT(Texture::CLAMP).min(Texture::LINEAR_MIPMAP_LINEAR).mag(Texture::LINEAR);
		//batch->setSampler(new Sampler(samplerParams));

		texture->reference();

		Font* font = new Font();
		font->_format = format;
		font->_family = family;
		font->_style = style;
		font->_size = size;
		font->_texture = texture;
		font->_renderer = renderer;

		font->_glyphs = new Glyph[glyphCount];
		memcpy(font->_glyphs, glyphs, sizeof(Glyph) * glyphCount);
		font->_glyphCount = glyphCount;

		return font;
	}

	void Font::getMeasurementInfo(const std::string& text, const Rectf& area, uint32 size, Justify justify, bool wrap, bool rightToLeft,
		std::vector<int32>& xPositions, int32& yPosition, std::vector<uint32>& lineLengths)
	{
		if (_size == 0)
			return;

		if (size == 0)
			size = _size;

		float scale = (float)size / _size;

		Justify vAlign = static_cast<Justify>(justify & 0xF0);
		if (vAlign == 0)
		{
			vAlign = AlignTop;
		}

		Justify hAlign = static_cast<Justify>(justify & 0x0F);
		if (hAlign == 0)
		{
			hAlign = AlignLeft;
		}

		const char* token = text.c_str();
		const float areaHeight = area.getSize().y - size;

		if (vAlign != AlignTop || hAlign != AlignLeft || rightToLeft)
		{
			int lineWidth = 0;
			int delimWidth = 0;

			if (wrap)
			{
				bool reachedEOF = false;
				unsigned int lineLength = 0;
				while (token[0] != 0)
				{
					unsigned int tokenWidth = 0;

					char delimiter = token[0];
					while (delimiter == ' ' ||
						delimiter == '\t' ||
						delimiter == '\r' ||
						delimiter == '\n' ||
						delimiter == 0)
					{
						switch (delimiter)
						{
						case ' ':
							delimWidth += _glyphs[0].advance;
							lineLength++;
							break;
						case '\r':
						case '\n':
							yPosition += size;

							if (lineWidth > 0)
							{
								addLineInfo(area, lineWidth, lineLength, hAlign, xPositions, lineLengths, rightToLeft);
							}

							lineWidth = 0;
							lineLength = 0;
							delimWidth = 0;
							break;
						case '\t':
							delimWidth += _glyphs[0].advance * 4;
							lineLength++;
							break;
						case 0:
							reachedEOF = true;
							break;
						}

						if (reachedEOF)
						{
							break;
						}

						token++;
						delimiter = token[0];
					}

					if (reachedEOF || token == NULL)
					{
						break;
					}

					unsigned int tokenLength = (unsigned int)strcspn(token, " \r\n\t");
					tokenWidth += getTokenWidth(token, tokenLength, size, scale);

					if (lineWidth + tokenWidth + delimWidth > area.getSize().x)
					{
						yPosition += size;

						if (lineLength)
						{
							addLineInfo(area, lineWidth, lineLength - 1, hAlign, xPositions, lineLengths, rightToLeft);
						}
						else
						{
							addLineInfo(area, lineWidth, tokenLength, hAlign, xPositions, lineLengths, rightToLeft);
						}

						lineWidth = 0;
						lineLength = 0;
						delimWidth = 0;
					}
					else
					{
						lineWidth += delimWidth;
						delimWidth = 0;
					}

					lineWidth += tokenWidth;
					lineLength += tokenLength;
					token += tokenLength;
				}

				int textHeight = yPosition - area.getTop();
				int vWhiteSpace = areaHeight - textHeight;
				if (vAlign == AlignVerticalCenter)
				{
					yPosition = area.getTop() + vWhiteSpace / 2;
				}
				else if (vAlign == AlignBottom)
				{
					yPosition = area.getTop() + vWhiteSpace;
				}

				addLineInfo(area, lineWidth, lineLength, hAlign, xPositions, lineLengths, rightToLeft);
			}
			else
			{
				while (token[0] != 0)
				{
					char delimiter = token[0];
					while (delimiter == '\n')
					{
						yPosition += size;
						++token;
						delimiter = token[0];
					}

					unsigned int tokenLength = (unsigned int)strcspn(token, "\n");
					if (tokenLength == 0)
					{
						tokenLength = (unsigned int)strlen(token);
					}

					int lineWidth = getTokenWidth(token, tokenLength, size, scale);
					addLineInfo(area, lineWidth, tokenLength, hAlign, xPositions, lineLengths, rightToLeft);

					token += tokenLength;
				}

				int textHeight = yPosition - area.getTop();
				int vWhiteSpace = areaHeight - textHeight;
				if (vAlign == AlignVerticalCenter)
				{
					yPosition = area.getTop() + vWhiteSpace / 2;
				}
				else if (vAlign == AlignBottom)
				{
					yPosition = area.getTop() + vWhiteSpace;
				}
			}

			if (vAlign == AlignTop)
			{
				yPosition = area.getTop();
			}
		}
	}

	int32 Font::getIndexOrLocation(const std::string& text, const Rectf& clip, uint32 size, const Vector2f& inLocation, Vector2f& outLocation,
		const int32 destIndex, Justify justify, bool wrap, bool rightToLeft)
	{
		if (_size == 0)
			return - 1;

		if (size == 0)
		{
			size = _size;
		}
		else
		{
			Font* f = findClosestSize(size);
			if (f != this)
			{
				return f->getIndexOrLocation(text, clip, size, inLocation, outLocation, destIndex, justify, wrap, rightToLeft);
			}
		}

		unsigned int charIndex = 0;

		float scale = (float)size / _size;
		int spacing = (int)(size * _spacing);
		int yPos = clip.getTop();
		const float areaHeight = clip.getSize().y - size;
		std::vector<int> xPositions;
		std::vector<unsigned int> lineLengths;

		getMeasurementInfo(text, clip, size, justify, wrap, rightToLeft, xPositions, yPos, lineLengths);

		int xPos = clip.getLeft();
		std::vector<int>::const_iterator xPositionsIt = xPositions.begin();
		if (xPositionsIt != xPositions.end())
		{
			xPos = *xPositionsIt++;
		}

		const char* token = text.c_str();

		int iteration = 1;
		unsigned int lineLength;
		unsigned int currentLineLength = 0;
		const char* lineStart = "";
		std::vector<unsigned int>::const_iterator lineLengthsIt;
		if (rightToLeft)
		{
			lineStart = token;
			lineLengthsIt = lineLengths.begin();
			lineLength = *lineLengthsIt++;
			token += lineLength - 1;
			iteration = -1;
		}

		while (token[0] != 0)
		{
			unsigned int delimLength = 0;
			int result;
			if (destIndex == -1)
			{
				result = handleDelimiters(&token, size, iteration, clip.getLeft(), &xPos, &yPos, &delimLength, &xPositionsIt, xPositions.end(), &charIndex, &inLocation);
			}
			else
			{
				result = handleDelimiters(&token, size, iteration, clip.getLeft(), &xPos, &yPos, &delimLength, &xPositionsIt, xPositions.end(), &charIndex, NULL, charIndex, destIndex);
			}

			currentLineLength += delimLength;
			if (result == 0 || result == 2)
			{
				outLocation.x = xPos;
				outLocation.y = yPos;
				return charIndex;
			}

			if (destIndex == (int)charIndex ||
				(destIndex == -1 &&
					inLocation.x >= xPos && inLocation.x < xPos + spacing &&
					inLocation.y >= yPos && inLocation.y < yPos + size))
			{
				outLocation.x = xPos;
				outLocation.y = yPos;
				return charIndex;
			}

			bool truncated = false;
			unsigned int tokenLength;
			unsigned int tokenWidth;
			unsigned int startIndex;
			if (rightToLeft)
			{
				tokenLength = getReversedTokenLength(token, text.c_str());
				currentLineLength += tokenLength;
				charIndex += tokenLength;
				token -= (tokenLength - 1);
				tokenWidth = getTokenWidth(token, tokenLength, size, scale);
				iteration = -1;
				startIndex = tokenLength - 1;
			}
			else
			{
				tokenLength = (unsigned int)strcspn(token, " \r\n\t");
				tokenWidth = getTokenWidth(token, tokenLength, size, scale);
				iteration = 1;
				startIndex = 0;
			}

			if (wrap && (xPos + (int)tokenWidth > clip.getLeft() + clip.getSize().y || (rightToLeft && currentLineLength > lineLength)))
			{
				yPos += size;
				currentLineLength = tokenLength;

				if (xPositionsIt != xPositions.end())
				{
					xPos = *xPositionsIt++;
				}
				else
				{
					xPos = clip.getLeft();
				}
			}

			if (yPos > clip.getTop() + areaHeight)
			{
				break;
			}

			AE_ASSERT(_glyphs);
			for (int i = startIndex; i < (int)tokenLength && i >= 0; i += iteration)
			{
				char c = token[i];
				int glyphIndex = c - 32;

				if (glyphIndex >= 0 && glyphIndex < (int)_glyphCount)
				{
					Glyph& g = _glyphs[glyphIndex];

					if (xPos + (int)(g.advance*scale) > clip.getLeft() + clip.getSize().x)
					{
						truncated = true;
						break;
					}

					if (destIndex == (int)charIndex ||
						(destIndex == -1 &&
							inLocation.x >= xPos && inLocation.x < floor(xPos + g.advance*scale + spacing) &&
							inLocation.y >= yPos && inLocation.y < yPos + size))
					{
						outLocation.x = xPos;
						outLocation.y = yPos;
						return charIndex;
					}

					xPos += floor(g.advance*scale + spacing);
					charIndex++;
				}
			}

			if (!truncated)
			{
				if (rightToLeft)
				{
					if (token == lineStart)
					{
						token += lineLength;

						if (!handleDelimiters(&token, size, 1, clip.getLeft(), &xPos, &yPos, &currentLineLength, &xPositionsIt, xPositions.end()))
						{
							break;
						}
						charIndex += currentLineLength;

						if (lineLengthsIt != lineLengths.end())
						{
							lineLength = *lineLengthsIt++;
						}
						lineStart = token;
						token += lineLength - 1;
						charIndex += tokenLength;
					}
					else
					{
						token--;
					}
				}
				else
				{
					token += tokenLength;
				}
			}
			else
			{
				if (rightToLeft)
				{
					token = lineStart + lineLength;

					if (!handleDelimiters(&token, size, 1, clip.getLeft(), &xPos, &yPos, &currentLineLength, &xPositionsIt, xPositions.end()))
					{
						break;
					}

					if (lineLengthsIt != lineLengths.end())
					{
						lineLength = *lineLengthsIt++;
					}
					lineStart = token;
					token += lineLength - 1;
				}
				else
				{
					unsigned int tokenLength = (unsigned int)strcspn(token, "\n");

					if (tokenLength > 0)
					{
						token += tokenLength;
						charIndex += tokenLength;
					}
				}
			}
		}


		if (destIndex == (int)charIndex ||
			(destIndex == -1 &&
				inLocation.x >= xPos && inLocation.x < xPos + spacing &&
				inLocation.y >= yPos && inLocation.y < yPos + size))
		{
			outLocation.x = xPos;
			outLocation.y = yPos;
			return charIndex;
		}

		return -1;
	}

	uint32 Font::getTokenWidth(const char* token, unsigned length, uint32 size, float scale)
	{
		AE_ASSERT(token);
		AE_ASSERT(_glyphs);

		if (size == 0)
			size = _size;

		int spacing = (int)(size * _spacing);

		unsigned int tokenWidth = 0;
		for (unsigned int i = 0; i < length; ++i)
		{
			char c = token[i];
			switch (c)
			{
			case ' ':
				tokenWidth += _glyphs[0].advance;
				break;
			case '\t':
				tokenWidth += _glyphs[0].advance * 4;
				break;
			default:
				int glyphIndex = c - 32;
				if (glyphIndex >= 0 && glyphIndex < (int)_glyphCount)
				{
					Glyph& g = _glyphs[glyphIndex];
					tokenWidth += floor(g.advance * scale + spacing);
				}
				break;
			}
		}

		return tokenWidth;
	}

	uint32 Font::getReversedTokenLength(const char* token, const char* bufStart)
	{
		AE_ASSERT(token);
		AE_ASSERT(bufStart);

		const char* cursor = token;
		char c = cursor[0];
		unsigned int length = 0;

		while (cursor != bufStart && c != ' ' && c != '\r' && c != '\n' && c != '\t')
		{
			length++;
			cursor--;
			c = cursor[0];
		}

		if (cursor == bufStart)
		{
			length++;
		}

		return length;
	}

	int32 Font::handleDelimiters(const char** token, const uint32 size, const int32 iteration, const int32 areaX, int* xPos, int* yPos, uint32* lineLength,
		std::vector<int32>::const_iterator* xPositionsIt, std::vector<int32>::const_iterator xPositionsEnd, uint32* charIndex,
		const Vector2f* stopAtPosition, const int32 currentIndex, const int32 destIndex)
	{
		AE_ASSERT(token);
		AE_ASSERT(*token);
		AE_ASSERT(xPos);
		AE_ASSERT(yPos);
		AE_ASSERT(lineLength);
		AE_ASSERT(xPositionsIt);

		char delimiter = *token[0];
		bool nextLine = true;
		while (delimiter == ' ' ||
			delimiter == '\t' ||
			delimiter == '\r' ||
			delimiter == '\n' ||
			delimiter == 0)
		{
			if ((stopAtPosition &&
				stopAtPosition->x >= *xPos && stopAtPosition->x < *xPos + ((int)size >> 1) &&
				stopAtPosition->y >= *yPos && stopAtPosition->y < *yPos + (int)size) ||
				(currentIndex >= 0 && destIndex >= 0 && currentIndex + (int)*lineLength == destIndex))
			{
				return 2;
			}

			switch (delimiter)
			{
			case ' ':
				*xPos += _glyphs[0].advance;
				(*lineLength)++;
				if (charIndex)
				{
					(*charIndex)++;
				}
				break;
			case '\r':
			case '\n':
				*yPos += size;

				if (nextLine)
				{
					if (*xPositionsIt != xPositionsEnd)
					{
						*xPos = **xPositionsIt;
						(*xPositionsIt)++;
					}
					else
					{
						*xPos = areaX;
					}
					nextLine = false;
					*lineLength = 0;
					if (charIndex)
					{
						(*charIndex)++;
					}
				}
				break;
			case '\t':
				*xPos += _glyphs[0].advance * 4;
				(*lineLength)++;
				if (charIndex)
				{
					(*charIndex)++;
				}
				break;
			case 0:
				return 0;
			}

			*token += iteration;
			delimiter = *token[0];
		}

		return 1;
	}

	void Font::addLineInfo(const Rectf& area, int32 lineWidth, int32 lineLength, Justify hAlign,
		std::vector<int>& xPositions, std::vector<uint32>& lineLengths, bool rightToLeft)
	{
		int32 hWhitespace = area.getSize().x - lineWidth;
		if (hAlign == AlignHorizontalCenter)
		{
			xPositions.push_back(area.getLeft() + hWhitespace / 2);
		}
		else if (hAlign == AlignRight)
		{
			xPositions.push_back(area.getLeft() + hWhitespace);
		}

		if (rightToLeft)
		{
			lineLengths.push_back(lineLength);
		}
	}

	Font* Font::findClosestSize(int32 size)
	{
		if (size == (int)_size)
			return this;

		int diff = abs(size - (int)_size);
		Font* closest = this;
		for (size_t i = 0, count = _sizes.size(); i < count; ++i)
		{
			Font* f = _sizes[i];
			int d = abs(size - (int)f->_size);
			if (d < diff || (d == diff && f->_size > closest->_size))
			{
				diff = d;
				closest = f;
			}
		}

		return closest;
	}
}