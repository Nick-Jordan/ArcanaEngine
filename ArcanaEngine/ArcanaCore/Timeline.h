#ifndef TIMELINE_H_
#define TIMELINE_H_

#include "CoreDefines.h"

#include "Object.h"
#include "ArcanaLog.h"

#include "Array.h"
#include "Event.h"
#include "EventHandler.h"
#include "Vector3.h"
#include "Color.h"

#include <mutex>

#include "CoreLoggers.h"

namespace Arcana
{
	/** \brief TimelineFinished callback typedef.
	 *
	 *  This callback is executed when the Timeline finishes.
	 */
	REGISTER_CALLBACK(TimelineFinished);

	REGISTER_CALLBACK(TimelineTrigger);

	/** \brief This class manages a timeline.
	 *
	 *  Events can be scheduled at specific times on the timeline.
	 *  The engine manages a timeline of its own.
	 */
	class ARCANA_CORE_API Timeline : public Object
	{
	public:
	
		/// Length mode enum.
		enum LengthMode
		{
			TimelineLength, ///< The timeline stops at a set length.
			LastKeyFrame    ///< The timeline stops at the last event keyframe.
		};
	
		/** \brief An event entry.
		 */
		struct EventEntry
		{
			Event* event;	 ///< The event to be broadcasted.
			TimelineTrigger trigger; ///< The trigger callback.

			double time;	 ///< The time to broadcast the event.
		};

		/** \brief A vector entry.
		 */
		struct VectorEntry
		{
			Vector3f v;
			double time;
		};

		/** \brief A float entry.
		 */
		struct FloatEntry
		{
			float f;
			double time;
		};

		/** \brief A linear color entry.
		 */
		struct LinearColorEntry
		{
			LinearColor lc;
			double time;
		};
	
		/** \brief Timeline default constructor.
		 *
		 *  Sets length and position to zero.
		 *  Does not start the timeline!
		 */
		Timeline();

		~Timeline();
		
		/** \brief Starts the timeline.
		 */
		void play();
		
		/** \brief Resets the position to zero and starts the timeline.
		 */
		void playFromStart();
		
		/** \brief Starts the timeline in reverse.
		 */
		void reverse();
		
		/** \brief Resets the position to the end and starts the timeline in reverse.
		 */
		void reverseFromEnd();
		
		/** \brief Stops the timeline.
		 */
		void stop();
		
		/** \brief Returns true if the timeline is playing.
		 */
		bool isPlaying() const;
		
		/** \brief Returns true if the timeline is reversing.
		 */
		bool isReversing() const;
		
		/** \brief Mutator for the timeline position.
		 *
		 *  If fireEvents is 'true' all events scheduled prior to 'position' will be broadcasted.
		 */
		void setPlaybackPosition(double position, bool fireEvents);// , fireUpdate = true
		
		/** \brief Accessor for the timeline position.
		 */
		double getPlaybackPosition() const;
		
		/** \brief Mutator for looping the timeline.
		 */
		void setLooped(bool looped);
		
		/** \brief Returns true if the timeline is looped.
		 */
		bool isLooped() const;
		
		/** \brief Mutator for the time scale.
		 *
		 *  Time deltas are multiplied by the time scale.
		 */
		void setTimeScale(double timeScale);
		
		/** \brief Accessor for the time scale.
		 */
		double getTimeScale() const;
		
		/** \brief Sets the timeline position.
		 *
		 *  Same as setPlaybackPosition.
		 *  Doesn't broadcast events.
		 */
		void setNewTime(double time);
		
		/** \brief Sets the timeline length.
		 */
		void setTimelineLength(double length);
		
		/** \brief Accessor for the timeline length.
		 */
		double getTimelineLength() const;
		
		/** \brief Sets the timeline length mode.
		 */
		void setTimelineLengthMode(LengthMode mode);
		
		//addEvent and interpolated vectors/floats/colors functions
		
		/** \brief Schedules an Event to be broadcasted at the specified time.
		 */
		void addEvent(double time, Event* event);

		/** \brief Schedules a trigger to be broadcasted at a specified time
		 */
		void addTrigger(double time, const TimelineTrigger& trigger);

		void addVector(double time, const Vector3f& v);

		void addFloat(double time, const float& f);

		void addLinearColor(double time, const LinearColor& lc);

		const Vector3f& getCurrentVector() const;

		const float& getCurrentFloat() const;

		const LinearColor& getCurrentLinearColor() const;

		/** \brief Updates the timeline.
		 */
		void updateTimeline(double deltaTime);
		
		/** \brief Sets the EventHandler for this timeline.
		 *
		 *  All scheduled events will be broadcasted with this event handler.
		 */
		void setEventHandler(EventHandler& eventHandler);

		/** \brief Returns a reference to the member function TimelineFinished callback.
		 */
		TimelineFinished& getTimelineFinishedCallback();
		
	private:
	
		/** \brief Gets the largest time in the EventEntry array.
		 */
		double getLastKeyframeTime() const;
		
	
		LengthMode _lengthMode;		///< The length mode.
		
		double _length;				///< The timeline length.
		bool _looped;				///< Whether or not the timeline loops.		
		bool _reversed;				///< Whether or not the timeline is in reverse.	
		bool _playing;				///< Whether or not the timeline is playing.		
		double _timeScale;			///< The time scale.		
		double _position;			///< The current timeline position.
		
		Array<EventEntry> _events;  ///< Array of scheduled events.		
		Array<VectorEntry> _vectors;
		Array<FloatEntry> _floats;
		Array<LinearColorEntry> _linearColors;

		Vector3f _currentVector;
		float _currentFloat;
		LinearColor _currentLinearColor;
						
		TimelineFinished _finishedCallback;  ///< TimelineFinished callback.

		EventHandler* _eventHandler;	   ///< This timeline's EventHandler.
	};
}

#endif // !TIMELINE_H_