#ifndef MODULE_MANAGER_H_
#define MODULE_MANAGER_H_

#ifdef ARCANA_CORE_EXPORTS
#define ARCANA_CORE_API __declspec(dllexport)
#else
#define ARCANA_CORE_API __declspec(dllimport)
#endif

namespace Arcana
{
	class ARCANA_CORE_API ModuleManager
	{
	public:

		ModuleManager();

		~ModuleManager();

		virtual bool startUp() = 0;

		virtual bool shutDown() = 0;
	};
}

#endif
