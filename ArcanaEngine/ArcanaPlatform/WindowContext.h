#ifndef WINDOW_CONTEXT_H_
#define WINDOW_CONTEXT_H_

#include "PlatformDefines.h"

#include "WindowHandle.h"
#include "WindowDefinition.h"
#include "EventProcessor.h"
#include "Types.h"
#include "Cursor.h"

#include "ArcanaLog.h"

#include "Vector2.h"

namespace Arcana
{

	class ARCANA_PLATFORM_API WindowContext
	{
	public:

		WindowContext() { };

		virtual ~WindowContext() { };

		virtual bool create(const WindowDefinition& definition) = 0;

		virtual bool destroy() = 0;


		virtual Vector2i getPosition() const = 0;

		virtual void setPosition(const Vector2i& position) = 0;

		virtual Vector2i getSize() const = 0;

		virtual void setSize(const Vector2i& size) = 0;

		virtual void setTitle(const std::string& title) = 0;

		virtual void setIcon(unsigned int width, unsigned int height, const uint8* pixels) = 0;

		virtual void setVisible(bool visible) = 0;

		virtual void requestFocus() = 0;

		virtual bool hasFocus() const = 0;

		virtual void setMouseCursorVisible(bool visible) = 0;

		virtual void setLockMouseCursor(bool lock, Recti rect) = 0;

		virtual void repeatKeyEvents(bool repeat) = 0;

		virtual WindowHandle getWindowHandle() const = 0;

		virtual void setCursor(const CursorContext& cursor) = 0;

		virtual void processEvents() const = 0;

		virtual void processControllerEvents() = 0;

		virtual EventProcessor& getEventProcessor() = 0;

	};
}

#endif // !WINDOW_CONTEXT_H_

