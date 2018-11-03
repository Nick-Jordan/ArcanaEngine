#ifndef TIMELINE_H_
#define TIMELINE_H_

#include "CoreDefines.h"

#include "Object.h"
#include "ArcanaLog.h"

#include "Array.h"
#include "Event.h"
#include "EventHandler.h"

#include "CoreLoggers.h"

namespace Arcana
{

	REGISTER_CALLBACK(TimelineFinished);


	class ARCANA_CORE_API Timeline : public Object
	{
	public:
	
		enum LengthMode
		{
			TimelineLength,
			LastKeyFrame
		};
	
		struct EventEntry
		{
			Event event;

			double time;
		};
	
		Timeline();
		
		
		void play();
		
		void playFromStart();
		
		void reverse();
		
		void reverseFromEnd();
		
		void stop();
		
		bool isPlaying() const;
		
		bool isReversing() const;
		
		void setPlaybackPosition(double position, bool fireEvents);// , fireUpdate = true
		
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
		
		void addEvent(double time, Event event);

		void updateTimeline(double deltaTime);

		void setEventHandler(EventHandler& eventHandler);

		TimelineFinished& getTimelineFinishedCallback();

		TimelineFinished& getTimelineFinishedCallbackStatic();
		
	private:
	
		double getLastKeyframeTime() const;
		
	
		LengthMode _lengthMode;
		
		double _length;
		
		bool _looped;
		
		bool _reversed;
		
		bool _playing;
		
		double _timeScale;
		
		double _position;
		
		Array<EventEntry> _events;
		
		//array of interpolated vectors
		
		//array of interpolated floats
		
		//array of interpolated linear colors
		
		//timeline events

		TimelineFinished _staticCallback;

		TimelineFinished _memberCallback;

		EventHandler* _eventHandler;
	};
}

#endif // !TIMELINE_H_