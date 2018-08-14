#ifndef THREAD_H_
#define THREAD_H_

#include "IODefines.h"

#include "ArcanaTemplate.h"
#include "ArcanaLog.h"


namespace Arcana
{
	class ARCANA_IO_API ThreadContext;
	struct ARCANA_IO_API ThreadFunction;

	REGISTER_CATEGORY_ENGINE(ARCANA_IO_API, ThreadLog, none)

	class ARCANA_IO_API Thread
	{
		friend class ThreadContext;

	public:

		template <typename F>
		Thread(F function);

		template <typename F, typename... ArgumentTypes>
		Thread(F function, ArgumentTypes... args);

		template <typename C>
		Thread(void(C::*function)(), C* object);

		~Thread();


		void start();

		void wait();

		void stop();

	private:

		void run();

		ThreadContext* _context;

		ThreadFunction* _function;
	};

}

#include "Thread.inl"

#endif // !THREAD_H_
