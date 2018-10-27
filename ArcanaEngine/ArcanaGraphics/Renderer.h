#ifndef RENDERER_H_
#define RENDERER_H_

#include "GraphicsDefines.h"

#include "Types.h"
#include "RenderContext.h"
#include "Timeline.h"
#include "Timer.h"

#include "WorldRenderer.h"

namespace Arcana
{
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

		static uint64 getActiveContextId();

		void clear(ClearFlags flags, const Vector4f& clearColor, float clearDepth, int32 clearStencil);

		void clear(ClearFlags flags, float red, float green, float blue, float alpha, float clearDepth, int32 clearStencil);
		
	private:
	
		RenderContext* _context;

		Timeline _timeline;

		Timer _timer;

		WorldRenderer* _worldRenderer;
	};
}

#endif