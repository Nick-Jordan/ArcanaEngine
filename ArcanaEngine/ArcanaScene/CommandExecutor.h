#ifndef COMMAND_EXECUTOR_H_
#define COMMAND_EXECUTOR_H_

#include "World.h"

namespace Arcana
{

	class CommandExecutor
	{
	public:

		virtual ~CommandExecutor() {};

		virtual bool execute(World* world, const std::string& command) = 0;
	};

}
#endif // !COMMAND_RECIPIENT_H_

