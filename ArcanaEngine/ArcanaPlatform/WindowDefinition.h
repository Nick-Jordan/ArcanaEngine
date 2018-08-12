#ifndef WINDOW_DEFINITION_H_
#define WINDOW_DEFINITION_H_

#include "PlatformDefines.h"

#include "Types.h"

#include <string>

namespace Arcana
{

	enum ARCANA_PLATFORM_API Style
	{
		None = 0,
		Titlebar = 1 << 0,
		Close = 1 << 2,
		Resize = 1 << 3,
		Fullscreen = 1 << 4,

		Default = Titlebar | Resize | Close
	};

	class ARCANA_PLATFORM_API WindowDefinition
	{
	public:

		WindowDefinition();

		virtual ~WindowDefinition();


		const std::string& getTitle() const;

		void setTitle(const std::string& title);

		const uint32& getWidth() const;

		void setWidth(uint32 width);

		const uint32& getHeight() const;

		void setHeight(uint32 height);

		const uint32& getBitsPerPixel() const;

		void setBitsPerPixel(uint32 bitsPerPixel);

		Style getStyle() const;

		void setStyle(Style style);

	private:

		std::string _title;

		uint32 _width;
		uint32 _height;

		uint32 _bitsPerPixel;

		Style _style;
	};
}

#endif