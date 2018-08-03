#ifndef BASE_CALLBACK_H_
#define BASE_CALLBACK_H_

#include "CoreDefines.h"

#include "ArcanaTemplate.h"

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

		/** \brief Function pointer typedef.
		 */

		typedef ReturnValue(*CallbackFunction)(ArgumentTypes...);

		/** \brief BaseCallback default constructor.
		 *  The function pointer is initially null.
		 */

		BaseCallback() : _object(nullptr), _function(nullptr)
		{
		}	
		
		/** \brief BaseCallback destructor.
		 */

		~BaseCallback()
		{
		}
		
		/** \brief Executes the function if one is bound.
		 *  An unknown number of arguments can be passed.
		 *  The argument types must coincide with the initial arguments specified in the template.
		 */

		ReturnValue execute(ArgumentTypes&&... args)
		{
			AE_ASSERT(isBound());
			
			/*if(_object != nullptr)
			{
				return _object->_function(args);
			}*/
			
			return ((CallbackFunction)_function)(Forward<ArgumentTypes>(args)...);
		}
		
		/** \brief Executes the function if one is bound.
		 *  An unknown number of arguments can be passed.
		 *  The argument types must coincide with the initial arguments specified in the template.
		 *  This method is the same as execute(), but without the assertion.
		 */

		ReturnValue executeIfBound(ArgumentTypes&&... args)
		{
			if(isBound())
			{
				/*if(_object != nullptr)
				{
					return _object->_function(args);
				}*/
				
				return ((CallbackFunction)_function)(Forward<ArgumentTypes>(args)...);
			}
			
			return ReturnValue();
		}

		/** \brief Returns true if a function pointer is bound.
		 */
		
		bool isBound() const
		{
			return _function != nullptr;
		}

		/** \brief Returns true if a function is a static function.
		 */
		
		bool isStaticFunction() const
		{
			return _object == nullptr;
		}
		
		/*template<class UserObject>
		void bind(UserObject* object, ReturnValue (*CallbackFunction)(ArgumentTypes...))
		{
			unbind();
		
			_object = object;
			
			_function = CallbackFunction;
		}*/
		
		/** \brief Binds a function pointer to the callback.
		 */

		void bind(CallbackFunction function)
		{
			unbind();
			
			_function = function;
		}

		/** \brief Sets the object and function pointer members to null.
		 */
		
		void unbind()
		{
			_object = nullptr;
			_function = nullptr;
		}
		
	private:
	
		void* _object;    ///< OBJECT
		void* _function;  ///< A pointer to a function.
	};
}

#endif