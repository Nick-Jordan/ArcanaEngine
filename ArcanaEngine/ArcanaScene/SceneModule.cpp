// SceneModule.cpp : Defines the exported functions for the DLL application.
//

#include "SceneModule.h"

#include "AxisMapping.h"
#include "ActionMapping.h"

namespace Arcana
{
	SceneModule::SceneModule()
	{

	}

	SceneModule::~SceneModule()
	{

	}

	bool SceneModule::startUp()
	{
		std::vector<KeyMapping> forwardsKeyMappings;
		forwardsKeyMappings.push_back(KeyMapping(Keys::W, 1.0));
		forwardsKeyMappings.push_back(KeyMapping(Keys::S, -1.0));
		forwardsKeyMappings.push_back(KeyMapping(Keys::Up, 1.0));
		forwardsKeyMappings.push_back(KeyMapping(Keys::Down, -1.0));
		forwardsKeyMappings.push_back(KeyMapping(Keys::ControllerLeftAnalogY, 1.0));
		AxisMapping::registerAxisMapping("MoveForwards", forwardsKeyMappings);

		std::vector<KeyMapping> rightKeyMappings;
		rightKeyMappings.push_back(KeyMapping(Keys::A, -1.0));
		rightKeyMappings.push_back(KeyMapping(Keys::D, 1.0));
		rightKeyMappings.push_back(KeyMapping(Keys::Right, 1.0));
		rightKeyMappings.push_back(KeyMapping(Keys::Left, -1.0));
		rightKeyMappings.push_back(KeyMapping(Keys::ControllerLeftAnalogX, 1.0));
		AxisMapping::registerAxisMapping("MoveRight", rightKeyMappings);

		std::vector<KeyMapping> upKeyMappings;
		upKeyMappings.push_back(KeyMapping(Keys::Space, 1.0));
		upKeyMappings.push_back(KeyMapping(Keys::LeftControl, -1.0));
		upKeyMappings.push_back(KeyMapping(Keys::ControllerLeftTriggerAxis, -1.0));
		upKeyMappings.push_back(KeyMapping(Keys::ControllerRightTriggerAxis, 1.0));
		AxisMapping::registerAxisMapping("MoveUp", upKeyMappings);

		std::vector<KeyMapping> rollKeyMappings;
		rollKeyMappings.push_back(KeyMapping(Keys::E, -1.0));
		rollKeyMappings.push_back(KeyMapping(Keys::Q, 1.0));
		rollKeyMappings.push_back(KeyMapping(Keys::ControllerLeftShoulder, -1.0));
		rollKeyMappings.push_back(KeyMapping(Keys::ControllerRightShoulder, 1.0));
		AxisMapping::registerAxisMapping("Roll", rollKeyMappings);

		std::vector<KeyMapping> mousePitchKeyMappings;
		mousePitchKeyMappings.push_back(KeyMapping(Keys::MouseYRelative, 1.0));
		AxisMapping::registerAxisMapping("LookVertical", mousePitchKeyMappings);

		std::vector<KeyMapping> mouseYawKeyMappings;
		mouseYawKeyMappings.push_back(KeyMapping(Keys::MouseXRelative, 1.0));
		AxisMapping::registerAxisMapping("LookHorizontal", mouseYawKeyMappings);

		ActionMapping::registerActionMapping("Jump", Keys::Space);

		return true;
	}

	bool SceneModule::shutDown()
	{
		return true;
	}

	bool SceneModule::isGameModule()
	{
		return false;
	}
}


