
namespace Arcana
{

	template<typename ObjectType>
	SmartPtr<ObjectType>::SmartPtr() : _pointer(nullptr), _referenceCount(0)
	{
	}

	template<typename ObjectType>
	SmartPtr<ObjectType>::SmartPtr(ObjectType* pointer) : _pointer(pointer), _referenceCount(0)
	{
		AddReference();
	}

	template<typename ObjectType>
	SmartPtr<ObjectType>::SmartPtr(const SmartPtr<ObjectType>& ptr) : _pointer(ptr._pointer), _referenceCount(ptr._referenceCount)
	{
		AddReference();
	}

	template<typename ObjectType>
	SmartPtr<ObjectType>::~SmartPtr()
	{
		if (Release() == 0)
		{
			//LOG(Info, CoreEngine, "Deleting SmartPtr pointer...");
			delete _pointer;
		}
	}

	template<typename ObjectType>
	void SmartPtr<ObjectType>::AddReference()
	{
		_referenceCount++;
	}

	template<typename ObjectType>
	unsigned int SmartPtr<ObjectType>::Release()
	{
		return --_referenceCount;
	}

	template<typename ObjectType>
	unsigned int SmartPtr<ObjectType>::getReferenceCount()
	{
		return _referenceCount;
	}

	template<typename ObjectType>
	ObjectType* SmartPtr<ObjectType>::get()
	{
		return _pointer;
	}

	template<typename ObjectType>
	bool SmartPtr<ObjectType>::isNull() const
	{
		return _pointer == nullptr;
	}

	template<typename ObjectType>
	ObjectType& SmartPtr<ObjectType>::operator* ()
	{
		return *_pointer;
	}

	template<typename ObjectType>
	ObjectType* SmartPtr<ObjectType>::operator->()
	{
		return _pointer;
	}

	template<typename ObjectType>
	SmartPtr<ObjectType>& SmartPtr<ObjectType>::operator=(const SmartPtr<ObjectType>& ptr)
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

	template<typename ObjectType>
	bool SmartPtr<ObjectType>::operator==(const SmartPtr<ObjectType>& other)
	{
		return _pointer == other._pointer;// && _referenceCount == other._referenceCount;
	}
}
