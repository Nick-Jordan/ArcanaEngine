#ifndef BASE_CALLBACK_H_
#define BASE_CALLBACK_H_

#include "ArcanaCoreAPIDefinition.h"

#include "Defines.h"
#include "ArcanaTemplate.h"

namespace Arcana
{
	template<typename ReturnValue, typename... ArgumentTypes>
	class ARCANA_CORE_API BaseCallback
	{
	public:	
	
		BaseCallback() : _object(nullptr), _function(nullptr)
		{
		}	
		
		~BaseCallback()
		{
		}
		
		ReturnValue execute(ArgumentTypes&&... args)
		{
			AE_ASSERT(isBound());
			
			/*if(_object != nullptr)
			{
				return _object->_function(args);
			}*/
			
			return ((ReturnValue(*)(ArgumentTypes...))_function)(Forward<ArgumentTypes>(args)...);
		}
		
		ReturnValue executeIfBound(ArgumentTypes&&... args)
		{
			if(isBound())
			{
				/*if(_object != nullptr)
				{
					return _object->_function(args);
				}*/
				
				return ((ReturnValue(*)(ArgumentTypes...))_function)(Forward<ArgumentTypes>(args)...);
			}
			
			return ReturnValue();
		}
		
		bool isBound() const
		{
			return _function != nullptr;
		}
		
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
		
		void bind(ReturnValue (*CallbackFunction)(ArgumentTypes...))
		{
			unbind();
			
			_function = CallbackFunction;
		}
		
		void unbind()
		{
			_object = nullptr;
			_function = nullptr;
		}
		
	private:
	
		void* _object;
		void* _function;
	};
}

#endif