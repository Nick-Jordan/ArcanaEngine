#ifndef STRING_UTILS_H_
#define STRING_UTILS_H_

#include "CoreDefines.h"

#include "Types.h"

#include <string>
#include <sstream>
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

		//Type conversions
		static std::string convertUint8ToString(uint8 u);
		static std::string convertUint16ToString(uint16 u);
		static std::string convertUint32ToString(uint32 u);
		static std::string convertUint64ToString(uint64 u);

		static std::string convertInt8ToString(int8 i);
		static std::string convertInt16ToString(int16 i);
		static std::string convertInt32ToString(int32 i);
		static std::string convertInt64ToString(int64 i);

		static std::string convertFloatToString(float f);
		static std::string convertDoubleToString(double d);
		static std::string convertBoolToString(bool b);

		template<typename PointerType>
		static std::string convertPointerToString(const PointerType* ptr);


		static uint8 convertStringToUint8(std::string str);
		static uint16 convertStringToUint16(std::string str);
		static uint32 convertStringToUint32(std::string str);
		static uint64 convertStringToUint64(std::string str);

		static int8 convertStringToInt8(std::string str);
		static int16 convertStringToInt16(std::string str);
		static int32 convertStringToInt32(std::string str);
		static int64 convertStringToInt64(std::string str);

		static float convertStringToFloat(std::string str);
		static double convertStringToDouble(std::string str);
		static bool convertStringToBool(std::string str);


		static std::string abbreviate(const std::string& str, uint32 maxWidth);
		static std::string abbreviate(const std::string& str, uint32 offset, uint32 maxWidth);
		static std::string abbreviate(const std::string& str, const std::string& marker, uint32 maxWidth);
		static std::string abbreviate(const std::string& str, const std::string& marker, uint32 offset, uint32 maxWidth);
		static std::string abbreviateMiddle(const std::string& str, const std::string& middle, uint32 length);
	};

	template<typename PointerType>
	inline std::string StringUtils::convertPointerToString(const PointerType* ptr)
	{
		const void * address = static_cast<const void*>(ptr);
		std::stringstream ss;
		ss << address;
		return ss.str();
	}
}

#endif // !STRING_UTILS_H_

