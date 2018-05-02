#ifndef LOG_CATEGORY_H_
#define LOG_CATEGORY_H_

#ifdef ARCANA_LOG_EXPORTS
#define ARCANA_LOG_API __declspec(dllexport)
#else
#define ARCANA_LOG_API __declspec(dllimport)
#endif

#include <string>

namespace Arcana
{
	class ARCANA_LOG_API LogCategory
	{
	public:
		
		LogCategory();
		
		LogCategory(std::string name, std::string hexColor);
		
		~LogCategory();
		
		const std::string& getName();
		
		const std::string& getHexColor();
		
	private:
	
		std::string _name;
		std::string _hexColor;
	};
}

#endif // !LOG_CATEGORY_H_