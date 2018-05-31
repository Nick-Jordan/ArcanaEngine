// EngineClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdlib>

#include "Globals.h"
#include "Array.h"
#include "SmartPtr.h"
#include "GlobalObjectID.h"
#include "Timeline.h"
#include "Callback.h"

#include "EventHandler.h"
#include "KeyListener.h"
#include "KeyEvent.h"

#include <iostream>

using namespace Arcana;

REGISTER_CALLBACK(TestCallback, std::string, float);

static void testCallbackFunction(std::string string, float f)
{
	std::cout << string << " " << std::to_string(f) << std::endl;
}

int main()
{
	{
		GEngine = new Engine();

		std::cout << GEngine->getType() << std::endl;

		SmartPtr<> p(new Object());
		p->yee();
		std::cout << p.getReferenceCount() << std::endl;
		{
			SmartPtr<> q = p;
			std::cout << q.getReferenceCount() << std::endl;
			q->yee();

			SmartPtr<> r;
			r = p;
			std::cout << r.getReferenceCount() << std::endl;
			r->yee();
		}
		std::cout << p.getReferenceCount() << std::endl;
		p->yee();


		Array<int32> array;

		array.init(10, 5);

		array.rangeCheck(-1);
		array.add(10);
		array.add(10);
		array.add(10);
		array.add(10);
		array.add(3);
		array.add(2);
		array.add(1);
		array.insert(0, 1);
		array.remove(3);
		array.emplace(102);

		int32 nums[] = { 53, 2, 43 };
		array.append(nums, 3);

		array.addUnique(63);
		array.addUnique(63);

		array.setSize(20);

		array.removeSwap(2);

		std::cout << array.getSlack() << std::endl;

		std::cout << array.size() << ", " << array.max() << std::endl;

		array.shrink();

		std::cout << array.size() << ", " << array.max() << std::endl;

		for (auto it = array.createConstIterator(); it; ++it)
		{
			std::cout << *it << " ";
		}
		std::cout << std::endl;

		GlobalObjectID id = GlobalObjectID("aaaa");

		std::cout << id.getId() << std::endl;


		Timeline timeline = Timeline();
		timeline.setLooped(false);
		timeline.setTimelineLength(10.0);
		timeline.setTimeScale(0.5);

		timeline.updateTimeline(1.0);
		timeline.updateTimeline(2.0);

		timeline.play();

		timeline.updateTimeline(1.0);
		timeline.updateTimeline(2.0);

		std::cout << "Timeline Position: " << timeline.getPlaybackPosition() << std::endl;

		TestCallback callback;
		std::cout << "Callback isBound: " << callback.isBound() << std::endl;

		callback.bind(testCallbackFunction);
		callback.execute("string", 0.1f);

		callback.executeIfBound("a", 2.0f);
		callback.unbind();
		callback.executeIfBound("b", 3.0f);


		std::cout << std::endl << "Events:" << std::endl << std::endl;

		KeyListener* listener = new KeyListener();
		GEngine->eventHandler.addEventListener(listener);

		KeyEvent event = KeyEvent(KeyEvent::Pressed, 34, false, false, false, false);
		GEngine->eventHandler.broadcast(event);
		GEngine->eventHandler.broadcast(event);

		/*KeyEvent timelineEvent = KeyEvent(KeyEvent::Released, 21, false, false, false, false);

		timeline.addEvent(3.5, timelineEvent);
		timeline.updateTimeline(2.0);
		std::cout << "Timeline Position: " << timeline.getPlaybackPosition() << std::endl;
		timeline.updateTimeline(2.2);
		std::cout << "Timeline Position: " << timeline.getPlaybackPosition() << std::endl;*/

		GEngine->eventHandler.removeEventListener(listener);
		delete listener;

	}

	system("pause");

    return 0;
}

