#ifndef WINDOW_H_
#define WINDOW_H_

#include "PlatformDefines.h"

#include "WindowDefinition.h"
#include "WindowContext.h"
#include "ArcanaLog.h"

namespace Arcana
{
	REGISTER_CATEGORY(WindowLog, none)


	class ARCANA_PLATFORM_API Window
	{
	public:

		Window();

		Window(const WindowDefinition& definition); //OpenGL context settings as well?

		Window(WindowHandle handle); //OpenGL context settings as well?

		virtual ~Window();


		void initialize();

		bool create(const WindowDefinition& definition);

		bool destroy();

		bool isOpen() const;

		const uint32& getWidth() const;

		const uint32& getHeight() const;

		const WindowDefinition& getWindowDefinition() const;

		void setDefinition(WindowDefinition* definition, bool reinitialize = false);

	private:

		WindowContext* _windowContext;

		uint32 _width;
		uint32 _height;

		const WindowDefinition* _definition;
	};

}

#endif // !WINDOW_H_

