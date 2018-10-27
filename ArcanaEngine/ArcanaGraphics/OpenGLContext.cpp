#include "OpenGLContext.h"

#include "ContextType.h"
#include "Renderer.h"

namespace Arcana
{
	OpenGLContext* OpenGLContext::CurrentContext = nullptr;

	uint64 OpenGLContext::Id = 1;

	Mutex OpenGLContext::ContextMutex = Mutex();

	GLContext* OpenGLContext::SharedContext = nullptr;

	std::vector<std::string> OpenGLContext::Extensions;


	OpenGLContext::OpenGLContext() : _id(Id++)
	{
		
	}	
	
	OpenGLContext::~OpenGLContext()
	{
	}
	
	
	bool OpenGLContext::setActive(bool active)
	{
		if (active)
		{
			if (this != CurrentContext)
			{
				Lock lock(ContextMutex);

				if (setCurrent(true))
				{
					CurrentContext = this;
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return true;
			}
		}
		else
		{
			if (this == CurrentContext)
			{
				Lock lock(ContextMutex);

				if (setCurrent(false))
				{
					CurrentContext = NULL;
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return true;
			}
		}
	}
	
	const RenderSettings& OpenGLContext::getSettings() const
	{
		return _settings;
	}
	
	uint64 OpenGLContext::getContextId() const
	{
		return _id;
	}

	GLContext* OpenGLContext::create()
	{
		if (!SharedContext)
		{
			SharedContext = new GLContext(nullptr);
			SharedContext->initialize(RenderSettings());
		}

		Lock lock(ContextMutex);

		GLContext* context = nullptr;

		SharedContext->setActive(true);
		context = new GLContext(SharedContext);
		SharedContext->setActive(false);

		context->initialize(RenderSettings());

		return context;
	}

	GLContext* OpenGLContext::create(const RenderSettings& settings, const WindowContext* owner)
	{
		if (!SharedContext)
		{
			SharedContext = new GLContext(nullptr);
			SharedContext->initialize(RenderSettings());
		}

		Lock lock(ContextMutex);

		GLContext* context = nullptr;

		SharedContext->setActive(true);
		context = new GLContext(SharedContext, settings, owner);
		SharedContext->setActive(false);

		context->initialize(settings);
		context->compareSettings(settings);

		return context;
	}

	GLContext* OpenGLContext::create(const RenderSettings& settings, unsigned int width, unsigned int height)
	{
		if (!SharedContext)
		{
			SharedContext = new GLContext(nullptr);
			SharedContext->initialize(RenderSettings());
		}

		Lock lock(ContextMutex);

		GLContext* context = nullptr;

		SharedContext->setActive(true);
		context = new GLContext(SharedContext, settings, width, height);
		SharedContext->setActive(false);

		context->initialize(settings);
		context->compareSettings(settings);

		return context;
	}

	uint64 OpenGLContext::getActiveContextId()
	{
		if (CurrentContext)
		{
			return CurrentContext->getContextId();
		}

		return 0;
	}

	bool OpenGLContext::hasExtension(const char* name)
	{
		return std::find(Extensions.begin(), Extensions.end(), name) != Extensions.end();
	}

	GLFunctionPointer OpenGLContext::getFunction(const char* name)
	{
		Lock lock(ContextMutex);

		return GLContext::getFunction(name);
	}

	void OpenGLContext::clear(int32 flags, float red, float green, float blue, float alpha, float clearDepth, int32 clearStencil)
	{
		GLbitfield bits = 0;
		if (flags & Renderer::ClearColor)
		{
			if (red != _clearColor.x ||
				green != _clearColor.y ||
				blue != _clearColor.z ||
				alpha != _clearColor.w)
			{
				glClearColor(red, green, blue, alpha);
				_clearColor.set(red, green, blue, alpha);
			}
			bits |= GL_COLOR_BUFFER_BIT;
		}

		if (flags & Renderer::ClearDepth)
		{
			if (clearDepth != _clearDepth)
			{
				glClearDepth(clearDepth);
				_clearDepth = clearDepth;
			}
			bits |= GL_DEPTH_BUFFER_BIT;

			glDepthMask(GL_TRUE);
		}

		if (flags & Renderer::ClearStencil)
		{
			if (clearStencil != _clearStencil)
			{
				glClearStencil(clearStencil);
				_clearStencil = clearStencil;
			}
			bits |= GL_STENCIL_BUFFER_BIT;
		}

		glClear(bits);
	}

	int32 OpenGLContext::evaluateFormat(uint32 bitsPerPixel, const RenderSettings& settings, int32 colorBits, int32 depthBits, int32 stencilBits, int32 antialiasing, bool accelerated, bool sRgb)
	{
		int colorDiff = static_cast<int>(bitsPerPixel) - colorBits;
		int depthDiff = static_cast<int>(settings.depthBits) - depthBits;
		int stencilDiff = static_cast<int>(settings.stencilBits) - stencilBits;
		int antialiasingDiff = static_cast<int>(settings.antialiasingLevel) - antialiasing;

		colorDiff *= ((colorDiff        > 0) ? 100000 : 1);
		depthDiff *= ((depthDiff        > 0) ? 100000 : 1);
		stencilDiff *= ((stencilDiff      > 0) ? 100000 : 1);
		antialiasingDiff *= ((antialiasingDiff > 0) ? 100000 : 1);

		int score = std::abs(colorDiff) + std::abs(depthDiff) + std::abs(stencilDiff) + std::abs(antialiasingDiff);

		if (settings.sRgb && !sRgb)
			score += 10000000;

		if (!accelerated)
			score += 100000000;

		return score;
	}

	void OpenGLContext::initialize(const RenderSettings& settings)
	{
		setActive(true);

		int32 majorVersion = 0;
		int32 minorVersion = 0;

		glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
		glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

		if (glGetError() != GL_INVALID_ENUM)
		{
			_settings.majorVersion = static_cast<unsigned int>(majorVersion);
			_settings.minorVersion = static_cast<unsigned int>(minorVersion);
		}

		_settings.attributeFlags = RenderSettings::Default;

		if (_settings.majorVersion >= 3)
		{
			int32 flags = 0;
			glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

			if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
				_settings.attributeFlags |= RenderSettings::Debug;

			if ((_settings.majorVersion == 3) && (_settings.minorVersion == 1))
			{
				_settings.attributeFlags |= RenderSettings::Core;

				glGetStringiFuncType glGetStringiFunc = reinterpret_cast<glGetStringiFuncType>(getFunction("glGetStringi"));

				if (glGetStringiFunc)
				{
					int numExtensions = 0;
					glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

					for (unsigned int i = 0; i < static_cast<unsigned int>(numExtensions); ++i)
					{
						const char* extensionString = reinterpret_cast<const char*>(glGetStringiFunc(GL_EXTENSIONS, i));

						if (std::strstr(extensionString, "GL_ARB_compatibility"))
						{
							_settings.attributeFlags &= ~static_cast<uint32>(RenderSettings::Core);
							break;
						}
					}
				}
			}
			else if ((_settings.majorVersion > 3) || (_settings.minorVersion >= 2))
			{
				int32 profile = 0;
				glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);

				if (profile & GL_CONTEXT_CORE_PROFILE_BIT)
					_settings.attributeFlags |= RenderSettings::Core;
			}
		}

		if ((settings.antialiasingLevel > 0) && (_settings.antialiasingLevel > 0))
		{
			glEnable(GL_MULTISAMPLE);
		}
		else
		{
			_settings.antialiasingLevel = 0;
		}

		if (settings.sRgb && _settings.sRgb)
		{
			glEnable(GL_FRAMEBUFFER_SRGB);

			if (glIsEnabled(GL_FRAMEBUFFER_SRGB) == GL_FALSE)
			{
				LOG(Error, CoreEngine, "Failed to enable GL_FRAMEBUFFER_SRGB");
				_settings.sRgb = false;
			}
		}
		else
		{
			_settings.sRgb = false;
		}
	}

	void OpenGLContext::compareSettings(const RenderSettings& settings)
	{
		const char* vendorName = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
		const char* rendererName = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

		if (vendorName && rendererName)
		{
			if ((std::strcmp(vendorName, "Microsoft Corporation") == 0) && (std::strcmp(rendererName, "GDI Generic") == 0))
			{
				LOG(Warning, CoreEngine, "Detected \"Microsoft Corporation GDI Generic\" OpenGL implementation");
				LOG(Warning, CoreEngine, "The current OpenGL implementation is not hardware-accelerated");
			}
		}

		int version = _settings.majorVersion * 10 + _settings.minorVersion;
		int requestedVersion = settings.majorVersion * 10 + settings.minorVersion;

		if ((_settings.attributeFlags != settings.attributeFlags) ||
			(version                      <  requestedVersion) ||
			(_settings.stencilBits       <  settings.stencilBits) ||
			(_settings.antialiasingLevel <  settings.antialiasingLevel) ||
			(_settings.depthBits         <  settings.depthBits) ||
			(!_settings.sRgb      && settings.sRgb))
		{
			LOG(Warning, CoreEngine, "The requested OpenGL context settings were not fully met");

			LOGF(Info, CoreEngine, "Requeseted: version = %d.%d ; depth bits = %d ; stencil bits = %d ; AA level = %d ; core = %s ; debug = %s ; sRGB = %s", 
				settings.majorVersion, settings.minorVersion, settings.depthBits, settings.stencilBits, settings.antialiasingLevel, 
				settings.attributeFlags & RenderSettings::Core ? "true" : "false", settings.attributeFlags & RenderSettings::Debug ? "true" : "false",
				settings.sRgb ? "true" : "false");

			LOGF(Info, CoreEngine, "Created: version = %d.%d ; depth bits = %d ; stencil bits = %d ; AA level = %d ; core = %s ; debug = %s ; sRGB = %s",
				_settings.majorVersion, _settings.minorVersion, _settings.depthBits, _settings.stencilBits, _settings.antialiasingLevel,
				_settings.attributeFlags & RenderSettings::Core ? "true" : "false", _settings.attributeFlags & RenderSettings::Debug ? "true" : "false",
				_settings.sRgb ? "true" : "false");
		}
	}
}