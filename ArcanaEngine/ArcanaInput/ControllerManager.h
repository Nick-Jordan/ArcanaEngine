#ifndef CONTROLLER_MANAGER_H_
#define CONTROLLER_MANAGER_H_

#include "InputDefines.h"

#include "Types.h"
#include "Controller.h"
#include "ControllerContext.h"

namespace Arcana
{

	class ARCANA_INPUT_API ControllerManager
	{
		friend class ControllerContext;

	public:		

		static ControllerManager& instance();

		
		const ControllerState& getState(uint32 controllerId) const;

		const Controller::Specification& getSpecification(uint32 controllerId) const;

		void update();

	private:

		ControllerManager();

		~ControllerManager();

		struct GameController
		{
			ControllerContext controller;

			ControllerState state;

			Controller::Specification specs;
		};

		//test 8 controllers;

		GameController _controllers[Controller::MaxControllers];
	};

}

#endif // !CONTROLLER_MANAGER_H_

