#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_

#include "SceneDefines.h"

#include <string>

namespace Arcana
{

	class ARCANA_SCENE_API BaseObject
	{
	public:

		BaseObject();

		BaseObject(const BaseObject& object);

		virtual ~BaseObject();

		bool isActive() const;

		void setActive(bool active);

		void* getUserData() const;

		void setUserData(void* data);

		const std::string& getName() const;

		void setName(const std::string& name);


		BaseObject& operator=(const BaseObject& object);

	private:

		bool _active;
		void* _userData;
		std::string _name;
	};

}

#endif // !BASE_OBJECT_H_
