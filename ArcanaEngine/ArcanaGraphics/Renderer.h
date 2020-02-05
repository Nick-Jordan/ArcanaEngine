#ifndef RENDERER_H_
#define RENDERER_H_

#include "GraphicsDefines.h"

#include "Types.h"
#include "RenderContext.h"
#include "Timeline.h"
#include "Timer.h"

#include "WorldRenderer.h"
#include "Mesh.h"
#include "Shader.h"

#include <atomic>

//test
#define DISABLE_SPLASH

namespace Arcana
{
	class ARCANA_GRAPHICS_API SplashScreen
	{
	public:

		bool renderingSplash;
		double splashLength;
		double splashTime;
		Texture* texture;

	public:

		SplashScreen();

		~SplashScreen();

		void render();
	};

	class ARCANA_GRAPHICS_API Renderer : public WindowRenderer
	{
	public:
	
		enum ClearFlags
		{
			ClearColor = 1 << 0,
			ClearDepth = 1 << 1,
			ClearStencil = 1 << 2,
			ClearColorDepth = ClearColor | ClearDepth,
			ClearColorStencil = ClearColor | ClearStencil,
			ClearDepthStencil = ClearDepth | ClearStencil,
			ClearColorDepthStencil = ClearColor | ClearDepth | ClearStencil
		};

		//threadLocalPtr
		static Renderer* ActiveRenderer;
	
		Renderer();

		Renderer(const RenderSettings& settings, Window* owner);

		Renderer(const RenderSettings& settings, uint32 width, uint32 height);

		~Renderer();
		
		bool setActive(bool active);

		virtual void render() override;

		virtual void setVerticalSync(bool enabled) override;
		
		const RenderSettings& getSettings() const;

		Timeline& getTimeline();
		
		void setWorldRenderer(WorldRenderer* renderer);

		WorldRenderer* getWorldRenderer() const;

		bool isFinishedRenderingSplash() const;

		static uint64 getActiveContextId();

		void clear(ClearFlags flags, const Vector4f& clearColor, float clearDepth, int32 clearStencil);

		void clear(ClearFlags flags, float red, float green, float blue, float alpha, float clearDepth, int32 clearStencil);
		
	private:
	
		RenderContext* _context;

		Timeline _timeline;

		Timer _timer;

		WorldRenderer* _worldRenderer;

		SplashScreen _splashScreen;

		std::atomic<bool> _finishedSplash;

#ifdef DISABLE_SPLASH
		bool _splashEnabled = false;
#else
		bool _splashEnabled = true;
#endif //DISABLE_SPLASH
	};
}

#endif