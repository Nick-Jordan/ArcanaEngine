#include "WindowsWindowContext.h"

#ifdef ARCANA_PLATFORM_WINDOWS

#include "WindowsWindowDefinition.h"
#include "WindowsApplicationContext.h"
#include "Window.h"
#include "WindowsCursor.h"

#include "NoDataEvents.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "ControllerConnectEvent.h"

#include <vector>

namespace Arcana
{
	const GUID GUID_DEVINTERFACE_HID = { 0x4d1e55b2, 0xf16f, 0x11cf,{ 0x88, 0xcb, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } };

	uint32 WindowsWindowContext::NumWin32WindowContexts = 0;
	//const wchar_t* WindowsWindowContext::className = L"WindowClass";

	WindowsWindowContext::WindowsWindowContext() : WindowContext(), 
		_cursorGrabbed(false), _cursorVisible(true), _repeatKeyEvents(false), _mouseContained(false)
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

				ControllerContext::setLazyUpdates(false);
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

	void WindowsWindowContext::setMouseCursorVisible(bool visible)
	{
		if (visible != _cursorVisible)
		{
			_cursorVisible = visible;
			ShowCursor(visible);
		}
	}

	void WindowsWindowContext::setMouseCursorGrabbed(bool grabbed)
	{
		_cursorGrabbed = grabbed;
		grabCursor(grabbed);
	}

	void WindowsWindowContext::repeatKeyEvents(bool repeat)
	{
		_repeatKeyEvents = repeat;
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

	void WindowsWindowContext::processControllerEvents()
	{
		//LOG(Error, CoreEngine, "Processing controller events...");

		ControllerManager::instance().update();

		for (uint32 i = 0; i < Controller::MaxControllers; ++i)
		{
			ControllerState previousState = _controllerStates[i];
			_controllerStates[i] = ControllerManager::instance().getState(i);

			bool connected = _controllerStates[i].connected;
			if (previousState.connected ^ connected)
			{
				ControllerConnectEvent::Type eventType = connected ? ControllerConnectEvent::Connected : ControllerConnectEvent::Disconnected;

				_eventProcessor.pushMessage(Message(new ControllerConnectEvent(eventType, i)));

				if (connected)
					std::fill_n(_previousAxes[i], static_cast<std::size_t>(Controller::FloatAxisCount), 0.0f);
			}

			if (connected)
			{
				for (uint32 j = 0; j < Controller::FloatAxisCount; ++j)
				{
					Key key = ReverseStateAxisMapping(j);

					float previousPos = _previousAxes[i][j];
					float currentPos = _controllerStates[i].floatAxes[j];

					//TEST  --- CHANGE HOW THRESHOLD IS HANDLED
					float controllerThreshold = 0.1;
					//TEST

					//LOGF(Info, CoreEngine, "Controller Key %s value is %f.", key.getGlobalObjectID().getName().c_str(), currentPos);

					if (fabs(currentPos - previousPos) >= controllerThreshold)
					{
						Message message = Message(
							new KeyEvent(i, key.getKeyCode(), currentPos)
						);
						_eventProcessor.pushMessage(message);
						_previousAxes[i][j] = currentPos;
					}

				}

				// Buttons
				for (uint32 j = 0; j < Controller::ButtonCount; ++j)
				{
					Key key = ReverseStateButtonMapping(j);

					bool previousPressed = previousState.buttons[j];
					bool currentPressed = _controllerStates[i].buttons[j];

					//LOGF(Info, CoreEngine, "Controller Key %s %s pressed.", key.getGlobalObjectID().getName().c_str(), currentPressed ? "is" : "is not");

					if (previousPressed ^ currentPressed)
					{
						KeyEvent::Type eventType = currentPressed ? KeyEvent::Pressed : KeyEvent::Released;

						Message message = Message(
							new KeyEvent(eventType, j, key.getKeyCode())
						);

						_eventProcessor.pushMessage(message);
					}
				}
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
			ControllerContext::setLazyUpdates(true);
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

		DEV_BROADCAST_DEVICEINTERFACE deviceInterface = { sizeof(DEV_BROADCAST_DEVICEINTERFACE), DBT_DEVTYP_DEVICEINTERFACE, 0, GUID_DEVINTERFACE_HID, 0 };
		RegisterDeviceNotification(_windowHandle, &deviceInterface, DEVICE_NOTIFY_WINDOW_HANDLE);

		ControllerContext::initialize();

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

	void WindowsWindowContext::trackMouse(bool track)
	{
		TRACKMOUSEEVENT mouseEvent;
		mouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
		mouseEvent.dwFlags = track ? TME_LEAVE : TME_CANCEL;
		mouseEvent.hwndTrack = _windowHandle;
		mouseEvent.dwHoverTime = HOVER_DEFAULT;
		TrackMouseEvent(&mouseEvent);
	}

	void WindowsWindowContext::grabCursor(bool grab)
	{
		if (grab)
		{
			RECT rect;
			GetClientRect(_windowHandle, &rect);
			MapWindowPoints(_windowHandle, NULL, reinterpret_cast<LPPOINT>(&rect), 2);
			ClipCursor(&rect);
		}
		else
		{
			ClipCursor(NULL);
		}
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
		case WM_CLOSE:
		{
			LOG(Debug, WindowLog, "Closed Event pushed...");
			Message message = Message(new WindowClosedEvent());
			_eventProcessor.pushMessage(message);
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
			if (_repeatKeyEvents || ((HIWORD(lParam) & KF_REPEAT) == 0))
			{

				Message message = Message(
					new KeyEvent(
						KeyEvent::Pressed,
						windowsKeyConversion(wParam, lParam).getKeyCode(),
						HIWORD(GetAsyncKeyState(VK_MENU)) != 0,
						HIWORD(GetAsyncKeyState(VK_CONTROL)) != 0,
						HIWORD(GetAsyncKeyState(VK_SHIFT)) != 0,
						HIWORD(GetAsyncKeyState(VK_LWIN)) || HIWORD(GetAsyncKeyState(VK_RWIN))
					));

				_eventProcessor.pushMessage(message);
			}
			break;
		}
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			Message message = Message(
				new KeyEvent(
					KeyEvent::Released,
					windowsKeyConversion(wParam, lParam).getKeyCode(),
					HIWORD(GetAsyncKeyState(VK_MENU)) != 0,
					HIWORD(GetAsyncKeyState(VK_CONTROL)) != 0,
					HIWORD(GetAsyncKeyState(VK_SHIFT)) != 0,
					HIWORD(GetAsyncKeyState(VK_LWIN)) || HIWORD(GetAsyncKeyState(VK_RWIN))
				));

			_eventProcessor.pushMessage(message);

			break;
		}

		case WM_MOUSEWHEEL:
		{
			// Mouse position is in screen coordinates, convert it to window coordinates
			POINT position;
			position.x = static_cast<int16>(LOWORD(lParam));
			position.y = static_cast<int16>(HIWORD(lParam));
			ScreenToClient(_windowHandle, &position);

			int16 delta = static_cast<int16>(HIWORD(wParam));

			Message message = Message(
				new MouseEvent(
					MouseEvent::WheelMoved,
					position.x,
					position.y,
					delta / 120
				));

			_eventProcessor.pushMessage(message);

			Message message2 = Message(
				new MouseEvent(
					position.x,
					position.y,
					static_cast<float>(delta) / 120.f,
					MouseEvent::Wheel::Vertical
				));

			_eventProcessor.pushMessage(message2);

			break;
		}

		case WM_MOUSEHWHEEL:
		{
			POINT position;
			position.x = static_cast<int16>(LOWORD(lParam));
			position.y = static_cast<int16>(HIWORD(lParam));
			ScreenToClient(_windowHandle, &position);

			int16 delta = static_cast<int16>(HIWORD(wParam));

			Message message = Message(
				new MouseEvent(
					position.x,
					position.y,
					static_cast<float>(delta) / 120.f,
					MouseEvent::Wheel::Horizontal
				));

			_eventProcessor.pushMessage(message);
			break;
		}

		case WM_LBUTTONDOWN:
		{
			Message message = Message(
				new KeyEvent(
					KeyEvent::Pressed,
					KeyCode::LeftMouseButton,
					static_cast<int16>(LOWORD(lParam)),
					static_cast<int16>(HIWORD(lParam))
				));

			_eventProcessor.pushMessage(message);
			break;
		}

		case WM_LBUTTONUP:
		{
			Message message = Message(
				new KeyEvent(
					KeyEvent::Released,
					KeyCode::LeftMouseButton,
					static_cast<int16>(LOWORD(lParam)),
					static_cast<int16>(HIWORD(lParam))
				));

			_eventProcessor.pushMessage(message);
			break;
		}

		case WM_RBUTTONDOWN:
		{
			Message message = Message(
				new KeyEvent(
					KeyEvent::Pressed,
					KeyCode::RightMouseButton,
					static_cast<int16>(LOWORD(lParam)),
					static_cast<int16>(HIWORD(lParam))
				));

			_eventProcessor.pushMessage(message);
			break;
		}

		case WM_RBUTTONUP:
		{
			Message message = Message(
				new KeyEvent(
					KeyEvent::Released,
					KeyCode::RightMouseButton,
					static_cast<int16>(LOWORD(lParam)),
					static_cast<int16>(HIWORD(lParam))
				));

			_eventProcessor.pushMessage(message);
			break;
		}

		case WM_MBUTTONDOWN:
		{
			Message message = Message(
				new KeyEvent(
					KeyEvent::Pressed,
					KeyCode::MiddleMouseButton,
					static_cast<int16>(LOWORD(lParam)),
					static_cast<int16>(HIWORD(lParam))
				));

			_eventProcessor.pushMessage(message);
			break;
		}

		case WM_MBUTTONUP:
		{
			Message message = Message(
				new KeyEvent(
					KeyEvent::Released,
					KeyCode::MiddleMouseButton,
					static_cast<int16>(LOWORD(lParam)),
					static_cast<int16>(HIWORD(lParam))
				));

			_eventProcessor.pushMessage(message);
			break;
		}

		case WM_XBUTTONDOWN:
		{
			Message message = Message(
				new KeyEvent(
					KeyEvent::Pressed,
					HIWORD(wParam) == XBUTTON1 ? KeyCode::ThumbMouseButton1 : KeyCode::ThumbMouseButton2,
					static_cast<int16>(LOWORD(lParam)),
					static_cast<int16>(HIWORD(lParam))
				));

			_eventProcessor.pushMessage(message);
			break;
		}

		case WM_XBUTTONUP:
		{
			Message message = Message(
				new KeyEvent(
					KeyEvent::Released,
					HIWORD(wParam) == XBUTTON1 ? KeyCode::ThumbMouseButton1 : KeyCode::ThumbMouseButton2,
					static_cast<int16>(LOWORD(lParam)),
					static_cast<int16>(HIWORD(lParam))
				));

			_eventProcessor.pushMessage(message);
			break;
		}

		case WM_MOUSELEAVE:
		{
			if (_mouseContained)
			{
				_mouseContained = false;

				Message message = Message(new MouseEvent(
					MouseEvent::Exited
				));
				_eventProcessor.pushMessage(message);
			}
			break;
		}


		case WM_MOUSEMOVE:
		{
			int16 x = static_cast<int16>(LOWORD(lParam));
			int16 y = static_cast<int16>(HIWORD(lParam));

			RECT area;
			GetClientRect(_windowHandle, &area);

			if ((wParam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON | MK_XBUTTON1 | MK_XBUTTON2)) == 0)
			{
				if (GetCapture() == _windowHandle)
					ReleaseCapture();
			}
			else if (GetCapture() != _windowHandle)
			{
				SetCapture(_windowHandle);
			}

			if ((x < area.left) || (x > area.right) || (y < area.top) || (y > area.bottom))
			{
				if (_mouseContained)
				{
					_mouseContained = false;

					trackMouse(false);

					Message message = Message(new MouseEvent(
						MouseEvent::Exited
					));
					_eventProcessor.pushMessage(message);
				}
			}
			else
			{
				if (!_mouseContained)
				{
					_mouseContained = true;

					trackMouse(true);

					Message message = Message(new MouseEvent(
						MouseEvent::Entered
					));
					_eventProcessor.pushMessage(message);
				}
			}

			Message message = Message(
				new MouseEvent(
					x,
					y
				));

			_eventProcessor.pushMessage(message);

			break;
		}

		case WM_DEVICECHANGE:
		{
			LOG(Info, CoreEngine, "Device change event....");

			if ((wParam == DBT_DEVICEARRIVAL) || (wParam == DBT_DEVICEREMOVECOMPLETE))
			{
				DEV_BROADCAST_HDR* deviceBroadcastHeader = reinterpret_cast<DEV_BROADCAST_HDR*>(lParam);

				if (deviceBroadcastHeader && (deviceBroadcastHeader->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE))
				{
					LOG(Info, CoreEngine, "Controller connected....");
					ControllerContext::updateConnections();
				}
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

	Key WindowsWindowContext::windowsKeyConversion(WPARAM key, LPARAM flags)
	{
		switch (key)
		{
		case 'A':			return Keys::A;
		case 'B':			return Keys::B;
		case 'C':			return Keys::C;
		case 'D':			return Keys::D;
		case 'E':			return Keys::E;
		case 'F':			return Keys::F;
		case 'G':			return Keys::G;
		case 'H':			return Keys::H;
		case 'I':			return Keys::I;
		case 'J':			return Keys::J;
		case 'K':			return Keys::K;
		case 'L':			return Keys::L;
		case 'M':			return Keys::M;
		case 'N':			return Keys::N;
		case 'O':			return Keys::O;
		case 'P':			return Keys::P;
		case 'Q':			return Keys::Q;
		case 'R':			return Keys::R;
		case 'S':			return Keys::S;
		case 'T':			return Keys::T;
		case 'U':			return Keys::U;
		case 'V':			return Keys::V;
		case 'W':			return Keys::W;
		case 'X':			return Keys::X;
		case 'Y':			return Keys::Y;
		case 'Z':			return Keys::Z;

		case '0':			return Keys::Num_0;
		case '1':			return Keys::Num_1;
		case '2':			return Keys::Num_2;
		case '3':			return Keys::Num_3;
		case '4':			return Keys::Num_4;
		case '5':			return Keys::Num_5;
		case '6':			return Keys::Num_6;
		case '7':			return Keys::Num_7;
		case '8':			return Keys::Num_8;
		case '9':			return Keys::Num_9;

		case VK_ESCAPE:		return Keys::Escape;
		case VK_LMENU:
		case VK_RMENU:
		case VK_APPS:		return Keys::Menu;

		case VK_OEM_4:		return Keys::LeftBracket;
		case VK_OEM_6:		return Keys::RightBracket;
		case VK_OEM_1:		return Keys::Semicolon;
		case VK_OEM_COMMA:	return Keys::Comma;
		case VK_DECIMAL:
		case VK_OEM_PERIOD: return Keys::Period;
		case VK_OEM_7:		return Keys::Quote;
		case VK_OEM_2:		return Keys::Slash;
		case VK_OEM_5:		return Keys::Backslash;
		case VK_OEM_3:		return Keys::Tilde;
		case VK_OEM_PLUS:	return Keys::Equal;
		case VK_OEM_MINUS:	return Keys::Hyphen;
		case VK_SPACE:		return Keys::Space;
		case VK_CAPITAL:	return Keys::CapsLock;
		case VK_RETURN:		return Keys::Enter;

		case VK_BACK:		return Keys::Back;
		case VK_TAB:		return Keys::Tab;
		case VK_PRIOR:		return Keys::PageUp;
		case VK_NEXT:		return Keys::PageDown;
		case VK_END:		return Keys::End;
		case VK_HOME:		return Keys::Home;
		case VK_INSERT:		return Keys::Insert;
		case VK_DELETE:		return Keys::Delete;
		case VK_ADD: 		return Keys::Add;
		case VK_SUBTRACT:	return Keys::Subtract;
		case VK_MULTIPLY:	return Keys::Multiply;
		case VK_DIVIDE:		return Keys::Divide;
		case VK_LEFT:		return Keys::Left;
		case VK_RIGHT:		return Keys::Right;
		case VK_UP:			return Keys::Up;
		case VK_DOWN:		return Keys::Down;

		case VK_NUMPAD0:	return Keys::Numpad_0;
		case VK_NUMPAD1:	return Keys::Numpad_1;
		case VK_NUMPAD2:	return Keys::Numpad_2;
		case VK_NUMPAD3:	return Keys::Numpad_3;
		case VK_NUMPAD4:	return Keys::Numpad_4;
		case VK_NUMPAD5:	return Keys::Numpad_5;
		case VK_NUMPAD6:	return Keys::Numpad_6;
		case VK_NUMPAD7:	return Keys::Numpad_7;
		case VK_NUMPAD8:	return Keys::Numpad_8;
		case VK_NUMPAD9:	return Keys::Numpad_9;

		case VK_F1:			return Keys::F1;
		case VK_F2:			return Keys::F2;
		case VK_F3:			return Keys::F3;
		case VK_F4:			return Keys::F4;
		case VK_F5:			return Keys::F5;
		case VK_F6:			return Keys::F6;
		case VK_F7:			return Keys::F7;
		case VK_F8:			return Keys::F8;
		case VK_F9:			return Keys::F9;
		case VK_F10:		return Keys::F10;
		case VK_F11:		return Keys::F11;
		case VK_F12:		return Keys::F12;
		case VK_F13:		return Keys::F13;
		case VK_F14:		return Keys::F14;
		case VK_F15:		return Keys::F15;
		case VK_PAUSE:		return Keys::Pause;

		case VK_SHIFT:
		{
			static UINT lShift = MapVirtualKeyW(VK_LSHIFT, MAPVK_VK_TO_VSC);
			UINT code = static_cast<UINT>((flags & (0xFF << 16)) >> 16);
			return code == lShift ? Keys::LeftShift : Keys::RightShift;
		}
		case VK_MENU: return (HIWORD(flags) & KF_EXTENDED) ? Keys::RightAlt : Keys::LeftAlt;

		case VK_CONTROL: return (HIWORD(flags) & KF_EXTENDED) ? Keys::RightControl : Keys::LeftControl;

		case VK_LWIN: return Keys::LeftSystem;
		case VK_RWIN: return Keys::RightSystem;

		}

		return Keys::ErrorKey;
	}
}

#endif // ARCANA_PLATFORM_WINDOWS
