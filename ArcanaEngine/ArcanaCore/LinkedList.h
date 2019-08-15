/*#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

namespace Arcana
{
	template<typename T>
	class LinkedList
	{
	private:

		class Node
		{
		public:

			Node(const T& value) : value(value), next(nullptr) {}
			Node(const Node<T>& copy) : value(copy.value), next(copy.next) {}

			Node<T>* next;
			T value;
		};

	public:

		LinkedList();

		LinkedList(const LinkedList<T>& copy);

		LinkedList& operator=(const LinkedList<T>& copy);

	private:


	};
}

#endif // !LINKED_LIST_H_*/