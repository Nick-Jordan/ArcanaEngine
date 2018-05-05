#ifndef OBJECT_H_
#define OBJECT_H_

#ifdef ARCANA_CORE_EXPORTS
#define ARCANA_CORE_API __declspec(dllexport)
#else
#define ARCANA_CORE_API __declspec(dllimport)
#endif

#include <string>

namespace Arcana
{

	class ARCANA_CORE_API Object
	{
	public:

		Object();

		Object(const std::string& type);

		~Object();

		const std::string& getType() const;

		void yee();

	private:

		std::string _type;
	};

}

#endif // !OBJECT_H_

