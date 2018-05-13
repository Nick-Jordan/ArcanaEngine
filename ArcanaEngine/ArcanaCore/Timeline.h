#ifndef TIMELINE_H_
#define TIMELINE_H_

#include "ArcanaCoreAPIDefinition.h"

#include "Object.h"
#include "ArcanaLog.h"

namespace Arcana
{
	REGISTER_CATEGORY(LogTimeline, none)

	class ARCANA_CORE_API Timeline : public Object
	{
	public:
	
		enum LengthMode
		{
			TimelineLength,
			LastKeyFrame
		};
	
	
		Timeline();
		
		
		void play();
		
		void playFromStart();
		
		void reverse();
		
		void reverseFromEnd();
		
		void stop();
		
		bool isPlaying() const;
		
		bool isReversing() const;
		
		void setPlaybackPosition(double position); //fireEvents, fireUpdate = true
		
		double getPlaybackPosition() const;
		
		void setLooped(bool looped);
		
		bool isLooped() const;
		
		void setTimeScale(double timeScale);
		
		double getTimeScale() const;
		
		void setNewTime(double time);
		
		void setTimelineLength(double length);
		
		double getTimelineLength() const;
		
		void setTimelineLengthMode(LengthMode mode);
		
		//addEvent and interpolated vectors/floats/colors functions
		
		void updateTimeline(double deltaTime);
		
	private:
	
		double getLastKeyframeTime() const;
		
	
		LengthMode _lengthMode;
		
		double _length;
		
		bool _looped;
		
		bool _reversed;
		
		bool _playing;
		
		double _timeScale;
		
		double _position;
		
		//array of events
		
		//array of interpolated vectors
		
		//array of interpolated floats
		
		//array of interpolated linear colors
		
		//timeline events
	};
}

#endif // !TIMELINE_H_