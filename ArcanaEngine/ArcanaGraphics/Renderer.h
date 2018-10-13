#ifndef RENDERER_H_
#define RENDERER_H_

#include "GraphicsDefines.h"

#include "Types.h"
#include "RenderContext.h"
#include "Timeline.h"
#include "Timer.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API Renderer : public WindowRenderer
	{
	public:
	
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
		
		static uint64 getActiveContextId();
		
	private:
	
		RenderContext* _context;

		Timeline _timeline;

		Timer _timer;
	};
}

#endif