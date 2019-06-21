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

		/** \brief Object copy constructor.
		 */
		Object(const Object& object);

		/** \brief Object destructor (should be virtual?)
		 */
		virtual ~Object();

		/** \brief Accessor for the object type.
		 */
		const std::string& getType() const;

		/** \brief Accessor for the object's user data.
		 */
		void* getUserData() const;

		/** \brief Mutator for the object's user data.
		 */
		void setUserData(void* data);

		/** \brief Increments the reference count.
		 *  
		 *  Returns the current reference count.
		 */
		virtual int32 reference();

		/** \brief Decrements the reference count.
		 *
		 *  Deletes the object if the reference count is zero.
		 */
		virtual void release();

		/** \brief Increments the reference count.
		 *
		 *  Returns the current reference count.
		 */
		virtual int32 referenceCount();

		/** \brief Marks the object to be destroyed by the garbage collector.
		 */
		void markForDestruction();

		/** \brief Returns true if the object has been marked for destruction.
		 */
		bool isPendingDestroy() const;

		/** \brief Allows the garbage collector to destroyed the object.
		 * 
		 *  Only works for objects that are marked for destruction.
		 */
		void allowDestruction();

		/** \brief Returns true if the object can be destroyed.
		 */
		bool canBeDestroyed() const;

		std::string toString() const;

		/** \brief Object assignment operator.
		 */
		Object& operator=(const Object& object);

	private:

		std::string _type;  ///< The object's type string.
		void* _userData;   ///< The object's user data.
		void* _referenceCount;  ///< The reference count.

		bool _markedForDestruction;  ///< Object marked for destruction.
		bool _canBeDestroyed;		 ///< Object can be destruction.
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

