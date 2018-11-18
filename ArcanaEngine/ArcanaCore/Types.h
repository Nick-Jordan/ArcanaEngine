#ifndef TYPES_H_
#define TYPES_H_

#include "CoreDefines.h"

#include <string>
#include <vector>

namespace Arcana
{

	typedef unsigned char 		uint8;	   ///< 8-bit  unsigned.
	typedef unsigned short int	uint16;	   ///< 16-bit unsigned.
	typedef unsigned int		uint32;	   ///< 32-bit unsigned.
	typedef unsigned long long	uint64;	   ///< 64-bit unsigned.

	typedef	signed char			int8;	   ///< 8-bit  signed.
	typedef signed short int	int16;	   ///< 16-bit signed.
	typedef signed int	 		int32;	   ///< 32-bit signed.
	typedef signed long long	int64;     ///< 64-bit signed.

	typedef char				ANSICHAR;  ///< An ANSI character.
	typedef wchar_t				WIDECHAR;  ///< A wide character.
	typedef uint8				CHAR8;	   ///< An 8-bit character type.
	typedef uint16				CHAR16;	   ///< A 16-bit character type.
	typedef uint32				CHAR32;	   ///< A 32-bit character type.
	typedef WIDECHAR			AE_CHAR;   ///< A switchable character.

	enum TypeName : int32
	{
		UnknownType = -1,

		Uint8,
		Uint16,
		Uint32,
		Uint64,

		Int8,
		Int16,
		Int32,
		Int64,

		Float,
		Double,
		Boolean,
		String,

		Vec2f,
		Vec3f,
		Vec4f,

		Vec2i,
		Vec3i,
		Vec4i,

		Vec2d,
		Vec3d,
		Vec4d,

		NumTypes
	};

	class ARCANA_CORE_API Type
	{
	public:

		int32 id;
		int64 size;
		std::string name;
		std::string displayName;

		Type() {}
		Type(int32 type, int64 size, std::string name, std::string displayName)
			: id(type), size(size), name(name), displayName(displayName) {}

		bool operator==(const Type& type) const
		{
			return id == type.id;
		}
	};

	class ARCANA_CORE_API Types
	{
	public:
		
		static const Type ErrorType;

		static const Type Uint8;
		static const Type Uint16;
		static const Type Uint32;
		static const Type Uint64;

		static const Type Int8;
		static const Type Int16;
		static const Type Int32;
		static const Type Int64;

		static const Type Float;
		static const Type Double;
		static const Type Boolean;
		static const Type String;

		static const Type Vec2f;
		static const Type Vec3f;
		static const Type Vec4f;

		static const Type Vec2i;
		static const Type Vec3i;
		static const Type Vec4i;

		static const Type Vec2d;
		static const Type Vec3d;
		static const Type Vec4d;

		static Type parseTypeFromString(const std::string& type);

		static void registerType(int64 size, std::string name, std::string displayName);

	private:

		static const Type TypeArray[TypeName::NumTypes];

		static std::vector<Type> TypeVector;

		static int32 numRegisteredTypes;
	};
}
#endif // !TYPES_H_



