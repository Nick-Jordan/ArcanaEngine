#include "ControllerManager.h"

#include "ArcanaLog.h"

namespace Arcana
{
	ControllerManager& ControllerManager::instance()
	{
		static ControllerManager instance;
		return instance;
	}


	const ControllerState& ControllerManager::getState(uint32 controllerId) const
	{
		return _controllers[controllerId].state;
	}

	const Controller::Specification& ControllerManager::getSpecification(uint32 controllerId) const
	{
		return _controllers[controllerId].specs;
	}

	void ControllerManager::update()
	{
		for (int i = 0; i < Controller::MaxControllers; ++i)
		{
			GameController& gc = _controllers[i];

			if (gc.state.connected)
			{
				gc.state = gc.controller.update();

				if (!gc.state.connected)
				{
					gc.controller.close();
					gc.state = ControllerState();
					gc.specs = Controller::Specification();
				}
			}
			else
			{
				if (ControllerContext::isConnected(i))
				{
					if (gc.controller.open(i))
					{
						gc.state = gc.controller.update();
						gc.specs = gc.controller.getSpecification();
					}
				}
			}
		}
	}


	ControllerManager::ControllerManager()
	{
		LOG(Info, CoreEngine, "Controller Manager initializing context...");
		//ControllerContext::initialize();
	}


	ControllerManager::~ControllerManager()
	{
		for (int i = 0; i < Controller::MaxControllers; ++i)
		{
			if (_controllers[i].state.connected)
				_controllers[i].controller.close();
		}

		ControllerContext::cleanup();
	}
}