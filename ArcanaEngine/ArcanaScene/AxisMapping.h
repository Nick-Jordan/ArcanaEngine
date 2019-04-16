#ifndef AXIS_MAPPING_H_
#define AXIS_MAPPING_H_

#include "SceneDefines.h"

#include "InputBindings.h"

#include <string>
#include <vector>

namespace Arcana
{

	class ARCANA_SCENE_API AxisMapping
	{
		friend class InputComponent;

	public:

		static void registerAxisMapping(const std::string& name, std::vector<KeyMapping> keyMappings);

		static const AxisMapping* get(const std::string& name);

	public:

		AxisMapping(const std::string& name, std::vector<KeyMapping> keyMappings);

		AxisMapping(const AxisMapping& copy);

		~AxisMapping();

	private:

		static std::vector<AxisMapping> __axisMappings;

	private:

		std::string _name;
		std::vector<KeyMapping> _keyMappings;
	};

}

#endif // !AXIS_MAPPING_H_

