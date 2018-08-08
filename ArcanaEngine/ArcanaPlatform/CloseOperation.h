#ifndef CLOSE_OPERATION_H_
#define CLOSE_OPERATION_H_

#include "PlatformDefines.h"

namespace Arcana
{
	class Window;

	class ARCANA_PLATFORM_API CloseOperation
	{
	public:

		enum Type
		{
			Exit,
			Hide,
			DoNothing,
			Default = Exit
		};

		CloseOperation();

		CloseOperation(Type type);

		CloseOperation(const CloseOperation& closeOperation);

		~CloseOperation();

		bool performOperation(Window& window);

		CloseOperation& operator=(const CloseOperation& other);

	private:

		Type _type;
	};

}

#endif // !CLOSE_OPERATION_H_