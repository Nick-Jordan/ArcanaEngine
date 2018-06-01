#ifndef ARCANA_INPUT_H_
#define ARCANA_INPUT_H_

#include "InputDefines.h"

#include "ModuleInterface.h"

namespace Arcana
{
	class ARCANA_INPUT_API ArcanaInput : public ModuleInterface
	{
	public:

		ArcanaInput();

		virtual ~ArcanaInput();

		virtual bool startUp() override;

		virtual bool shutDown() override;

		virtual bool isGameModule() override;
	};
}

#endif // !ARCANA_INPUT_H_
