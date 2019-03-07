#ifndef MATERIAL_MAP_H_
#define MATERIAL_MAP_H_

#include "GraphicsDefines.h"

#include "Material.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API MaterialMap : public Material
	{
	public:

		MaterialMap();

		MaterialMap(const std::string& name);

		~MaterialMap();

		void addTechniqueMapping(uint32 componentIndex, uint32 techniqueIndex);

		virtual Technique* getTechnique(uint32 index) override;

	private:

		std::map<uint32, uint32> _techniqueMapping;
	};
}

#endif // !MATERIAL_MAP_H_

