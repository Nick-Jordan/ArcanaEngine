#include "Renderer.h"

#include "ArcanaLog.h"
#include "ContextType.h"
#include "Application.h"

//test
#include "NoDataEvents.h"

namespace Arcana
{
	//threadLocalPtr
	Renderer* Renderer::ActiveRenderer = nullptr;
	
	
	Renderer::Renderer()
	{
		_context = RenderContext::create();

		setActive(true);
	}

	Renderer::Renderer(const RenderSettings& settings, Window* owner) : _context(nullptr)
	{
		_context = RenderContext::create(settings, owner->getWindowContext());

		owner->_renderer = this;

		_timeline.setEventHandler(owner->getParent()->getEventHandler());
		_timeline.setTimelineLengthMode(Timeline::LastKeyFrame);

		setActive(true);
	}

	Renderer::Renderer(const RenderSettings& settings, uint32 width, uint32 height) : _context(nullptr)
	{
		_context = RenderContext::create(settings, width, height);

		setActive(true);
	}
	
	Renderer::~Renderer()
	{
		setActive(false);
	}
	
	bool Renderer::setActive(bool active)
	{
		if (_context)
		{
			bool a = _context->setActive(active);

			if (a)
			{
				ActiveRenderer = (active ? this : nullptr);
			}

			if (active)
			{
				_timeline.play();
			}
			else
			{
				_timeline.stop();
			}

			return a;
		}
		
		return false;
	}

	void Renderer::render()
	{
		if (setActive(true))
		{
			if (_context)
			{
				double elapsedTime = _timer.reset().toSeconds();

				//LOGF(Error, CoreEngine, "Elapsed Time: %f", elapsedTime);
				//LOGF(Error, CoreEngine, "FPS: %f", 1.0/elapsedTime);
				//LOGF(Info, CoreEngine, "Timeline: %f", _timeline.getPlaybackPosition());

				_context->render();
				_timeline.updateTimeline(elapsedTime);
			}
		}
	}

	void Renderer::setVerticalSync(bool enabled)
	{
		if (_context)
		{
			_context->setVerticalSync(enabled);
		}
	}
	
	const RenderSettings& Renderer::getSettings() const
	{
		if (_context)
		{
			return _context->getSettings();
		}

		return RenderSettings();
	}

	Timeline& Renderer::getTimeline()
	{
		return _timeline;
	}
	
	uint64 Renderer::getActiveContextId()
	{
		return RenderContext::getActiveContextId();
	}
}