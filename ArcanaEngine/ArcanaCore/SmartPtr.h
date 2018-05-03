#ifndef SMART_PTR_H_
#define SMART_PTR_H_

#ifdef ARCANA_CORE_EXPORTS
#define ARCANA_CORE_API __declspec(dllexport)
#else
#define ARCANA_CORE_API __declspec(dllimport)
#endif

#include "Object.h"

namespace Arcana
{

	class ARCANA_CORE_API SmartPtr
	{
	public:

		SmartPtr();

		SmartPtr(Object* pointer);

		SmartPtr(const SmartPtr& ptr);

		~SmartPtr();

		void AddReference();

		unsigned int Release();

		unsigned int getReferenceCount();

		Object& operator*();

		Object* operator->();

		SmartPtr& operator=(const SmartPtr& ptr);

	private:

		Object* _pointer;
		unsigned int _referenceCount;
	};

}

#endif // !SMART_PTR_H_

