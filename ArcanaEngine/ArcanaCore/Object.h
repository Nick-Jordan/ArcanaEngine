#ifndef OBJECT_H_
#define OBJECT_H_

#include "CoreDefines.h"

#include <string>
#include "Types.h"

namespace Arcana
{
	/** \brief The base class for all core classes.
	 *
	 */

	class ARCANA_CORE_API Object
	{
	public:

		/** \brief Object default constructor.
		 */

		Object();

		/** \brief Object constructor with type argument.
		 */

		Object(const std::string& type);

		Object(const Object& object);

		/** \brief Object destructor (should be virtual?)
		 */

		virtual ~Object();

		/** \brief Accessor for the object type.
		 */

		const std::string& getType() const;

		void* getUserData() const;

		void setUserData(void* data);

		virtual int32 reference();

		virtual void release();


		Object& operator=(const Object& object);

	private:

		std::string _type;  ///< The object's type string.
		void* _userData;   ///< The object's user data.
		void* _referenceCount;
	};


	inline Object& Object::operator=(const Object& object)
	{
		_userData = object._userData;
		_referenceCount = object._referenceCount;
		_type = object._type;

		return *this;
	}
}

#endif // !OBJECT_H_

