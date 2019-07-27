#ifndef BASE_CALLBACK_H_
#define BASE_CALLBACK_H_

#include "CoreDefines.h"

#include "ArcanaTemplate.h"
#include "StaticFunctionCallbackInstance.h"
#include "MemberFunctionCallbackInstance.h"

namespace Arcana
{
	/** \brief A templated callback class.
	 *  
	 *  Functions can be bound to a callback and executed with the execute function.
	 *  Callbacks can be registered with a return type and arguments.
	 */

	template<typename ReturnValue, typename... ArgumentTypes>
	class BaseCallback
	{
	public:

		/** \brief Default constructor.
		 *
		 *  The callback instance is initially null.
		 */
		BaseCallback() : _callbackInstance(nullptr)
		{
		}	
		
		/** \brief BaseCallback copy constructor.
		 *
		 *  Copies and references the callback instance.
		 */
		BaseCallback(const BaseCallback<ReturnValue, ArgumentTypes...>& copy) : _callbackInstance(copy._callbackInstance)
		{
			if (_callbackInstance)
			{
				_callbackInstance->reference();
			}
		}

		/** \brief BaseCallback destructor.
		 */
		~BaseCallback()
		{
			if (_callbackInstance)
			{
				AE_RELEASE(_callbackInstance);
			}
		}
		
		/** \brief Executes the function if one is bound.
		 *
		 *  An unknown number of arguments can be passed.
		 *  The argument types must coincide with the initial arguments specified in the template.
		 */
		ReturnValue execute(ArgumentTypes... args)
		{			
			if (_callbackInstance)
			{
				return _callbackInstance->execute(Forward<ArgumentTypes>(args)...);
			}

			return ReturnValue();
		}
		
		/** \brief Executes the function if one is bound.
		 *
		 *  An unknown number of arguments can be passed.
		 *  The argument types must coincide with the initial arguments specified in the template.
		 *  This method is the same as execute(), but without the assertion.
		 */
		ReturnValue executeIfBound(ArgumentTypes... args)
		{
			if (_callbackInstance)
			{
				return _callbackInstance->executeIfBound(Forward<ArgumentTypes>(args)...);
			}

			return ReturnValue();
		}

		/** \brief Returns true if a function pointer is bound.
		 */	
		bool isBound() const
		{
			if (_callbackInstance)
			{
				return _callbackInstance->isBound();
			}

			return false;
		}

		/** \brief Returns true if a function is a static function.
		 */	
		bool isStaticFunction() const
		{
			if (_callbackInstance)
			{
				return _callbackInstance->isStaticFunction();
			}

			return false;
		}
		
		/** \brief Binds this callback to the member function of an object.
		 *
		 *  The UserObject is templated; it can be of any type.
		 *  The member function must take the arguments specified in the BaseCallback templates.
		 */
		template<class UserObject>
		void bind(UserObject* object, ReturnValue (UserObject::*CallbackFunction)(ArgumentTypes...))
		{
			if (_callbackInstance)
			{
				AE_DELETE(_callbackInstance);
			}

			_callbackInstance = new MemberFunctionCallbackInstance<UserObject, ReturnValue, ArgumentTypes...>();
			_callbackInstance->reference();

			unbind();

			_callbackInstance->bind(object, reinterpret_cast<void(Object::*)()>(CallbackFunction));
		}
		
		/** \brief Binds a static function pointer to the callback.
		 *
		 *  The static function must take the arguments specified in the BaseCallback templates.
		 */
		void bind(ReturnValue(*CallbackFunction)(ArgumentTypes...))
		{
			if (_callbackInstance)
			{
				if (!_callbackInstance->isStaticFunction())
				{
					AE_DELETE(_callbackInstance);
				}
			}

			if (!_callbackInstance)
			{
				_callbackInstance = new StaticFunctionCallbackInstance<ReturnValue, ArgumentTypes...>();
				_callbackInstance->reference();
			}

			unbind();

			_callbackInstance->bind(CallbackFunction);
		}

		/** \brief Sets the object and function pointer members to null.
		 */	
		void unbind()
		{
			if (_callbackInstance)
			{
				_callbackInstance->unbind();
			}
		}

		BaseCallback<ReturnValue, ArgumentTypes...>& operator=(const BaseCallback<ReturnValue, ArgumentTypes...>& copy)
		{
			_callbackInstance = copy._callbackInstance;
			if (_callbackInstance)
			{
				_callbackInstance->reference();
			}

			return *this;
		}
		
	private:
	
		CallbackInstance<void, ReturnValue, ArgumentTypes...>* _callbackInstance; ///< The instance of the callback. Can be a static or member function.
	};
}

#endif