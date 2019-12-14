#ifndef FONT_H_
#define FONT_H_

#include "GraphicsDefines.h"

#include "SpriteImmediateRenderer.h"
#include "Object.h"
#include "GlobalObjectID.h"
#include "ArcanaLog.h"

namespace Arcana
{
	REGISTER_CATEGORY_ENGINE(ARCANA_GRAPHICS_API, FontLog, none);


	class ARCANA_GRAPHICS_API Font : public Object
	{

	public:

		enum Style
		{
			Plain = 0,
			Bold = 1,
			Italic = 2,
			BoldItalic = 4
		};

		enum Justify
		{
			AlignLeft = 0x01,
			AlignHorizontalCenter = 0x02,
			AlignRight = 0x04,
			AlignTop = 0x10,
			AlignVerticalCenter = 0x20,
			AlignBottom = 0x40,
			AlignTopLeft = AlignTop | AlignLeft,
			AlignVerticalCenterLeft = AlignVerticalCenter | AlignLeft,
			AlignBottomLeft = AlignBottom | AlignLeft,
			AlignTopHorizontalCenter = AlignTop | AlignHorizontalCenter,
			AlignVerticalHorizontalCenter = AlignVerticalCenter | AlignHorizontalCenter,
			AlignBottomHorizontalCenter = AlignBottom | AlignHorizontalCenter,
			AlignTopRight = AlignTop | AlignRight,
			AlignVerticalCenterRight = AlignVerticalCenter | AlignRight,
			AlignBottomRight = AlignBottom | AlignRight
		};

		enum Format
		{
			Bitmap = 0,
			DistanceField = 1
		};

		~Font();

		static Font* load(const std::string& path, const std::string& id, std::vector<uint32>& fontSizes);

		static Font* load(const std::string& path, const char* id = nullptr);

		static Font* load(uint32 mainSize, const std::string& path, const char* id = nullptr);


		uint32 getSize(uint32 index = 0) const;

		uint32 getSizeCount() const;

		Format getFormat();

		bool isCharacterSupported(int32 character) const;

		void start();

		void drawText(const std::string& text, int32 x, int32 y, const Color& color, uint32 size = 0,
			bool rightToLeft = false, float z = 0.0f);

		void drawText(const std::string& text, int32 x, int32 y, float red, float green, float blue, float alpha, uint32 size = 0,
			bool rightToLeft = false, float z = 0.0f);

		void drawText(const std::string& text, const Rectf& area, const Color& color, uint32 size = 0,
			Justify justify = AlignTopLeft, bool wrap = true, bool rightToLeft = false,
			const Rectf& clip = Rectf(0, 0, 0, 0), float z = 0.0f);

		void finish();

		void measureText(const std::string& text, uint32 size, uint32& widthOut, uint32& heightOut);

		void measureText(const std::string&  text, const Rectf& clip, uint32 size, Rectf& out,
			Justify justify = AlignTopLeft, bool wrap = true, bool ignoreClip = false);

		float getCharacterSpacing() const;

		void setCharacterSpacing(float spacing);

		int32 getIndexAtLocation(const std::string&  text, const Rectf& clip, uint32 size, const Vector2f& inLocation,
			Vector2f& outLocation, Justify justify = AlignTopLeft, bool wrap = true, bool rightToLeft = false);

		void getLocationAtIndex(const std::string&  text, const Rectf& clip, uint32 size, Vector2f& outLocation,
			const uint32 destIndex, Justify justify = AlignTopLeft, bool wrap = true,
			bool rightToLeft = false);

		SpriteImmediateRenderer* getSpriteImmediateRenderer(uint32 size = 0) const;

		Texture* getGlyphTexture() const;

	private:

		static std::vector<Font*> __fontCache;

	private:

		class Glyph
		{
		public:

			uint32 code;
			uint32 width;
			int bearingX;
			uint32 advance;
			float uvs[4];
		};

		Font();

		Font(const Font& copy);

		Font& operator=(const Font&);

		static void drawBitmap(uint8* dstBitmap, int32 x, int32 y, int32 dstWidth, uint8* srcBitmap, int32 srcWidth, int32 srcHeight);

		static Font* load(const std::string& family, Style style, uint32 size, Glyph* glyphs, int32 glyphCount, Texture* texture, Font::Format format);

		void getMeasurementInfo(const std::string& text, const Rectf& area, uint32 size, Justify justify, bool wrap, bool rightToLeft,
			std::vector<int32>& xPositions, int32& yPosition, std::vector<uint32>& lineLengths);

		int32 getIndexOrLocation(const std::string& text, const Rectf& clip, uint32 size, const Vector2f& inLocation, Vector2f& outLocation,
			const int32 destIndex = -1, Justify justify = AlignTopLeft, bool wrap = true, bool rightToLeft = false);

		uint32 getTokenWidth(const char* token, unsigned length, uint32 size, float scale);

		uint32 getReversedTokenLength(const char* token, const char* bufStart);

		int32 handleDelimiters(const char** token, const uint32 size, const int32 iteration, const int32 areaX, int* xPos, int* yPos, uint32* lineLength,
			std::vector<int32>::const_iterator* xPositionsIt, std::vector<int32>::const_iterator xPositionsEnd, uint32* charIndex = nullptr,
			const Vector2f* stopAtPosition = nullptr, const int32 currentIndex = -1, const int32 destIndex = -1);

		void addLineInfo(const Rectf& area, int32 lineWidth, int32 lineLength, Justify hAlign,
			std::vector<int>& xPositions, std::vector<uint32>& lineLengths, bool rightToLeft);

		Font* findClosestSize(int32 size);

		Format _format;
		std::string _path;
		GlobalObjectID _id;
		std::string _family;
		Style _style;
		uint32 _size;
		std::vector<Font*> _sizes;
		float _spacing;
		Glyph* _glyphs;
		uint32 _glyphCount;
		Texture* _texture;
		SpriteImmediateRenderer* _renderer;
		Rectf _viewport;
	};

}

#endif