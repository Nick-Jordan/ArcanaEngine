#include "Object.h"

#include "ArcanaLog.h"

namespace Arcana
{

	Object::Object()
	{
	}


	Object::~Object()
	{
	}

	void Object::yee()
	{
		LOG(Debug, CoreEngine, "yee");
	}
}