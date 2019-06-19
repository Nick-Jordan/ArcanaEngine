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

	const Type Types::Vec2f = Type(TypeName::Vec2f, 8, "vec2f", "2d Float Vector");
	const Type Types::Vec3f = Type(TypeName::Vec3f, 12, "vec3f", "3d Float Vector");
	const Type Types::Vec4f = Type(TypeName::Vec4f, 16, "vec4f", "4d Float Vector");

	const Type Types::Vec2i = Type(TypeName::Vec2i, 8, "vec2i", "2d Integer Vector");
	const Type Types::Vec3i = Type(TypeName::Vec3i, 12, "vec3i", "3d Integer Vector");
	const Type Types::Vec4i = Type(TypeName::Vec4i, 16, "vec4i", "4d Integer Vector");

	const Type Types::Vec2d = Type(TypeName::Vec2d, 16, "vec2d", "2d Double Vector");
	const Type Types::Vec3d = Type(TypeName::Vec3d, 24, "vec3d", "3d Double Vector");
	const Type Types::Vec4d = Type(TypeName::Vec4d, 32, "vec4d", "4d Double Vector");

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
		else if (type == "vec2" || type == "vec2f")
		{
			return Vec2f;
		}
		else if (type == "vec3" || type == "vec3f")
		{
			return Vec3f;
		}
		else if (type == "vec4" || type == "vec4f")
		{
			return Vec4f;
		}
		else if (type == "vec2i")
		{
			return Vec2i;
		}
		else if (type == "vec3i")
		{
			return Vec3i;
		}
		else if (type == "vec4i")
		{
			return Vec4i;
		}
		else if (type == "vec2d")
		{
			return Vec2d;
		}
		else if (type == "vec2d")
		{
			return Vec3d;
		}
		else if (type == "vec2d")
		{
			return Vec4d;
		}

		for (auto i = TypeVector.begin(); i != TypeVector.end(); ++i)
		{
			if (type == (*i).Name)
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