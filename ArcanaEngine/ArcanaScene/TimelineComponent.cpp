#include "TimelineComponent.h"

namespace Arcana
{

	TimelineComponent::TimelineComponent()
	{
		_timeline.setTimelineLengthMode(Timeline::Indefinite);
	}


	TimelineComponent::~TimelineComponent()
	{
	}

	double TimelineComponent::getCurrentPosition() const
	{
		return _timeline.getPlaybackPosition();
	}

	void TimelineComponent::update(double elapsedTime)
	{
		_timeline.updateTimeline(elapsedTime);
	}
}
