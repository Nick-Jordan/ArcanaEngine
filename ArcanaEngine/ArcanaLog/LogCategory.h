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
	/** \brief Defines the category of a message. Labels logged messages in the output file.
	 */
	class ARCANA_LOG_API LogCategory
	{
	public:
		
		/** \brief Default constructor.
		 *  Category name is Default. Hex color is none.
		 */
		LogCategory();
		
		/** \brief Basic constructor initializing both name and hex color.
		 */
		LogCategory(std::string name, std::string hexColor);
		
		/** \brief Default destructor.
		 */
		~LogCategory();
		
		/** \brief Accessor for the category name.
		 */
		const std::string& getName();
		
		/** \brief Accessor for the category color.
		 */
		const std::string& getHexColor();
		
	private:
	
		/** \brief The name of the category.
		 */
		std::string _name;

		/** \brief The color of the category.
		 */
		std::string _hexColor;
	};
}

#endif // !LOG_CATEGORY_H_