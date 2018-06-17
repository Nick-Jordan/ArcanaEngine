#include "WindowsWindowContext.h"

#ifdef ARCANA_PLATFORM_WINDOWS

#include "WindowsWindowDefinition.h"

#include "WindowsApplicationContext.h"

namespace Arcana
{

	WindowsWindowContext::WindowsWindowContext() : WindowContext()
	{
	}


	WindowsWindowContext::~WindowsWindowContext()
	{
	}

	bool WindowsWindowContext::create(const WindowDefinition& definition)
	{
		LOG(Info, CoreEngine, "Creating Windows Window Context...");

		try {
			const WindowsWindowDefinition &def = dynamic_cast<const WindowsWindowDefinition&>(definition);

			return createWindow(def);
		}
		catch (const std::bad_cast& e) {
			LOGF(Error, CoreEngine, "Bad Cast");
			return false;
		}
	}

	bool WindowsWindowContext::destroy()
	{
		return true;
	}

	HINSTANCE WindowsWindowContext::getInstance() const
	{
		return _instance;
	}

	bool WindowsWindowContext::createWindow(const WindowsWindowDefinition& def)
	{
		_instance = def.getInstance();

		LOG(Debug, CoreEngine, "Instance: " + std::to_string((int)_instance));

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
		_windowHandle = CreateWindowW(def.getWindowClass(), windowTitle, style, left, top, width, height, NULL, NULL, _instance, this);

		LOG(Info, CoreEngine, "Making Window");

		if (!_windowHandle)
		{
			LOG(Info, CoreEngine, "hWnd is NULL");
			LOG(Info, CoreEngine, "hWnd = " + std::to_string((int)_windowHandle));

			return false;
		}

		ShowWindow(_windowHandle, def.getShowCommand());
		UpdateWindow(_windowHandle);




		///MESSAGE LOOP FOR TESTING
		MSG msg;

		// Main message loop:
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			if (!TranslateAccelerator(msg.hwnd, NULL, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		return true;
	}
}

#endif // ARCANA_PLATFORM_WINDOWS
