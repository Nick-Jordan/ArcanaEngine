#ifndef SMART_PTR_H_
#define SMART_PTR_H_

#include "Object.h"

namespace Arcana
{
	template<typename ObjectType = Object>
	class SmartPtr
	{
	public:

		SmartPtr();

		SmartPtr(ObjectType* pointer);

		SmartPtr(const SmartPtr& ptr);

		~SmartPtr();

		void AddReference();

		unsigned int Release();

		unsigned int getReferenceCount();

		ObjectType* get();

		bool isNull() const;


		ObjectType& operator*();

		ObjectType* operator->();

		SmartPtr& operator=(const SmartPtr& ptr);

		bool operator==(const SmartPtr& other);

		operator bool() const
		{
			return !isNull();
		};


	private:

		ObjectType* _pointer;
		unsigned int _referenceCount;
	};

}

#include "SmartPtr.inl"

#endif // !SMART_PTR_H_

