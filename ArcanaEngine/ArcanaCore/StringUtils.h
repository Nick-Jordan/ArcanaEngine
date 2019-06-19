#ifndef STRING_UTILS_H_
#define STRING_UTILS_H_

#include "CoreDefines.h"

#include "Types.h"

#include <string>
#include <vector>

namespace Arcana
{
	class ARCANA_CORE_API StringUtils
	{
	public:
		
		static std::vector<std::string> split(const std::string& str, const std::string& delim);

		static std::string substring(const std::string& str, int32 beginIndex, int32 endIndex = -1);

		static void replace(std::string& str, const std::string& src, const std::string& rep);

		static std::string replaceFixed(const std::string& str, const std::string& src, const std::string& rep);

		static void replaceAll(std::string& str, const std::string& src, const std::string& rep);

		static std::string replaceAllFixed(const std::string& str, const std::string& src, const std::string& rep);

		static bool startsWith(const std::string& str, const std::string& prefix);

		static bool endsWith(const std::string& str, const std::string& suffix);

		static std::string toUpper(const std::string& str);

		static std::string toLower(const std::string& str);

		static std::string trim(const std::string& str);
	};
}

#endif // !STRING_UTILS_H_

