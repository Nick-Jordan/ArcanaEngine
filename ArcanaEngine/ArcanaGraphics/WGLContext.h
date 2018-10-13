#ifndef WGL_CONTEXT_H_
#define WGL_CONTEXT_H_

#include "GraphicsDefines.h"

#include "opengl/include.h"
#include "OpenGLContext.h"

namespace Arcana
{
	typedef const GLubyte* (APIENTRY *glGetStringiFuncType)(GLenum, GLuint);

	class ARCANA_GRAPHICS_API WGLContext : public OpenGLContext
	{
	public:

		WGLContext(WGLContext* sharedContext);

		WGLContext(WGLContext* sharedContext, const RenderSettings& settings, const WindowContext* owner);

		WGLContext(WGLContext* sharedContext, const RenderSettings& settings, uint32 width, uint32 height);

		~WGLContext();


		virtual void render() override;

		virtual bool setCurrent(bool current) override;

		virtual void setVerticalSync(bool enabled) override;

		virtual bool initializeExtensions(OpenGLContext* sharedContext) override;


		static int selectBestPixelFormat(HDC deviceContext, const RenderSettings& settings, bool pbuffer = false);

		static std::string getErrorString(DWORD errorCode);

		static GLFunctionPointer getFunction(const char* name);

	private:

		void setDevicePixelFormat();

		void updateSettingsFromPixelFormat();

		void createSurface(WGLContext* shared, unsigned int width, unsigned int height);

		void createSurface(HWND window);

		void createContext(WGLContext* shared);

	private:

		HWND _window;
		HPBUFFERARB _pbuffer;
		HDC _deviceContext;
		HGLRC _context;
		bool _ownsWindow;
	};
}

#endif // !WGL_CONTEXT_H_