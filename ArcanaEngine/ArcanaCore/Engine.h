#ifndef ARCANA_ENGINE_H_
#define ARCANA_ENGINE_H_

#ifdef ARCANA_CORE_EXPORTS
#define ARCANA_CORE_API __declspec(dllexport)
#else
#define ARCANA_CORE_API __declspec(dllimport)
#endif

namespace Arcana
{
	class ARCANA_CORE_API Engine
	{
	public:

		Engine();
	};
}

#endif // !ARCANA_ENGINE_H_