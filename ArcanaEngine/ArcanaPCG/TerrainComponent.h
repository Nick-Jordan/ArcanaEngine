#ifndef TERRAIN_COMPONENT_H_
#define TERRAIN_COMPONENT_H_

#include "PCGDefines.h"

#include "GeometryComponent.h"
#include "TerrainRenderProcedure.h"

namespace Arcana
{

	class ARCANA_PCG_API TerrainComponent : public GeometryComponent
	{
	public:

		TerrainComponent();
		
		virtual ~TerrainComponent();

		virtual void initialize() override;

		virtual bool createRenderProcedure() override;

	private:

		Terrain* _terrain;
	};

}

#endif // !TERRAIN_COMPONENT_H_

