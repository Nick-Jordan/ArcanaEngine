#include "PostProcessQueue.h"

#include "GraphicsLoggers.h"

namespace Arcana
{

	PostProcessQueue::PostProcessQueue()
	{
	}


	PostProcessQueue::~PostProcessQueue()
	{
	}

	void PostProcessQueue::setBaseEffect(PostProcessEffect* effect, bool enabled)
	{
		if (effect)
		{
			if (enabled)
				effect->enable();
			else
				effect->disable();

			_base = effect;
		}
	}

	void PostProcessQueue::setEndEffect(PostProcessEffect* effect, bool enabled)
	{
		if (effect)
		{
			if (enabled)
				effect->enable();
			else
				effect->disable();

			_end = effect;
		}
	}

	void PostProcessQueue::addEffect(PostProcessEffect* effect, bool enabled)
	{
		if (effect)
		{
			if (enabled)
				effect->enable();
			else
				effect->disable();

			_queue.push_back(effect);
		}
	}

	/*void PostProcessQueue::setEffect(PostProcessEffect* input, PostProcessEffect* effect, bool enabled)
	{

	}*/

	void PostProcessQueue::build()
	{

	}

	void PostProcessQueue::buildCenter()
	{

	}

	void PostProcessQueue::toggleEffect(const std::string& name)
	{
		PostProcessEffect* effect = get(name);

		if (effect)
		{
			effect->toggleEffect();
		}
	}

	void PostProcessQueue::enableEffect(const std::string& name)
	{
		PostProcessEffect* effect = get(name);

		if (effect)
		{
			effect->enable();
		}
	}

	void PostProcessQueue::disableEffect(const std::string& name)
	{
		PostProcessEffect* effect = get(name);

		if (effect)
		{
			effect->disable();
		}
	}

	PostProcessEffect* PostProcessQueue::get(const std::string& name)
	{
		if(_base && _base->getName() == name)
		{
			return _base;
		}
		if (_end && _end->getName() == name)
		{
			return _end;
		}

		for (auto i = _queue.begin(); i != _queue.end(); i++)
		{
			PostProcessEffect* effect = *i;

			if (effect->getName() == name)
			{
				return effect;
			}
		}

		LOGF(Warning, PostProcessQueueLog, "Effect, \'%s\', does not exist in this queue!", name.c_str());
		return nullptr;
	}

	void PostProcessQueue::process(std::vector<PostProcessEffect*>& queue) const
	{
		queue.clear();

		queue.resize(2 + _queue.size());

		queue[0] = _base;
		queue.insert(queue.begin() + 1, _queue.begin(), _queue.end());
		queue[queue.size() - 1] = _end;
	}
}
