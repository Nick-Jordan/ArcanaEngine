#ifndef CALLBACK_INSTANCE_H_
#define CALLBACK_INSTANCE_H_

#include "Object.h"

namespace Arcana
{
	/**\brief An instance of a callback
	 *
	 *  This class wraps either a static or member function pointer.
	 *  Derived classes implement one of the bind methods (one for member functions, one for static functions).
	 */
	template<typename ObjectType, typename ReturnValue, typename... ArgumentTypes>
	class CallbackInstance : public Object
	{
	public:
		
		/** \brief Executes the function.
		 *
		 *  An unknown number of arguments can be passed.
		 *  The argument types must coincide with the initial arguments specified in the template.
		 */
		virtual ReturnValue execute(ArgumentTypes&&... args) = 0;

		/** \brief Executes the function.
		 *
		 *  An unknown number of arguments can be passed.
		 *  The argument types must coincide with the initial arguments specified in the template.
		 *  This method is the same as execute(), but without the assertion.
		 */
		virtual ReturnValue executeIfBound(ArgumentTypes&&... args) = 0;

		/** \brief Returns true if a function pointer is bound.
		 */
		virtual bool isBound() = 0;

		/** \brief Returns true if a function is a static function.
		 */
		virtual bool isStaticFunction() const = 0;

		/** \brief Sets the object and function pointer members to null.
		 */
		virtual void unbind() = 0;
		
		/** \brief Binds this callback to the member function of an object.
		 *
		 *  The member function must take the arguments specified in the CallbackInstance templates.
		 */
		virtual void bind(void* object, void(Object::*)()) {};

		/** \brief Binds a static function pointer to the callback.
		 *
		 *  The static function must take the arguments specified in the CallbackInstance templates.
		 */
		virtual void bind(ReturnValue(*CallbackFunction)(ArgumentTypes...)) {};
	};
}

#endif // !CALLBACK_INSTANCE_H_