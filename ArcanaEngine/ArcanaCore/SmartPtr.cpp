#include "SmartPtr.h"

namespace Arcana
{

	SmartPtr::SmartPtr() : _pointer(nullptr), _referenceCount(0)
	{
	}

	SmartPtr::SmartPtr(Object* pointer) : _pointer(pointer), _referenceCount(0)
	{
		AddReference();
	}

	SmartPtr::SmartPtr(const SmartPtr& ptr) : _pointer(ptr._pointer), _referenceCount(ptr._referenceCount)
	{
		AddReference();
	}

	SmartPtr::~SmartPtr()
	{
		if (Release() == 0)
		{
			delete _pointer;
		}
	}

	void SmartPtr::AddReference()
	{
		_referenceCount++;
	}

	unsigned int SmartPtr::Release()
	{
		return --_referenceCount;
	}

	unsigned int SmartPtr::getReferenceCount()
	{
		return _referenceCount;
	}

	Object& SmartPtr::operator* ()
	{
		return *_pointer;
	}

	Object* SmartPtr::operator->()
	{
		return _pointer;
	}

	SmartPtr& SmartPtr::operator=(const SmartPtr& ptr)
	{
		if (this != &ptr)
		{
			if (Release() == 0)
			{
				delete _pointer;
			}

			_pointer = ptr._pointer;
			_referenceCount = ptr._referenceCount;
			AddReference();
		}
		return *this;
	}
}
