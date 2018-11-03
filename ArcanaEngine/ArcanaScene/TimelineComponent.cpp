#include "TimelineComponent.h"

namespace Arcana
{

	TimelineComponent::TimelineComponent()
	{
		_timeline.play();
	}


	TimelineComponent::~TimelineComponent()
	{
		_timeline.stop();
	}

	double TimelineComponent::getCurrentPosition() const
	{
		return _timeline.getPlaybackPosition();
	}

	void TimelineComponent::setLength(double length)
	{
		_timeline.setTimelineLengthMode(Timeline::LengthMode::TimelineLength);
		_timeline.setTimelineLength(length);
	}

	void TimelineComponent::update(double elapsedTime)
	{
		_timeline.updateTimeline(elapsedTime);
	}
}
