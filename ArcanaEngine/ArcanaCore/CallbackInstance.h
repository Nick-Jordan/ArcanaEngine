#ifndef CALLBACK_INSTANCE_H_
#define CALLBACK_INSTANCE_H_

namespace Arcana
{
	template<typename ObjectType, typename ReturnValue, typename... ArgumentTypes>
	class CallbackInstance
	{
	public:

		virtual ReturnValue execute(ArgumentTypes&&... args) = 0;

		virtual ReturnValue executeIfBound(ArgumentTypes&&... args) = 0;

		virtual bool isBound() = 0;

		virtual bool isStaticFunction() const = 0;

		virtual void unbind() = 0;
		
		virtual void bind(void* object, void(Object::*)()) {};

		virtual void bind(ReturnValue(*CallbackFunction)(ArgumentTypes...)) {};
	};
}

#endif // !CALLBACK_INSTANCE_H_