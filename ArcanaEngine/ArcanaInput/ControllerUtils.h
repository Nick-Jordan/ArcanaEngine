#ifndef CONTROLLER_UTILS_H_
#define CONTROLLER_UTILS_H_

#include "InputDefines.h"

#include "Types.h"

namespace Arcana
{
	class ARCANA_INPUT_API StateButtonMapping
	{
	public:
		StateButtonMapping(int32 button) : button(button - 113)
		{

		}

		operator int32() const { return button; }

	private:

		int32 button;
	};

	class ARCANA_INPUT_API StateAxisMapping
	{
	public:
		StateAxisMapping(int32 axis) : axis(axis - 107)
		{

		}

		operator int32() const { return axis; }

	private:

		int32 axis;
	};

	class ARCANA_INPUT_API ReverseStateButtonMapping
	{
	public:
		ReverseStateButtonMapping(int32 button) : button(button + 113)
		{

		}

		operator int32() const { return button; }

	private:

		int32 button;
	};

	class ARCANA_INPUT_API ReverseStateAxisMapping
	{
	public:
		ReverseStateAxisMapping(int32 axis) : axis(axis + 107)
		{

		}

		operator int32() const { return axis; }

	private:

		int32 axis;
	};
}

#endif // !CONTROLLER_UTILS_H_
