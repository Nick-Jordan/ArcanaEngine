#ifndef OPENGL_CONTEXT_H_
#define OPENGL_CONTEXT_H_

#include "GraphicsDefines.h"

#include "Types.h"
#include "RenderSettings.h"
#include "Window.h"
#include "Lock.h"
#include "Mutex.h"
#include "Vector4.h"

//test
#define ARCANA_PLATFORM_WINDOWS

namespace Arcana
{
	#ifdef ARCANA_PLATFORM_WINDOWS
	class ARCANA_GRAPHICS_API WGLContext;
	typedef WGLContext GLContext;
	#endif

	typedef void(*GLFunctionPointer)();

	class ARCANA_GRAPHICS_API OpenGLContext
	{
	public:

		//ThreadLocalPtr
		static OpenGLContext* CurrentContext;

	private:

		static uint64 Id;
	
		static Mutex ContextMutex;

		static GLContext* SharedContext;

	protected:

		static std::vector<std::string> Extensions;

	public:
		
		virtual ~OpenGLContext();
		
		
		bool setActive(bool active);
		
		const RenderSettings& getSettings() const;
		
		uint64 getContextId() const;
		
		virtual void render() = 0;
		
		virtual void setVerticalSync(bool enabled) = 0;


		static GLContext* create();

		static GLContext* create(const RenderSettings& settings, const WindowContext* owner);

		static GLContext* create(const RenderSettings& settings, unsigned int width, unsigned int height);

		static uint64 getActiveContextId();

		static bool hasExtension(const char* name);

		static GLFunctionPointer getFunction(const char* name);

		void clear(int32 flags, float red, float green, float blue, float alpha, float clearDepth, int32 clearStencil);
		
	protected:

		OpenGLContext();


		virtual bool setCurrent(bool current) = 0;

		virtual bool initializeExtensions(OpenGLContext* sharedContext) = 0;

		static int32 evaluateFormat(uint32 bitsPerPixel, const RenderSettings& settings, int32 colorBits, int32 depthBits, int32 stencilBits, int32 antialiasing, bool accelerated, bool sRgb);
	
		void initialize(const RenderSettings& settings);

		void compareSettings(const RenderSettings& settings);


		RenderSettings _settings;

	private:

		GLContext* _context;
		
		uint64 _id;

		Vector4f _clearColor;
		float _clearDepth;
		int32 _clearStencil;
	};
}

#endif // !OPENGL_CONTEXT_H_