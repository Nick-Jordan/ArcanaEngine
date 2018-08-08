#include "CloseOperation.h"

#include "Window.h"
#include "ArcanaLog.h"

namespace Arcana
{
	CloseOperation::CloseOperation() : _type(Default)
	{

	}

	CloseOperation::CloseOperation(Type type) : _type(type)
	{
	}

	CloseOperation::CloseOperation(const CloseOperation& closeOperation) : _type(closeOperation._type)
	{

	}

	CloseOperation::~CloseOperation()
	{
	}

	bool CloseOperation::performOperation(Window& window)
	{
		switch (_type)
		{
		case Exit:
			LOG(Error, WindowLog, "Destroying window with performOperation...");
			window.close();
			return true;
		case Hide:
			LOG(Error, WindowLog, "Hiding window with performOperation...");
			return false;
		case DoNothing:
			LOG(Error, WindowLog, "Doing nothing with performOperation...");
			return true;
		}
	}

	CloseOperation& CloseOperation::operator=(const CloseOperation& other)
	{
		_type = other._type;

		return *this;
	}

}
