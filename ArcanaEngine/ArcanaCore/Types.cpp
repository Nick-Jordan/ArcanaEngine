#include "Types.h"

namespace Arcana
{
	const Type Types::ErrorType = Type(UnknownType, -1, "error_type", "Error Type");

	const Type Types::Uint8 = Type(TypeName::Uint8, 1, "uint8", "8-bit Unsigned Integer");
	const Type Types::Uint16 = Type(TypeName::Uint16, 2, "uint16", "16-bit Unsigned Integer");
	const Type Types::Uint32 = Type(TypeName::Uint32, 4, "uint32", "32-bit Unsigned Integer");
	const Type Types::Uint64 = Type(TypeName::Uint64, 8, "uint64", "64-bit Unsigned Integer");

	const Type Types::Int8 = Type(TypeName::Int8, 1, "int8", "8-bit Integer");
	const Type Types::Int16 = Type(TypeName::Int16, 2, "int16", "16-bit Integer");
	const Type Types::Int32 = Type(TypeName::Int32, 4, "int32", "32-bit Integer");
	const Type Types::Int64 = Type(TypeName::Int64, 8, "int64", "64-bit Integer");

	const Type Types::Float = Type(TypeName::Float, 4, "float", "32-bit Floating Point");
	const Type Types::Double = Type(TypeName::Double, 8, "double", "64-bit Floating Point");
	const Type Types::Boolean = Type(TypeName::Boolean, 1, "bool", "Boolean");
	const Type Types::String = Type(TypeName::String, 8, "string", "String");

	std::vector<Type> Types::TypeVector = std::vector<Type>();
	int32 Types::numRegisteredTypes = 0;

	Type Types::parseTypeFromString(const std::string& type)
	{
		if (type == "uint8" || type == "char")
		{
			return Uint8;
		}
		else if (type == "uint16")
		{
			return Uint16;
		}
		else if (type == "uint32")
		{
			return Uint32;
		}
		else if (type == "uint64")
		{
			return Uint64;
		}
		else if (type == "int8")
		{
			return Int8;
		}
		else if (type == "int16" || type == "short")
		{
			return Int16;
		}
		else if (type == "int32" || type == "integer")
		{
			return Int32;
		}
		else if (type == "int64" || type == "long")
		{
			return Int64;
		}
		else if (type == "float")
		{
			return Float;
		}
		else if (type == "double")
		{
			return Double;
		}
		else if (type == "boolean" || type == "bool")
		{
			return Boolean;
		}
		else if (type == "string")
		{
			return String;
		}

		for (auto i = TypeVector.begin(); i != TypeVector.end(); ++i)
		{
			if (type == (*i).name)
			{
				return (*i);
			}
		}

		return ErrorType;
	}

	void Types::registerType(int64 size, std::string name, std::string displayName)
	{
		numRegisteredTypes++;
		TypeVector.push_back(Type(NumTypes + numRegisteredTypes, size, name, displayName));
	}
}