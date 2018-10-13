#include "Timeline.h"

#include "ArcanaMath.h"

namespace Arcana
{

	Timeline::Timeline() : Object("Timeline"), 
		_lengthMode(TimelineLength),
		_length(0.0),
		_looped(false),
		_reversed(false),
		_playing(false),
		_timeScale(1.0),
		_position(0.0)
	{
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

		if (fireEvents)
		{
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
					if (_eventHandler)
					{
						_eventHandler->broadcast(_events[i].event);
					}
				}
			}
		}
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

	void Timeline::addEvent(double time, Event event)
	{
		EventEntry entry;
		entry.time = time;
		entry.event = event;

		_events.add(entry);
	}
		
	//addEvent and interpolated vectors/floats/colors functions
		
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
			//TimelineFinishedFunc.ExecuteIfBound();
			//TimelineFinishFuncStatic.ExecuteIfBound();
		}
	}

	void Timeline::setEventHandler(EventHandler& eventHandler)
	{
		_eventHandler = &eventHandler;
	}

	double Timeline::getLastKeyframeTime() const
	{
		double maxTime = 0.0;

		for (auto it = _events.createConstIterator(); it; ++it)
		{
			const EventEntry& currentEvent = *it;
			maxTime = std::max(currentEvent.time, maxTime);
		}

		return maxTime;
	}
}