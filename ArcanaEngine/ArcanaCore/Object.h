#ifndef OBJECT_H_
#define OBJECT_H_

#ifdef ARCANA_CORE_EXPORTS
#define ARCANA_CORE_API __declspec(dllexport)
#else
#define ARCANA_CORE_API __declspec(dllimport)
#endif

namespace Arcana
{

	class ARCANA_CORE_API Object
	{
	public:

		Object();
		~Object();

		void yee();
	};

}

#endif // !OBJECT_H_

