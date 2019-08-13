#ifndef STATIC_FUNCTION_CALLBACK_INSTANCE_H_
#define STATIC_FUNCTION_CALLBACK_INSTANCE_H_

#include "CallbackInstance.h"
#include "CoreDefines.h"

namespace Arcana
{
	template<typename ReturnValue, typename... ArgumentTypes>
	class StaticFunctionCallbackInstance : public CallbackInstance<void, ReturnValue, ArgumentTypes...>
	{
	public:

		/** \brief Function pointer typedef.
		 */

		typedef ReturnValue(*CallbackFunction)(ArgumentTypes...);


		StaticFunctionCallbackInstance() : _function(nullptr)
		{

		}

		virtual ~StaticFunctionCallbackInstance()
		{
		}

		virtual ReturnValue execute(ArgumentTypes&&... args) override
		{
			AE_ASSERT(isBound());

			return _function(Forward<ArgumentTypes>(args)...);
		}

		virtual ReturnValue executeIfBound(ArgumentTypes&&... args) override
		{
			if (isBound())
			{

				return _function(Forward<ArgumentTypes>(args)...);
			}

			return ReturnValue();
		}

		virtual bool isBound() override
		{
			return _function != nullptr;
		}

		virtual bool isStaticFunction() const override
		{
			return true;
		}

		virtual void unbind() override
		{
			_function = nullptr;
		}

		virtual void bind(CallbackFunction function) override
		{
			_function = function;
		}

	public:

		CallbackFunction _function;
	};
}

#endif // !STATIC_FUNCTION_CALLBACK_INSTANCE_H_