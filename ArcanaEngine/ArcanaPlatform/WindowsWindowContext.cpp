#include "WindowsWindowContext.h"

#ifdef ARCANA_PLATFORM_WINDOWS

#include "WindowsWindowDefinition.h"
#include "WindowsApplicationContext.h"
#include "Window.h"
#include "WindowsCursor.h"

#include "KeyEvent.h"

#include <vector>

namespace Arcana
{

	uint32 WindowsWindowContext::NumWin32WindowContexts = 0;
	//const wchar_t* WindowsWindowContext::className = L"WindowClass";

	WindowsWindowContext::WindowsWindowContext() : WindowContext()
	{
	}


	WindowsWindowContext::~WindowsWindowContext()
	{
	}

	bool WindowsWindowContext::create(const WindowDefinition& definition)
	{
		LOG(Info, WindowLog, "Creating Windows Window Context...");

		try {
			const WindowsWindowDefinition &def = dynamic_cast<const WindowsWindowDefinition&>(definition);

			return createWindow(def);
		}
		catch (const std::bad_cast& e) {
			LOGF(Error, WindowLog, "Bad Cast");
			return false;
		}
	}

	bool WindowsWindowContext::destroy()
	{
		if (_icon)
			DestroyIcon(_icon);

		if (!_callback)
		{
			if (_windowHandle)
			{
				DestroyWindow(_windowHandle);
			}

			NumWin32WindowContexts--;

			if (NumWin32WindowContexts == 0)
			{
				LOG(Info, WindowLog, "Unregistering Window Class");
				UnregisterClassW(_className, _instance);
			}
		}
		else
		{
			SetWindowLongPtrW(_windowHandle, GWLP_WNDPROC, _callback);
		}

		return true;
	}


	Vector2i WindowsWindowContext::getPosition() const
	{
		RECT rect;
		GetWindowRect(_windowHandle, &rect);

		return Vector2i(rect.left, rect.top);
	}

	void WindowsWindowContext::setPosition(const Vector2i& position)
	{
		SetWindowPos(_windowHandle, NULL, position.x, position.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}

	Vector2i WindowsWindowContext::getSize() const
	{
		RECT rect;
		GetClientRect(_windowHandle, &rect);

		return Vector2i(rect.right - rect.left, rect.bottom - rect.top);
	}

	void WindowsWindowContext::setSize(const Vector2i& size)
	{
		RECT rectangle = { 0, 0, size.x, size.y };
		AdjustWindowRect(&rectangle, GetWindowLong(_windowHandle, GWL_STYLE), false);
		int width = rectangle.right - rectangle.left;
		int height = rectangle.bottom - rectangle.top;

		SetWindowPos(_windowHandle, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
	}

	void WindowsWindowContext::setTitle(const std::string& title)
	{
		std::wstring stemp = std::wstring(title.begin(), title.end());
		LPCWSTR windowTitle = stemp.c_str();

		SetWindowTextW(_windowHandle, windowTitle);
	}

	void WindowsWindowContext::setIcon(unsigned int width, unsigned int height, const uint8* pixels)
	{
		if (_icon)
			DestroyIcon(_icon);

		std::vector<uint8> iconPixels(width * height * 4);
		for (std::size_t i = 0; i < iconPixels.size() / 4; ++i)
		{
			iconPixels[i * 4 + 0] = pixels[i * 4 + 2];
			iconPixels[i * 4 + 1] = pixels[i * 4 + 1];
			iconPixels[i * 4 + 2] = pixels[i * 4 + 0];
			iconPixels[i * 4 + 3] = pixels[i * 4 + 3];
		}

		_icon = CreateIcon(_instance, width, height, 1, 32, NULL, &iconPixels[0]);

		if (_icon)
		{
			SendMessageW(_windowHandle, WM_SETICON, ICON_BIG, (LPARAM)_icon);
			SendMessageW(_windowHandle, WM_SETICON, ICON_SMALL, (LPARAM)_icon);
		}
		else
		{
			LOG(Error, WindowLog, "Failed to set the window's icon.");
		}
	}

	void WindowsWindowContext::setVisible(bool visible)
	{
		ShowWindow(_windowHandle, visible ? SW_SHOW : SW_HIDE);
	}

	void WindowsWindowContext::requestFocus()
	{
		DWORD pid = GetWindowThreadProcessId(_windowHandle, NULL);
		DWORD foregroundPid = GetWindowThreadProcessId(GetForegroundWindow(), NULL);

		if (pid == foregroundPid)
		{
			SetForegroundWindow(_windowHandle);
		}
		else
		{
			FLASHWINFO info;
			info.cbSize = sizeof(info);
			info.hwnd = _windowHandle;
			info.dwFlags = FLASHW_TRAY;
			info.dwTimeout = 0;
			info.uCount = 3;

			FlashWindowEx(&info);
		}
	}

	bool WindowsWindowContext::hasFocus() const
	{
		return _windowHandle == GetForegroundWindow();
	}

	WindowHandle WindowsWindowContext::getWindowHandle() const
	{
		return _windowHandle;
	}

	void WindowsWindowContext::setCursor(Cursor* cursor)
	{
		/*WindowsCursor* wc = dynamic_cast<WindowsCursor*>(cursor);
		if (wc)
		{
			_cursor = wc;
			SetCursor(_cursor->getHCursor());
		}*/
	}

	Cursor* WindowsWindowContext::getCursor() const
	{
		return _cursor;
	}

	void WindowsWindowContext::processEvents() const
	{
		if (!_callback)
		{
			MSG message;
			while (PeekMessageW(&message, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessageW(&message);
			}
		}
	}

	EventProcessor& WindowsWindowContext::getEventProcessor()
	{
		return _eventProcessor;
	}

	HINSTANCE WindowsWindowContext::getInstance() const
	{
		return _instance;
	}

	bool WindowsWindowContext::createWindow(const WindowsWindowDefinition& def)
	{
		_instance = def.getInstance();
		_className = def.getWindowClass();

		_callback = 0;
		_icon = 0;

		if (NumWin32WindowContexts == 0)
		{
			registerClass(def);
		}

		NumWin32WindowContexts++;

		LOG(Debug, WindowLog, "Instance: " + std::to_string((int)_instance));

		HDC screenDC = GetDC(NULL);
		int left = (GetDeviceCaps(screenDC, HORZRES) - static_cast<int>(def.getWidth())) / 2;
		int top = (GetDeviceCaps(screenDC, VERTRES) - static_cast<int>(def.getHeight())) / 2;
		int width = def.getWidth();
		int height = def.getHeight();
		ReleaseDC(NULL, screenDC);

		DWORD style = WS_VISIBLE;
		if (def.getStyle() == Style::None)
		{
			style |= WS_POPUP;
		}
		else
		{
			if (def.getStyle() & Style::Titlebar) style |= WS_CAPTION | WS_MINIMIZEBOX;
			if (def.getStyle() & Style::Resize)   style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
			if (def.getStyle() & Style::Close)    style |= WS_SYSMENU;
		}

		bool fullscreen = (style & Style::Fullscreen) != 0;
		if (!fullscreen)
		{
			RECT rectangle = { 0, 0, width, height };
			AdjustWindowRect(&rectangle, style, false);
			width = rectangle.right - rectangle.left;
			height = rectangle.bottom - rectangle.top;
		}

		std::wstring stemp = std::wstring(def.getTitle().begin(), def.getTitle().end());
		LPCWSTR windowTitle = stemp.c_str();
		_windowHandle = CreateWindowW(_className, windowTitle, style, left, top, width, height, NULL, NULL, _instance, this);
		setSize(Vector2i(def.getWidth(), def.getHeight()));

		LOG(Info, WindowLog, "Making Window");

		if (!_windowHandle)
		{
			LOG(Info, WindowLog, "hWnd is NULL");
			LOG(Info, WindowLog, "hWnd = " + std::to_string((int)_windowHandle));

			return false;
		}

		//_cursor = new WindowsCursor(this);
		//HCURSOR c = LoadCursor(_instance, IDC_IBEAM);
		//SetCursor(NULL);

		ShowWindow(_windowHandle, def.getShowCommand());
		UpdateWindow(_windowHandle);

		return true;
	}

	void WindowsWindowContext::registerClass(const WindowsWindowDefinition &def)
	{
		LOG(Info, WindowLog, "Registering Window Class.");

		WNDCLASSW windowClass;
		windowClass.style = 0;
		windowClass.lpfnWndProc = &globalWndProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = def.getInstance();
		windowClass.hIcon = NULL;
		windowClass.hCursor = LoadCursor(_instance, IDC_IBEAM);
		windowClass.hbrBackground = 0;
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = _className;
		RegisterClassW(&windowClass);
	}

	void WindowsWindowContext::processEvent(UINT message, WPARAM wParam, LPARAM lParam)
	{
		//LOG(Debug, WindowLog, "Process Event.....");

		if (_windowHandle == NULL)
			return;

		switch (message)
		{
		case WM_DESTROY:
		{
			destroy();
			break;
		}

		case WM_SETCURSOR:
		{
			if (LOWORD(lParam) == HTCLIENT)
				setCursor(_cursor);

			break;
		}

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			LOG(Debug, WindowLog, "KeyEvent pushed...");
			//if (_keyRepeatEnabled || ((HIWORD(lParam) & KF_REPEAT) == 0))
			{
				Message message = Message(
					new KeyEvent(
						KeyEvent::Pressed,
						0,
						HIWORD(GetAsyncKeyState(VK_MENU)) != 0,
						HIWORD(GetAsyncKeyState(VK_CONTROL)) != 0,
						HIWORD(GetAsyncKeyState(VK_SHIFT)) != 0,
						HIWORD(GetAsyncKeyState(VK_LWIN)) || HIWORD(GetAsyncKeyState(VK_RWIN))
					));

				//code = virtualKeyCodeToSF(wParam, lParam);
				_eventProcessor.pushMessage(message);
			}
			break;
		}
		}
	}

	LRESULT CALLBACK WindowsWindowContext::globalWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (message == WM_CREATE)
		{
			LONG_PTR window = (LONG_PTR)reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams;

			SetWindowLongPtrW(hWnd, GWLP_USERDATA, window);
		}

		WindowsWindowContext* window = hWnd ? reinterpret_cast<WindowsWindowContext*>(GetWindowLongPtr(hWnd, GWLP_USERDATA)) : NULL;

		if (window)
		{
			if (message == WA_ACTIVE)
			{
				ShowWindow(hWnd, SW_SHOW);
			}

			if (message == WA_INACTIVE)
			{
				ShowWindow(hWnd, SW_MINIMIZE);
			}
			window->processEvent(message, wParam, lParam);

			if (window->_callback)
				return CallWindowProcW(reinterpret_cast<WNDPROC>(window->_callback), hWnd, message, wParam, lParam);
		}

		if (message == WM_CLOSE)
			return 0;

		if ((message == WM_SYSCOMMAND) && (wParam == SC_KEYMENU))
			return 0;

		return DefWindowProcW(hWnd, message, wParam, lParam);
	}
}

#endif // ARCANA_PLATFORM_WINDOWS
