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
#include "Controller.h"
#include "ControllerContext.h"
#include "ControllerManager.h"
#include "CursorContext.h"

#include <dbt.h>


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

		virtual void setMouseCursorVisible(bool visible) override;

		virtual void setLockMouseCursor(bool lock, Recti rect) override;

		virtual void repeatKeyEvents(bool repeat) override;

		virtual WindowHandle getWindowHandle() const override;

		virtual void setCursor(const CursorContext& cursor) override;

		virtual void setFullscreen(bool fullscreen, bool forMetro) override;

		virtual void processEvents() const override;

		virtual void processControllerEvents() override;

		virtual EventProcessor& getEventProcessor() override;

		HINSTANCE getInstance() const;

	private:

		bool createWindow(const WindowsWindowDefinition& def);

		void trackMouse(bool track);

		void lockCursor(bool lock, Recti rect = Recti(-1, -1, 0, 0));

		void registerClass(const WindowsWindowDefinition &def);

		void processEvent(UINT message, WPARAM wParam, LPARAM lParam);

		static LRESULT CALLBACK globalWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


	private:

		struct SavedWindowInfo
		{
			bool maximized;
			LONG style;
			LONG ex_style;
			RECT window_rect;
		};

	private:

		const wchar_t* _className;

		HINSTANCE _instance;
		HWND _windowHandle;
		LONG_PTR _callback;
		HCURSOR _cursor;
		HICON _icon;

		bool _cursorLocked;
		bool _repeatKeyEvents;
		bool _cursorVisible;
		bool _mouseContained;
		bool _fullscreen;

		SavedWindowInfo _savedWindowInfo;

		EventProcessor _eventProcessor;

		ControllerState _controllerStates[Controller::MaxControllers];
		float _previousAxes[Controller::MaxControllers][Controller::FloatAxisCount];
	};

}

#endif // !WINDOWS_WINDOW_CONTEXT_H_

#endif // ARCANA_PLATFORM_WINDOWS

