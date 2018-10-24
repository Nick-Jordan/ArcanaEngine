#ifndef ACTOR_H_
#define ACTOR_H_

#include "SceneDefines.h"

#include "BaseObject.h"
#include "Transform.h"
#include "Shape.h"

//test
#include "Material.h"
#include "RenderState.h"

namespace Arcana
{
	class ARCANA_SCENE_API Actor : public BaseObject
	{
	public:

		Actor();

		Actor(const Actor& actor);

		virtual ~Actor();


		virtual void update(double elapsedTime);

		virtual void render(ObjectRenderer& renderer);


		Transform& getTransform();

		void setTransform(const Transform& transform);

		Shape* getShape() const;

		void setShape(Shape& shape);


		//test
		Material* test;
		RenderState testRenderState;

	private:

		Shape* _shape;

		Transform _transform;
	};

}

#endif // !ACTOR_H_

