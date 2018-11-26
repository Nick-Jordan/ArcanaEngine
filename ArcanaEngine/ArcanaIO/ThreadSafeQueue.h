#ifndef THREAD_SAFE_QUEUE_H_
#define THREAD_SAFE_QUEUE_H_

#include <queue>

#include "Types.h"
#include "Mutex.h"
#include "Lock.h"

namespace Arcana
{
	template <typename T>
	class ThreadSafeQueue 
	{
	public:

		ThreadSafeQueue();

		ThreadSafeQueue(ThreadSafeQueue<T>& other);

		~ThreadSafeQueue();


		bool empty();

		int32 size();

		void enqueue(T& t);

		bool dequeue(T& t);

	private:

		std::queue<T> _queue;
		Mutex _mutex;
	};
}

#include "ThreadSafeQueue.inl"

#endif // !THREAD_SAFE_QUEUE_H_