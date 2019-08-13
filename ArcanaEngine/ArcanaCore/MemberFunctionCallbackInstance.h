#ifndef MEMBER_FUNCTION_CALLBACK_INSTANCE_H_
#define MEMBER_FUNCTION_CALLBACK_INSTANCE_H_

#include "CallbackInstance.h"
#include "CoreDefines.h"

namespace Arcana
{
	/** \brief Callback instance wrapping a member function pointer.
	 *
	 *  Needs to be bound to a function pointer and an object.
	 */

	template<typename ObjectType, typename ReturnValue, typename... ArgumentTypes>
	class MemberFunctionCallbackInstance : public CallbackInstance<void, ReturnValue, ArgumentTypes...>
	{
	public:

		/** \brief Function pointer typedef.
		 */

		typedef ReturnValue(ObjectType::*CallbackFunction)(ArgumentTypes...);

		MemberFunctionCallbackInstance() : _function(nullptr)
		{

		}

		virtual ~MemberFunctionCallbackInstance()
		{
		}

		virtual ReturnValue execute(ArgumentTypes&&... args) override
		{
			AE_ASSERT(isBound());

			return (_object->*_function)(Forward<ArgumentTypes>(args)...);
		}

		virtual ReturnValue executeIfBound(ArgumentTypes&&... args) override
		{
			if (isBound())
			{
				return (_object->*_function)(Forward<ArgumentTypes>(args)...);
			}

			return ReturnValue();
		}

		virtual bool isBound() override
		{
			return _object != nullptr && _function != nullptr;
		}

		virtual bool isStaticFunction() const override
		{
			return false;
		}

		virtual void unbind() override
		{
			_object = nullptr;
			_function = nullptr;
		}

		virtual void bind(void* object, void (Object::*function)()) override
		{
			_object = (ObjectType*)object;
			_function = reinterpret_cast<CallbackFunction>(function);
		}

	public:

		ObjectType* _object;
		CallbackFunction _function;
	};
}

#endif // !MEMBER_FUNCTION_CALLBACK_INSTANCE_H_