#include "WindowsApplicationContext.h"

#ifdef ARCANA_PLATFORM_WINDOWS

namespace Arcana
{
	uint32 WindowsApplicationContext::NUM_WINDOWS_CONTEXTS = 0;


	WindowsApplicationContext::WindowsApplicationContext()
	{
	}


	WindowsApplicationContext::~WindowsApplicationContext()
	{
	}

	bool WindowsApplicationContext::create(const ApplicationDefinition& definition)
	{
		LOG(Info, CoreEngine, "Creating Windows Application Context...");

		try {
			const WindowsApplicationDefinition &def = dynamic_cast<const WindowsApplicationDefinition&>(definition);

			LOGF(Debug, CoreEngine, "Show Command: %i", def.getShowCommand());


			if (NUM_WINDOWS_CONTEXTS == 0)
			{
				registerClass(def);
			}

			NUM_WINDOWS_CONTEXTS++;
		}
		catch (const std::bad_cast& e) {
			LOGF(Error, CoreEngine, "Bad Cast");
			return false;
		}

		LOG(Info, CoreEngine, "Done creating Windows Application Context.");

		return true;
	}

	LRESULT CALLBACK WindowsApplicationContext::WndProcTest(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			EndPaint(hWnd, &ps);
		}
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}

	void WindowsApplicationContext::registerClass(const WindowsApplicationDefinition &def)
	{
		LOG(Info, CoreEngine, "Registering Class.");

		/*WNDCLASSEXW wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = 0;
		wcex.lpfnWndProc = &WndProcTest;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = def.getInstance();
		wcex.hIcon = nullptr;// LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPLATFORMCLIENT));
		wcex.hCursor = 0;//LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = 0;//(HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = nullptr;//MAKEINTRESOURCEW(IDC_WINDOWSPLATFORMCLIENT);
		wcex.lpszClassName = (LPCWSTR)def.getWindowClass().c_str();
		//wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

		RegisterClassExW(&wcex);*/



		WNDCLASSW windowClass;
		windowClass.style = 0;
		windowClass.lpfnWndProc = &WndProcTest;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = def.getInstance();
		windowClass.hIcon = NULL;
		windowClass.hCursor = 0;
		windowClass.hbrBackground = 0;
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = def.getWindowClass();
		RegisterClassW(&windowClass);
	}
}

#endif // ARCANA_PLATFORM_WINDOWS
