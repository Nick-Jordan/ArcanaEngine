#ifndef WINDOW_H_
#define WINDOW_H_

#include "PlatformDefines.h"

#include "WindowDefinition.h"
#include "WindowContext.h"
#include "WindowHandle.h"
#include "ArcanaLog.h"
#include "EventProcessor.h"

#include "Cursor.h"

namespace Arcana
{
	REGISTER_CATEGORY_ENGINE(ARCANA_PLATFORM_API, WindowLog, none)


	class ARCANA_PLATFORM_API Window
	{
	public:

		Window();

		Window(const WindowDefinition& definition); //OpenGL context settings as well?

		Window(WindowHandle handle); //OpenGL context settings as well?

		virtual ~Window();


		Vector2i getPosition() const;

		void setPosition(const Vector2i& position);

		Vector2i getSize() const;

		void setSize(const Vector2i& size);

		void setTitle(const std::string& title);

		void setIcon(unsigned int width, unsigned int height, const uint8* pixels);

		void setVisible(bool visible);

		void requestFocus();

		bool hasFocus() const;


		void initialize();

		bool create(const WindowDefinition& definition);

		bool destroy();

		bool isOpen() const;

		HWND getWindowHandle() const;

		const WindowDefinition& getWindowDefinition() const;

		void setDefinition(WindowDefinition* definition, bool reinitialize = false);

		void setCursor(Cursor* cursor);

		Cursor* getCursor() const;


		bool pollMessage(Message& msg);

		bool waitMessage(Message& msg);

		bool filterEvent(const Event& event);

	private:

		WindowContext* _windowContext;

		const WindowDefinition* _definition;
	};

}

#endif // !WINDOW_H_

