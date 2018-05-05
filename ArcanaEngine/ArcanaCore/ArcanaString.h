#ifndef STRING_H_
#define STRING_H_

#ifdef ARCANA_CORE_EXPORTS
#define ARCANA_CORE_API __declspec(dllexport)
#else
#define ARCANA_CORE_API __declspec(dllimport)
#endif

namespace Arcana
{

	class ARCANA_CORE_API String
	{
	public:

		String();


		~String();

	private:

	};

}

#endif

