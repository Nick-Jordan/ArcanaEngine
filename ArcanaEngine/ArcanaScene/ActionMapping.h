#ifndef ACTION_MAPPING_H_
#define ACTION_MAPPING_H_

#include "SceneDefines.h"

#include "Key.h"

#include <string>
#include <vector>

namespace Arcana
{

	class ARCANA_SCENE_API ActionMapping
	{
	public:

		static void registerActionMapping(const std::string& name, std::vector<Key> keys);

		static const ActionMapping* get(const std::string& name);

	public:

		ActionMapping(const std::string& name, std::vector<Key> keys);

		ActionMapping(const ActionMapping& copy);

		~ActionMapping();

	private:

		static std::vector<ActionMapping> __actionMappings;

	private:

		std::string _name;
		std::vector<Key> _keys;
	};

}

#endif // !ACTION_MAPPING_H_

