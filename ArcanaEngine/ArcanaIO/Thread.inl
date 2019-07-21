
#include <functional>

namespace Arcana
{
	struct ThreadFunction
	{
		virtual ~ThreadFunction() {}
		virtual void run() = 0;
	};

	template <typename T>
	struct ThreadFunctor : ThreadFunction
	{
		ThreadFunctor(T functor) : _functor(functor) {}

		virtual void run() 
		{ 
			_functor(); 
		}

		T _functor;
	};

	template <typename T, typename... ArgumentTypes>
	struct ThreadFunctorArgs : ThreadFunction
	{
		using funcType = decltype(std::bind(std::declval<std::function<void(ArgumentTypes...)>>(), std::declval<ArgumentTypes>()...));

		ThreadFunctorArgs(T function, ArgumentTypes... args) : _function(function, Forward<ArgumentTypes>(args)...) {}

		virtual void run() 
		{ 
			_function();
		}

		funcType _function;
	};

	template <typename C>
	struct ThreadMemberFunction : ThreadFunction
	{
		ThreadMemberFunction(void(C::*function)(), C* object) : _function(function), _object(object) {}

		virtual void run() 
		{ 
			(_object->*_function)(); 
		}

		void(C::*_function)();
		C* _object;
	};

	template <typename F>
	Thread::Thread(F function) :
		_context(nullptr),
		_function(new ThreadFunctor<F>(function))
	{

	}

	template <typename F, typename... ArgumentTypes>
	Thread::Thread(F function, ArgumentTypes... args) :
		_context(nullptr),
		_function(new ThreadFunctorArgs<F, ArgumentTypes...>(function, args...))
	{

	}

	template <typename C>
	Thread::Thread(void(C::*function)(), C* object) :
		_context(nullptr),
		_function(new ThreadMemberFunction<C>(function, object))
	{

	}
}