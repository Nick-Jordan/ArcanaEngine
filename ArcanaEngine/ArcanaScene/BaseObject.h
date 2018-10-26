#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_

#include "SceneDefines.h"

#include <string>
#include "Object.h"

namespace Arcana
{

	class ARCANA_SCENE_API BaseObject : public Object
	{
	public:

		BaseObject();

		BaseObject(const BaseObject& object);

		virtual ~BaseObject();

		bool isActive() const;

		void setActive(bool active);

		const std::string& getName() const;

		void setName(const std::string& name);


		BaseObject& operator=(const BaseObject& object);

	private:

		bool _active;
		std::string _name;
	};

}

#endif // !BASE_OBJECT_H_
