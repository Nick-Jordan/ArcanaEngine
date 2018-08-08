//test
#define ARCANA_PLATFORM_WINDOWS

#ifdef ARCANA_PLATFORM_WINDOWS

#ifndef WINDOWS_WINDOW_CONTEXT_H_
#define WINDOWS_WINDOW_CONTEXT_H_

#include "PlatformDefines.h"

#include "WindowHandle.h"
#include "WindowContext.h"
#include "WindowsWindowDefinition.h"

#include "Key.h"

namespace Arcana
{

	class WindowsCursor;

	class ARCANA_PLATFORM_API WindowsWindowContext : public WindowContext
	{
	public:
		
		static uint32 NumWin32WindowContexts;

		WindowsWindowContext();

		virtual ~WindowsWindowContext();

		virtual bool create(const WindowDefinition& definition) override;

		virtual bool destroy() override;


		virtual Vector2i getPosition() const override;

		virtual void setPosition(const Vector2i& position) override;

		virtual Vector2i getSize() const override;

		virtual void setSize(const Vector2i& size) override;

		virtual void setTitle(const std::string& title) override;

		virtual void setIcon(unsigned int width, unsigned int height, const uint8* pixels) override;

		virtual void setVisible(bool visible) override;

		virtual void requestFocus() override;

		virtual bool hasFocus() const override;

		virtual WindowHandle getWindowHandle() const override;

		virtual void setCursor(Cursor* cursor) override;

		virtual Cursor* getCursor() const override;

		virtual void processEvents() const override;

		virtual EventProcessor& getEventProcessor() override;

		HINSTANCE getInstance() const;

	private:

		bool createWindow(const WindowsWindowDefinition& def);

		void registerClass(const WindowsWindowDefinition &def);

		void processEvent(UINT message, WPARAM wParam, LPARAM lParam);

		static LRESULT CALLBACK globalWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		static Key windowsKeyConversion(WPARAM key, LPARAM flags);


		const wchar_t* _className;

		HINSTANCE _instance;
		HWND _windowHandle;
		LONG_PTR _callback;
		WindowsCursor* _cursor;
		HICON _icon;

		EventProcessor _eventProcessor;
	};

}

#endif // !WINDOWS_WINDOW_CONTEXT_H_

#endif // ARCANA_PLATFORM_WINDOWS

