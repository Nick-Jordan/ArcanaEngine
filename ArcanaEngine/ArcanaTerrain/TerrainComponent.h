#ifndef TERRAIN_COMPONENT_H_
#define TERRAIN_COMPONENT_H_

#include "TerrainDefines.h"

#include "GeometryComponent.h"
#include "TerrainRenderProcedure.h"

namespace Arcana
{

	class ARCANA_TERRAIN_API TerrainComponent : public GeometryComponent
	{
	public:

		TerrainComponent(const Terrain::Parameters& params, Transform* transform);
		
		virtual ~TerrainComponent();

		virtual void initialize() override;

		virtual void update(double elapsedTime) override;

		virtual bool createRenderProcedure() override;

	private:

		Terrain* _terrain;

		Terrain::Parameters _parameters;

		Transform _transform;

		TerrainRenderProcedure* _terrainRenderProcedure;
	};

}

#endif // !TERRAIN_COMPONENT_H_

