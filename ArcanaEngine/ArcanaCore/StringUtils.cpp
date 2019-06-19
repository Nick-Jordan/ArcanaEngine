#include "StringUtils.h"

#include <algorithm>
#include <sstream>
#include <iterator>

namespace Arcana
{
	std::vector<std::string> StringUtils::split(const std::string& str, const std::string& delim)
	{
		std::vector<std::string> tokens;

		size_t prev = 0, pos = 0;
		do
		{
			pos = str.find(delim, prev);
			if (pos == std::string::npos) pos = str.length();
			std::string token = str.substr(prev, pos - prev);
			tokens.push_back(token);
			prev = pos + delim.length();
		} while (pos < str.length() && prev < str.length());

		return tokens;
	}

	std::string StringUtils::substring(const std::string& str, int32 beginIndex, int32 endIndex)
	{
		if (endIndex == -1)
			endIndex = str.length();

		if (str.size() == 0 || beginIndex > endIndex || beginIndex >= str.length())
			return "";

		return str.substr(beginIndex, endIndex - beginIndex);
	}

	void StringUtils::replace(std::string& str, const std::string& src, const std::string& rep)
	{
		size_t startPos = str.find(src);
		if (startPos == std::string::npos)
		{
			return;
		}
		str.replace(startPos, src.length(), rep);
	}

	std::string StringUtils::replaceFixed(const std::string& str, const std::string& src, const std::string& rep)
	{
		std::string s = str;
		replace(s, src, rep);
		return s;
	}

	void StringUtils::replaceAll(std::string& str, const std::string& src, const std::string& rep)
	{
		if (src.length() == 0)
		{
			return;
		}
		size_t startPos = 0;
		while ((startPos = str.find(src, startPos)) != std::string::npos) 
		{
			str.replace(startPos, src.length(), rep);
			startPos += rep.length();
		}
	}

	std::string StringUtils::replaceAllFixed(const std::string& str, const std::string& src, const std::string& rep)
	{
		std::string s = str;
		replaceAll(s, src, rep);
		return s;
	}

	bool StringUtils::startsWith(const std::string& str, const std::string& prefix)
	{
		return substring(str, 0, prefix.length()) == prefix;
	}

	bool StringUtils::endsWith(const std::string& str, const std::string& suffix)
	{
		return substring(str, str.length() - suffix.length(), -1) == suffix;
	}

	std::string StringUtils::toUpper(const std::string& str)
	{
		std::string s = str;
		std::transform(s.begin(), s.end(), s.begin(), ::toupper);
		return s;
	}

	std::string StringUtils::toLower(const std::string& str)
	{
		std::string s = str;
		std::transform(s.begin(), s.end(), s.begin(), ::tolower);
		return s;
	}

	template <class ForwardIterator, class OutputIterator, class UnaryPredicate>
	void trim(ForwardIterator first, ForwardIterator last, OutputIterator result, UnaryPredicate pred)
	{
		while (first != last && pred(*first))
		{
			first++;
		}

		for (ForwardIterator p = last; first != last; first++)
		{
			if (pred(*first))
			{
				p = first;
			}
			else
			{
				if (p != last) 
				{
					*result = *p;
					p = last;
				}

				*result = *first;
			}
		}
	}

	std::string StringUtils::trim(const std::string& str)
	{
		std::ostringstream result;
		Arcana::trim(str.begin(), str.end(), std::ostream_iterator<char>(result, ""), isspace);
		return result.str();
	}
}
