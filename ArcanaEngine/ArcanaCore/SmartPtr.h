#ifndef SMART_PTR_H_
#define SMART_PTR_H_

#include "Object.h"

#include "ArcanaLog.h"

namespace Arcana
{
	/** \brief A templated smart pointer implementation.
	 *  
	 *  Smart pointers wrap raw pointers, deleting the raw pointers upon destruction.
	 *  This eliminates the need to call 'delete' on raw pointers and can prevent memory leaks.
	 */

	template<typename ObjectType = Object>
	class SmartPtr
	{
	public:

		/** \brief SmartPtr default constructor.
		 */

		SmartPtr();

		/** \brief SmartPtr constructor with raw pointer argument.
		 */

		SmartPtr(ObjectType* pointer);

		/** \brief SmartPtr copy constructor.
		 */

		SmartPtr(const SmartPtr<ObjectType>& ptr);

		/** \brief SmartPtr destructor.
		 */

		~SmartPtr();

		/** \brief Increments the reference counter.
		 */

		void AddReference();

		/** \brief Decrements the reference counter.
		 */

		unsigned int Release();

		/** \brief Returns the current reference count.
		 */

		unsigned int getReferenceCount();

		/** \brief Returns the raw pointer.
		 */

		ObjectType* get();

		/** \brief Returns true if the raw pointer is null.
		 */

		bool isNull() const;


		/** \brief SmartPtr dereference operator.
		 */

		ObjectType& operator*();

		/** \brief SmartPtr pointer member access operator.
		 */

		ObjectType* operator->();

		/** \brief SmartPtr assignement operator.
		 */

		SmartPtr& operator=(const SmartPtr& ptr);

		/** \brief SmartPtr relational equivalence operator.
		 */

		bool operator==(const SmartPtr& other);

		/** \brief SmartPtr boolean conversion operator.
		 */

		operator bool() const
		{
			return !isNull();
		};


	private:

		ObjectType* _pointer;          ///< The raw pointer being contained.
		unsigned int _referenceCount;  ///< The current reference count.
	};

}

#include "SmartPtr.inl"

#endif // !SMART_PTR_H_

