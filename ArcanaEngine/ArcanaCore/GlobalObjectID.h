#ifndef GLOBAL_OBJECT_ID_H_
#define GLOBAL_OBJECT_ID_H_

#ifdef ARCANA_CORE_EXPORTS
#define ARCANA_CORE_API __declspec(dllexport)
#else
#define ARCANA_CORE_API __declspec(dllimport)
#endif

namespace Arcana
{

	class ARCANA_CORE_API GlobalObjectID
	{
	public:

		GlobalObjectID();
		~GlobalObjectID();
	};

}

#endif

