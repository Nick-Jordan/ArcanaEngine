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

		TerrainComponent(Transform* transform);
		
		virtual ~TerrainComponent();

		virtual void initialize() override;

		virtual void update(double elapsedTime) override;

		virtual bool createRenderProcedure() override;

	private:

		Terrain* _terrain;

		Transform _transform;

		TerrainRenderProcedure* _terrainRenderProcedure;
	};

}

#endif // !TERRAIN_COMPONENT_H_

