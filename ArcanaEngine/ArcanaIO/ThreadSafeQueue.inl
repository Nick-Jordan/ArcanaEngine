
namespace Arcana
{
	template<typename T>
	ThreadSafeQueue<T>::ThreadSafeQueue()
	{

	}

	template<typename T>
	ThreadSafeQueue<T>::ThreadSafeQueue(ThreadSafeQueue<T>& other) 
	{
		
	}

	template<typename T>
	ThreadSafeQueue<T>::~ThreadSafeQueue()
	{

	}


	template<typename T>
	bool ThreadSafeQueue<T>::empty() 
	{
		Lock lock(_mutex);
		return _queue.empty();
	}

	template<typename T>
	int32 ThreadSafeQueue<T>::size() 
	{
		Lock lock(_mutex);
		return _queue.size();
	}

	template<typename T>
	void ThreadSafeQueue<T>::enqueue(T& t)
	{
		Lock lock(_mutex);
		_queue.push(t);
	}

	template<typename T>
	bool ThreadSafeQueue<T>::dequeue(T& t)
	{
		Lock lock(_mutex);

		if (_queue.empty())
		{
			return false;
		}
		t = std::move(_queue.front());

		_queue.pop();
		return true;
	}
}
