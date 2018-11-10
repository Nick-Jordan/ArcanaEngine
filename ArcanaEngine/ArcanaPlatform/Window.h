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

	class WindowRenderer
	{
	public:

		virtual void render() = 0;

		virtual void setVerticalSync(bool enabled) = 0;
	};

	class ARCANA_PLATFORM_API Application;

	class ARCANA_PLATFORM_API Window
	{
	public:

		Window();

		Window(const WindowDefinition& definition);

		Window(WindowHandle handle);

		//Window(const Window& window);

		virtual ~Window();


		Vector2i getPosition() const;

		void setPosition(const Vector2i& position);

		Vector2i getSize() const;

		float getAspectRatio() const;

		void setSize(const Vector2i& size);

		void setTitle(const std::string& title);

		void setIcon(unsigned int width, unsigned int height, const uint8* pixels);

		void setVisible(bool visible);

		void requestFocus();

		bool hasFocus() const;


		void initialize();

		bool create(const WindowDefinition& definition);

		void close();

		bool destroy();

		bool isOpen() const;

		void render();

		void setVerticalSync(bool enabled);

		HWND getWindowHandle() const;

		const WindowDefinition& getWindowDefinition() const;

		const WindowContext* getWindowContext() const;

		void setDefinition(WindowDefinition* definition, bool reinitialize = false);

		void setCursor(Cursor* cursor);

		Cursor* getCursor() const;

		void setParent(Application* parent);
		
		Application* getParent() const;


		bool pollMessage(Message& msg);

		bool waitMessage(Message& msg);

		bool filterEvent(const Event& event);


		//Window& operator=(const Window& window);

	private:

		WindowContext* _windowContext;

		const WindowDefinition* _definition;

		int32 _running;

		Application* _parent;

	public:

		WindowRenderer* _renderer;
	};

}

#endif // !WINDOW_H_

