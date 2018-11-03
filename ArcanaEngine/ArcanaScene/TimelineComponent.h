#ifndef TIMELINE_COMPONENT_H_
#define TIMELINE_COMPONENT_H_

#include "SceneDefines.h"

#include "ActorComponent.h"
#include "Timeline.h"

namespace Arcana
{

	class ARCANA_SCENE_API TimelineComponent : public ActorComponent
	{
	public:

		TimelineComponent();

		~TimelineComponent();


		double getCurrentPosition() const;

		void setLength(double length);


		virtual void update(double elapsedTime) override;

	private:

		Timeline _timeline;
	};

}

#endif // !TIMELINE_COMPONENT_H_

