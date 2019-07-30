#include "Timeline.h"

#include "ArcanaMath.h"

namespace Arcana
{
	std::mutex mutex;

	Timeline::Timeline() : Object("Timeline"), 
		_lengthMode(TimelineLength),
		_length(0.0),
		_looped(false),
		_reversed(false),
		_playing(false),
		_timeScale(1.0),
		_position(0.0),
		_eventHandler(nullptr),
		_currentFloat(0.0f),
		_currentVector(Vector3f::zero()),
		_currentLinearColor(0.0f, 0.0f, 0.0f, 0.0f)
	{
	}
	
	Timeline::~Timeline()
	{
		for (auto i = _events.createIterator(); i; i++)
		{
			AE_DELETE((*i).event);
		}
	}
		
	void Timeline::play()
	{
		_reversed = false;
		_playing = true;
	}
		
	void Timeline::playFromStart()
	{
		setPlaybackPosition(0.0, false);
		play();
	}
		
	void Timeline::reverse()
	{
		_reversed = true;
		_playing = true;
	}
		
	void Timeline::reverseFromEnd()
	{
		setPlaybackPosition(getTimelineLength(), false);
		reverse();
	}
		
	void Timeline::stop()
	{
		_playing = false;
	}
		
	bool Timeline::isPlaying() const
	{
		return _playing;
	}
		
	bool Timeline::isReversing() const
	{
		return _playing && _reversed;
	}
		
	void Timeline::setPlaybackPosition(double position, bool fireEvents)//, fireUpdate = true
	{
		double oldPosition = _position;

		_position = position;

		double minTime, maxTime;
		if (!_reversed)
		{
			minTime = oldPosition;
			maxTime = position;

			if (maxTime == getTimelineLength())
			{
				maxTime += Math::EPSILON;
			}
		}
		else
		{
			minTime = position;
			maxTime = oldPosition;

			if (minTime == 0.0)
			{
				minTime -= Math::EPSILON;
			}
		}

		if (fireEvents)
		{
			for (int32 i = 0; i < _events.size(); i++)
			{
				double eventTime = _events[i].time;

				bool fireThisEvent = false;
				if (!_reversed)
				{
					if (eventTime >= minTime && eventTime < maxTime)
					{
						fireThisEvent = true;
					}
				}
				else
				{
					if (eventTime > minTime && eventTime <= maxTime)
					{
						fireThisEvent = true;
					}
				}

				if (fireThisEvent)
				{
					if (_eventHandler && _events[i].event)
					{
						_eventHandler->broadcast(*_events[i].event);
					}
					_events[i].trigger.executeIfBound();
				}
			}
		}

		//mutex.lock();
		if (_floats.size() > 0)
		{
			if (_floats.size() == 1)
			{
				if (_position >= _floats[0].time)
					_currentFloat = _floats[0].f;
			}
			else
			{
				for (int32 i = 0; i < _floats.size() - 1; i++)
				{
					double t0 = _floats[i].time;
					double t1 = _floats[i + 1].time;
					//reverse
					if (_position >= t0 && _position <= t1)
					{
						float f0 = _floats[i].f;
						float f1 = _floats[i + 1].f;

						_currentFloat = Math::lerp(f0, f1, Math::range(_position, t0, t1, 0.0, 1.0));
					}
				}
			}
		}

		if (_vectors.size() > 0)
		{
			if (_vectors.size() == 1)
			{
				if (_position >= _vectors[0].time)
					_currentVector = _vectors[0].v;
			}
			else
			{
				for (int32 i = 0; i < _vectors.size() - 1; i++)
				{
					double t0 = _vectors[i].time;
					double t1 = _vectors[i + 1].time;
					//reverse
					if (_position >= t0 && _position <= t1)
					{
						Vector3f f0 = _vectors[i].v;
						Vector3f f1 = _vectors[i + 1].v;

						_currentVector = Vector3f::lerp(f0, f1, Math::range(_position, t0, t1, 0.0, 1.0));
					}
				}
			}
		}

		if (_linearColors.size() > 0)
		{
			if (_linearColors.size() == 1)
			{
				if(_position >= _linearColors[0].time)
					_currentLinearColor = _linearColors[0].lc;
			}
			else
			{
				for (int32 i = 0; i < _linearColors.size() - 1; i++)
				{
					double t0 = _linearColors[i].time;
					double t1 = _linearColors[i + 1].time;
					//reverse
					if (_position >= t0 && _position <= t1)
					{
						LinearColor f0 = _linearColors[i].lc;
						LinearColor f1 = _linearColors[i + 1].lc;

						_currentLinearColor = LinearColor::lerp(f0, f1, Math::range(_position, t0, t1, 0.0, 1.0));
					}
				}
			}
		}
		//mutex.unlock();
	}
		
	double Timeline::getPlaybackPosition() const
	{
		return _position;
	}
		
	void Timeline::setLooped(bool looped)
	{
		_looped = looped;
	}
		
	bool Timeline::isLooped() const
	{
		return _looped;
	}
		
	void Timeline::setTimeScale(double timeScale)
	{
		_timeScale = timeScale;
	}
		
	double Timeline::getTimeScale() const
	{
		return _timeScale;
	}
		
	void Timeline::setNewTime(double time)
	{
		time = Math::clamp(time, 0.0, _length);
		setPlaybackPosition(time, false);
	}
		
	void Timeline::setTimelineLength(double length)
	{
		_length = length;
		if (_position > length)
		{
			setNewTime(length - Math::EPSILON);
		}
	}
		
	double Timeline::getTimelineLength() const
	{
		switch (_lengthMode)
		{
		case TimelineLength:
			return _length;
		case LastKeyFrame:
			return getLastKeyframeTime();
		default:
			LOG(Error, LogTimeline, "Invalid timeline length mode on timeline!");
			return 0.0;
		}
	}
		
	void Timeline::setTimelineLengthMode(LengthMode mode)
	{
		_lengthMode = mode;
	}

	void Timeline::addEvent(double time, Event* event)
	{
		EventEntry entry;
		entry.time = time;
		entry.event = event;

		_events.add(entry);
	}

	void Timeline::addTrigger(double time, const TimelineTrigger& trigger)
	{
		EventEntry entry;
		entry.time = time;
		entry.event = nullptr;
		entry.trigger = trigger;

		_events.add(entry);
	}

	void Timeline::addVector(double time, const Vector3f& v)
	{
		VectorEntry entry;
		entry.time = time;
		entry.v = v;

		_vectors.add(entry);
	}

	void Timeline::addFloat(double time, const float& f)
	{
		FloatEntry entry;
		entry.time = time;
		entry.f = f;

		_floats.add(entry);
	}

	void Timeline::addLinearColor(double time, const LinearColor& lc)
	{
		LinearColorEntry entry;
		entry.time = time;
		entry.lc = lc;

		_linearColors.add(entry);
	}

	const Vector3f& Timeline::getCurrentVector() const
	{
		//std::lock_guard<std::mutex> lock(mutex);
		return _currentVector;
	}

	const float& Timeline::getCurrentFloat() const
	{
		//std::lock_guard<std::mutex> lock(mutex);
		return _currentFloat;
	}

	const LinearColor& Timeline::getCurrentLinearColor() const
	{
		//std::lock_guard<std::mutex> lock(mutex);
		return _currentLinearColor;
	}

	void Timeline::updateTimeline(double deltaTime)
	{
		bool isFinished = false;

		if (_playing)
		{
			const double timelineLength = getTimelineLength();
			const double effectiveDeltaTime = deltaTime * (_reversed ? (-_timeScale) : (_timeScale));

			double newPosition = _position + effectiveDeltaTime;

			if (effectiveDeltaTime > 0.0)
			{
				if (newPosition > timelineLength)
				{
					if (_looped)
					{
						setPlaybackPosition(timelineLength, true);
						setPlaybackPosition(0.0, false);

						if (timelineLength > 0.0)
						{
							while (newPosition > timelineLength)
							{
								newPosition -= timelineLength;
							}
						}
						else
						{
							newPosition = 0.0;
						}
					}
					else
					{
						newPosition = timelineLength;
						stop();
						isFinished = true;
					}
				}
			}
			else
			{
				if (newPosition < 0.0)
				{
					if (_looped)
					{
						setPlaybackPosition(0.0, true);
						setPlaybackPosition(timelineLength, false);

						if (timelineLength > 0.0)
						{
							while (newPosition < 0.0)
							{
								newPosition += timelineLength;
							}
						}
						else
						{
							newPosition = 0.0;
						}
					}
					else
					{
						newPosition = 0.0;
						stop();
						isFinished = true;
					}
				}
			}

			setPlaybackPosition(newPosition, true);
		}

		if (isFinished)
		{
			_staticCallback.executeIfBound();
			_memberCallback.executeIfBound();
		}
	}

	void Timeline::setEventHandler(EventHandler& eventHandler)
	{
		_eventHandler = &eventHandler;
	}

	TimelineFinished& Timeline::getTimelineFinishedCallback()
	{
		return _memberCallback;
	}

	TimelineFinished& Timeline::getTimelineFinishedCallbackStatic()
	{
		return _staticCallback;
	}

	double Timeline::getLastKeyframeTime() const
	{
		double maxTime = 0.0;

		for (auto it = _events.createConstIterator(); it; ++it)
		{
			const EventEntry& currentEvent = *it;
			maxTime = std::max(currentEvent.time, maxTime);
		}

		for (auto it = _floats.createConstIterator(); it; ++it)
		{
			const FloatEntry& currentFloat = *it;
			maxTime = std::max(currentFloat.time, maxTime);
		}

		for (auto it = _vectors.createConstIterator(); it; ++it)
		{
			const VectorEntry& currentVector = *it;
			maxTime = std::max(currentVector.time, maxTime);
		}

		for (auto it = _linearColors.createConstIterator(); it; ++it)
		{
			const LinearColorEntry& currentLinearColor = *it;
			maxTime = std::max(currentLinearColor.time, maxTime);
		}

		return maxTime;
	}
}