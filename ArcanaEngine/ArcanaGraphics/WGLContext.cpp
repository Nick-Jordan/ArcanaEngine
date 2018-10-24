#include "WGLContext.h"

#include <sstream>
#include <locale>
#include <codecvt>

namespace Arcana
{
	WGLContext::WGLContext(WGLContext* sharedContext) :
		_window(0),
		_pbuffer(0),
		_deviceContext(0),
		_context(0),
		_ownsWindow(false)
	{
		_settings = RenderSettings();

		if (!initializeExtensions(sharedContext))
		{
			LOG(Error, CoreEngine, "Failed to initialize OpenGL extensions.");
		}

		createSurface(sharedContext, 1, 1);

		if (_deviceContext)
			createContext(sharedContext);
	}

	WGLContext::WGLContext(WGLContext* sharedContext, const RenderSettings& settings, const WindowContext* owner) :
		_window(0),
		_pbuffer(0),
		_deviceContext(0),
		_context(0),
		_ownsWindow(false)
	{
		_settings = settings;

		if (!initializeExtensions(sharedContext))
		{
			LOG(Error, CoreEngine, "Failed to initialize OpenGL extensions.");
		}

		createSurface(owner->getWindowHandle());

		if (_deviceContext)
			createContext(sharedContext);
	}

	WGLContext::WGLContext(WGLContext* sharedContext, const RenderSettings& settings, uint32 width, uint32 height) :
		_window(0),
		_pbuffer(0),
		_deviceContext(0),
		_context(0),
		_ownsWindow(false)
	{
		_settings = settings;

		if (!initializeExtensions(sharedContext))
		{
			LOG(Error, CoreEngine, "Failed to initialize OpenGL extensions.");
		}

		createSurface(sharedContext, width, height);

		if (_deviceContext)
			createContext(sharedContext);
	}

	WGLContext::~WGLContext()
	{
		//cleanupUnsharedResources();

		if (_context)
		{
			if (CurrentContext == this)
			{
				if (wglMakeCurrent(_deviceContext, NULL) == TRUE)
					CurrentContext = nullptr;
			}

			wglDeleteContext(_context);
		}

		if (_deviceContext)
		{
			if (_pbuffer)
			{
				wglReleasePbufferDCARB(_pbuffer, _deviceContext);
				wglDestroyPbufferARB(_pbuffer);
			}
			else
			{
				ReleaseDC(_window, _deviceContext);
			}
		}

		if (_window && _ownsWindow)
		{
			DestroyWindow(_window);
		}
	}




	void WGLContext::render()
	{
		if (_deviceContext && _context)
			SwapBuffers(_deviceContext);
	}

	bool WGLContext::setCurrent(bool current)
	{
		if (!_deviceContext || !_context)
			return false;

		if (wglMakeCurrent(_deviceContext, current ? _context : NULL) == FALSE)
		{
			LOGF(Error, CoreEngine, "Failed to %s OpenGL context: %f", current ? "activate" : "deactivate", getErrorString(GetLastError()).c_str());
			return false;
		}

		CurrentContext = (current ? this : nullptr);

		return true;
	}

	void WGLContext::setVerticalSync(bool enabled)
	{
		initializeExtensions(this);

		if (wglSwapIntervalEXT)
		{
			if (wglSwapIntervalEXT(enabled ? 1 : 0) == FALSE)
			{
				LOGF(Error, CoreEngine, "Error setting vertical sync: %s", getErrorString(GetLastError()).c_str());
			}
		}
		else
		{
			static bool warned = false;

			if (!warned)
			{
				LOG(Error, CoreEngine, "Setting vertical sync not supported");

				warned = true;
			}
		}
	}

	bool WGLContext::initializeExtensions(OpenGLContext* sharedContext)
	{
		static bool initialized = false;

		if (initialized)
		{
			return true;
		}

		initialized = true;

		HDC deviceContext = 0;

		if (sharedContext)
		{
			WGLContext* wglc = dynamic_cast<WGLContext*>(sharedContext);

			deviceContext = wglc ? wglc->_deviceContext : 0;
		}
		
		if(!deviceContext)
		{
			PIXELFORMATDESCRIPTOR pfd =
			{
				sizeof(PIXELFORMATDESCRIPTOR),
				1,
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
				PFD_TYPE_RGBA,
				32,
				0, 0, 0, 0, 0, 0,
				0,
				0,
				0,
				0, 0, 0, 0,
				24,
				8,
				0,
				PFD_MAIN_PLANE,
				0,
				0, 0, 0
			};

			WNDCLASSW windowClass;
			windowClass.style = 0;
			windowClass.lpfnWndProc = DefWindowProc;
			windowClass.cbClsExtra = 0;
			windowClass.cbWndExtra = 0;
			windowClass.hInstance = GetModuleHandleW(NULL);
			windowClass.hIcon = NULL;
			windowClass.hCursor = 0;
			windowClass.hbrBackground = 0;
			windowClass.lpszMenuName = NULL;
			windowClass.lpszClassName = L"Dummy";
			RegisterClassW(&windowClass);

			HWND hWnd = CreateWindowW(windowClass.lpszClassName, L"", NULL, 0, 0, 0, 0, NULL, NULL, GetModuleHandle(NULL), NULL);
			deviceContext = GetDC(hWnd);

			int pixelFormat;
			pixelFormat = ChoosePixelFormat(deviceContext, &pfd);
			SetPixelFormat(deviceContext, pixelFormat, &pfd);
		}

		HGLRC context = wglCreateContext(deviceContext);
		wglMakeCurrent(deviceContext, context);

		glewExperimental = GL_TRUE;
		GLenum glewinit = glewInit();
		if (glewinit != GLEW_OK)
		{
			LOG(Info, CoreEngine, "Failed initializing GLEW...");
			return false;
		}

		if (deviceContext)
		{
			if (context)
			{
				wglMakeCurrent(deviceContext, 0);
				wglDeleteContext(context);
			}

			ReleaseDC(GetDesktopWindow(), deviceContext);
		}

		Extensions.clear();

		int majorVersion = 0;
		glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);

		if (glGetError() == GL_INVALID_ENUM)
		{
			const char* extensionString = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));

			do
			{
				const char* extension = extensionString;

				while (*extensionString && (*extensionString != ' '))
					extensionString++;

				Extensions.push_back(std::string(extension, extensionString));
			} while (*extensionString++);
		}
		else
		{
			glGetStringiFuncType glGetStringiFunc = NULL;
			glGetStringiFunc = reinterpret_cast<glGetStringiFuncType>(getFunction("glGetStringi"));

			if (glGetStringiFunc)
			{
				int numExtensions = 0;
				glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

				if (numExtensions)
				{
					for (unsigned int i = 0; i < static_cast<unsigned int>(numExtensions); ++i)
					{
						const char* extensionString = reinterpret_cast<const char*>(glGetStringiFunc(GL_EXTENSIONS, i));

						Extensions.push_back(extensionString);
					}
				}
			}
		}

		return true;
	}

	int WGLContext::selectBestPixelFormat(HDC deviceContext, const RenderSettings& settings, bool pbuffer)
	{
		int bestFormat = 0;

		if (wglChoosePixelFormatARB)
		{
			int intAttributes[] =
			{
				WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
				WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
				WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
				WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
				0,                      0
			};

			int  formats[512];
			UINT nbFormats;
			bool isValid = wglChoosePixelFormatARB(deviceContext, intAttributes, NULL, 512, formats, &nbFormats) != FALSE;

			if (!isValid)
				LOGF(Error, CoreEngine, "Failed to enumerate pixel formats: %s", getErrorString(GetLastError()).c_str());

			if (isValid && (nbFormats > 0))
			{
				int bestScore = 0x7FFFFFFF;
				for (UINT i = 0; i < nbFormats; ++i)
				{
					int values[7];
					const int attributes[] =
					{
						WGL_RED_BITS_ARB,
						WGL_GREEN_BITS_ARB,
						WGL_BLUE_BITS_ARB,
						WGL_ALPHA_BITS_ARB,
						WGL_DEPTH_BITS_ARB,
						WGL_STENCIL_BITS_ARB,
						WGL_ACCELERATION_ARB
					};

					if (wglGetPixelFormatAttribivARB(deviceContext, formats[i], PFD_MAIN_PLANE, 7, attributes, values) == FALSE)
					{
						LOGF(Error, CoreEngine, "Failed to retrieve pixel format information: ", getErrorString(GetLastError()).c_str());
						break;
					}

					int sampleValues[2] = { 0, 0 };
					if (GLEW_ARB_multisample) //sfwgl_ext_ARB_multisample == sfwgl_LOAD_SUCCEEDED)
					{
						const int sampleAttributes[] =
						{
							WGL_SAMPLE_BUFFERS_ARB,
							WGL_SAMPLES_ARB
						};

						if (wglGetPixelFormatAttribivARB(deviceContext, formats[i], PFD_MAIN_PLANE, 2, sampleAttributes, sampleValues) == FALSE)
						{
							LOGF(Error, CoreEngine, "Failed to retrieve pixel format multisampling information: %s", getErrorString(GetLastError()).c_str());
							break;
						}
					}

					int sRgbCapableValue = 0;
					if (GLEW_EXT_framebuffer_sRGB || GLEW_ARB_framebuffer_sRGB)//(sfwgl_ext_ARB_framebuffer_sRGB == sfwgl_LOAD_SUCCEEDED) || (sfwgl_ext_EXT_framebuffer_sRGB == sfwgl_LOAD_SUCCEEDED))
					{
						const int sRgbCapableAttribute = WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB;

						if (wglGetPixelFormatAttribivARB(deviceContext, formats[i], PFD_MAIN_PLANE, 1, &sRgbCapableAttribute, &sRgbCapableValue) == FALSE)
						{
							LOGF(Error, CoreEngine, "Failed to retrieve pixel format sRGB capability information: %s", getErrorString(GetLastError()).c_str());
							break;
						}
					}

					if (pbuffer)
					{
						const int pbufferAttributes[] =
						{
							WGL_DRAW_TO_PBUFFER_ARB
						};

						int pbufferValue = 0;

						if (wglGetPixelFormatAttribivARB(deviceContext, formats[i], PFD_MAIN_PLANE, 1, pbufferAttributes, &pbufferValue) == FALSE)
						{
							LOGF(Error, CoreEngine, "Failed to retrieve pixel format pbuffer information: %s", getErrorString(GetLastError()).c_str());
							break;
						}

						if (pbufferValue != GL_TRUE)
							continue;
					}

					int color = values[0] + values[1] + values[2] + values[3];
					int score = evaluateFormat(settings.bitsPerPixel, settings, color, values[4], values[5], sampleValues[0] ? sampleValues[1] : 0, values[6] == WGL_FULL_ACCELERATION_ARB, sRgbCapableValue == TRUE);

					if (score < bestScore)
					{
						bestScore = score;
						bestFormat = formats[i];
					}
				}
			}
		}

		if (pbuffer)
			return bestFormat;

		if (bestFormat == 0)
		{
			PIXELFORMATDESCRIPTOR descriptor;
			ZeroMemory(&descriptor, sizeof(descriptor));
			descriptor.nSize = sizeof(descriptor);
			descriptor.nVersion = 1;
			descriptor.iLayerType = PFD_MAIN_PLANE;
			descriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			descriptor.iPixelType = PFD_TYPE_RGBA;
			descriptor.cColorBits = static_cast<BYTE>(settings.bitsPerPixel);
			descriptor.cDepthBits = static_cast<BYTE>(settings.depthBits);
			descriptor.cStencilBits = static_cast<BYTE>(settings.stencilBits);
			descriptor.cAlphaBits = settings.bitsPerPixel == 32 ? 8 : 0;

			bestFormat = ChoosePixelFormat(deviceContext, &descriptor);
		}

		return bestFormat;
	}

	std::string WGLContext::getErrorString(DWORD errorCode)
	{
		std::basic_ostringstream<TCHAR, std::char_traits<TCHAR> > ss;
		TCHAR errBuff[256];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorCode, 0, errBuff, sizeof(errBuff), NULL);
		ss << errBuff;

		using convert_type = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_type, wchar_t> converter;
		std::string cstr = converter.to_bytes(ss.str());

		return cstr;
	}

	GLFunctionPointer WGLContext::getFunction(const char* name)
	{
		GLFunctionPointer address = reinterpret_cast<GLFunctionPointer>(wglGetProcAddress(reinterpret_cast<LPCSTR>(name)));

		if (address)
		{
			ptrdiff_t errorCode = reinterpret_cast<ptrdiff_t>(address);

			if ((errorCode != -1) && (errorCode != 1) && (errorCode != 2) && (errorCode != 3))
				return address;
		}

		static HMODULE module = NULL;

		if (!module)
			module = GetModuleHandleA("OpenGL32.dll");

		if (module)
			return reinterpret_cast<GLFunctionPointer>(GetProcAddress(module, reinterpret_cast<LPCSTR>(name)));

		return 0;
	}

	void WGLContext::setDevicePixelFormat()
	{
		int bestFormat = selectBestPixelFormat(_deviceContext, _settings);

		if (bestFormat == 0)
		{
			LOGF(Error, CoreEngine, "Failed to find a suitable pixel format for device context: %s", getErrorString(GetLastError()).c_str());
			LOG(Error, CoreEngine, "Cannot create OpenGL context");
			return;
		}

		PIXELFORMATDESCRIPTOR actualFormat;
		actualFormat.nSize = sizeof(actualFormat);
		actualFormat.nVersion = 1;
		DescribePixelFormat(_deviceContext, bestFormat, sizeof(actualFormat), &actualFormat);

		if (SetPixelFormat(_deviceContext, bestFormat, &actualFormat) == FALSE)
		{
			LOGF(Error, CoreEngine, "Failed to set pixel format for device context: %s", getErrorString(GetLastError()).c_str());
			LOG(Error, CoreEngine, "Cannot create OpenGL context");
			return;
		}
	}

	void WGLContext::updateSettingsFromPixelFormat()
	{
		int format = GetPixelFormat(_deviceContext);

		if (format == 0)
		{
			LOGF(Error, CoreEngine, "Failed to get selected pixel format: %s", getErrorString(GetLastError()).c_str());
			return;
		}

		PIXELFORMATDESCRIPTOR actualFormat;
		actualFormat.nSize = sizeof(actualFormat);
		actualFormat.nVersion = 1;

		if (DescribePixelFormat(_deviceContext, format, sizeof(actualFormat), &actualFormat) == 0)
		{
			LOGF(Error, CoreEngine, "Failed to retrieve pixel format information: %s", getErrorString(GetLastError()).c_str());
			return;
		}

		if (WGLEW_ARB_pixel_format)//sfwgl_ext_ARB_pixel_format == sfwgl_LOAD_SUCCEEDED)
		{
			const int attributes[] = { WGL_DEPTH_BITS_ARB, WGL_STENCIL_BITS_ARB };
			int values[2];

			if (wglGetPixelFormatAttribivARB(_deviceContext, format, PFD_MAIN_PLANE, 2, attributes, values) == TRUE)
			{
				_settings.depthBits = values[0];
				_settings.stencilBits = values[1];
			}
			else
			{
				LOGF(Error, CoreEngine, "Failed to retrieve pixel format information: %s", getErrorString(GetLastError()).c_str());
				_settings.depthBits = actualFormat.cDepthBits;
				_settings.stencilBits = actualFormat.cStencilBits;
			}

			if (WGLEW_ARB_multisample)//sfwgl_ext_ARB_multisample == sfwgl_LOAD_SUCCEEDED)
			{
				const int sampleAttributes[] = { WGL_SAMPLE_BUFFERS_ARB, WGL_SAMPLES_ARB };
				int sampleValues[2];

				if (wglGetPixelFormatAttribivARB(_deviceContext, format, PFD_MAIN_PLANE, 2, sampleAttributes, sampleValues) == TRUE)
				{
					_settings.antialiasingLevel = sampleValues[0] ? sampleValues[1] : 0;
				}
				else
				{
					LOGF(Error, CoreEngine, "Failed to retrieve pixel format multisampling information: %s", getErrorString(GetLastError()).c_str());
					_settings.antialiasingLevel = 0;
				}
			}
			else
			{
				_settings.antialiasingLevel = 0;
			}

			if (GLEW_EXT_framebuffer_sRGB || GLEW_ARB_framebuffer_sRGB)//(sfwgl_ext_ARB_framebuffer_sRGB == sfwgl_LOAD_SUCCEEDED) || (sfwgl_ext_EXT_framebuffer_sRGB == sfwgl_LOAD_SUCCEEDED))
			{
				const int sRgbCapableAttribute = WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB;
				int sRgbCapableValue = 0;

				if (wglGetPixelFormatAttribivARB(_deviceContext, format, PFD_MAIN_PLANE, 1, &sRgbCapableAttribute, &sRgbCapableValue) == TRUE)
				{
					_settings.sRgb = (sRgbCapableValue == TRUE);
				}
				else
				{
					LOGF(Error, CoreEngine, "Failed to retrieve pixel format sRGB capability information: %s", getErrorString(GetLastError()).c_str());
					_settings.sRgb = false;
				}
			}
			else
			{
				_settings.sRgb = false;
			}
		}
		else
		{
			_settings.depthBits = actualFormat.cDepthBits;
			_settings.stencilBits = actualFormat.cStencilBits;
			_settings.antialiasingLevel = 0;
		}
	}


	void WGLContext::createSurface(WGLContext* shared, unsigned int width, unsigned int height)
	{
		if (shared && shared->_deviceContext && WGLEW_ARB_pbuffer)//(sfwgl_ext_ARB_pbuffer == sfwgl_LOAD_SUCCEEDED))
		{
			int bestFormat = selectBestPixelFormat(shared->_deviceContext, _settings, true);

			if (bestFormat > 0)
			{
				int attributes[] = { 0, 0 };

				_pbuffer = wglCreatePbufferARB(shared->_deviceContext, bestFormat, width, height, attributes);

				if (_pbuffer)
				{
					_window = shared->_window;
					_deviceContext = wglGetPbufferDCARB(_pbuffer);

					if (!_deviceContext)
					{
						LOGF(Error, CoreEngine, "Failed to retrieve pixel buffer device context: %s", getErrorString(GetLastError()).c_str());

						wglDestroyPbufferARB(_pbuffer);
						_pbuffer = NULL;
					}
				}
				else
				{
					LOGF(Error, CoreEngine, "Failed to create pixel buffer: %s", getErrorString(GetLastError()).c_str());
				}
			}
		}

		if (!_deviceContext)
		{
			_window = CreateWindowA("STATIC", "", WS_POPUP | WS_DISABLED, 0, 0, width, height, NULL, NULL, GetModuleHandle(NULL), NULL);
			ShowWindow(_window, SW_HIDE);
			_deviceContext = GetDC(_window);

			_ownsWindow = true;

			setDevicePixelFormat();
		}

		updateSettingsFromPixelFormat();
	}


	void WGLContext::createSurface(HWND window)
	{
		_window = window;
		_deviceContext = GetDC(window);

		setDevicePixelFormat();

		updateSettingsFromPixelFormat();
	}


	void WGLContext::createContext(WGLContext* shared)
	{
		RenderSettings settings = _settings;

		HGLRC sharedContext = shared ? shared->_context : NULL;

		while (!_context && _settings.majorVersion)
		{
			if (WGLEW_ARB_create_context)//sfwgl_ext_ARB_create_context == sfwgl_LOAD_SUCCEEDED)
			{
				std::vector<int> attributes;

				if ((_settings.majorVersion > 1) || ((_settings.majorVersion == 1) && (_settings.minorVersion > 1)))
				{
					attributes.push_back(WGL_CONTEXT_MAJOR_VERSION_ARB);
					attributes.push_back(_settings.majorVersion);
					attributes.push_back(WGL_CONTEXT_MINOR_VERSION_ARB);
					attributes.push_back(_settings.minorVersion);
				}

				if (WGLEW_ARB_create_context_profile)//sfwgl_ext_ARB_create_context_profile == sfwgl_LOAD_SUCCEEDED)
				{
					int profile = (_settings.attributeFlags & RenderSettings::Core) ? WGL_CONTEXT_CORE_PROFILE_BIT_ARB : WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
					int debug = (_settings.attributeFlags & RenderSettings::Debug) ? WGL_CONTEXT_DEBUG_BIT_ARB : 0;

					attributes.push_back(WGL_CONTEXT_PROFILE_MASK_ARB);
					attributes.push_back(profile);
					attributes.push_back(WGL_CONTEXT_FLAGS_ARB);
					attributes.push_back(debug);
				}
				else
				{
					if ((_settings.attributeFlags & RenderSettings::Core) || (_settings.attributeFlags & RenderSettings::Debug))
						LOG(Error, CoreEngine, "Selecting a profile during context creation is not supported. Disabling comptibility and debug");

					_settings.attributeFlags = RenderSettings::Default;
				}

				attributes.push_back(0);
				attributes.push_back(0);

				if (sharedContext)
				{
					static Mutex mutex;
					Lock lock(mutex);

					if (CurrentContext == shared)
					{
						if (wglMakeCurrent(shared->_deviceContext, NULL) == FALSE)
						{
							LOGF(Error, CoreEngine, "Failed to deactivate shared context before sharing: %s", getErrorString(GetLastError()).c_str());
							return;
						}

						CurrentContext = NULL;
					}
				}

				_context = wglCreateContextAttribsARB(_deviceContext, sharedContext, &attributes[0]);
			}
			else
			{
				break;
			}

			if (!_context)
			{
				if (_settings.attributeFlags != RenderSettings::Default)
				{
					_settings.attributeFlags = RenderSettings::Default;
				}
				else if (_settings.minorVersion > 0)
				{
					_settings.minorVersion--;

					_settings.attributeFlags = settings.attributeFlags;
				}
				else
				{
					_settings.majorVersion--;
					_settings.minorVersion = 9;

					_settings.attributeFlags = settings.attributeFlags;
				}
			}
		}

		if (!_context)
		{
			_settings.majorVersion = 1;
			_settings.minorVersion = 1;
			_settings.attributeFlags = RenderSettings::Default;

			_context = wglCreateContext(_deviceContext);
			if (!_context)
			{
				LOGF(Error, CoreEngine, "Failed to create an OpenGL context for this window: %s", getErrorString(GetLastError()).c_str());
				return;
			}

			if (sharedContext)
			{
				static Mutex mutex;
				Lock lock(mutex);

				if (CurrentContext == shared)
				{
					if (wglMakeCurrent(shared->_deviceContext, NULL) == FALSE)
					{
						LOGF(Error, CoreEngine, "Failed to deactivate shared context before sharing: %s", getErrorString(GetLastError()).c_str());
						return;
					}

					CurrentContext = NULL;
				}

				if (wglShareLists(sharedContext, _context) == FALSE)
					LOGF(Error, CoreEngine, "Failed to share the OpenGL context: %s", getErrorString(GetLastError()).c_str());
			}
		}
	}
}