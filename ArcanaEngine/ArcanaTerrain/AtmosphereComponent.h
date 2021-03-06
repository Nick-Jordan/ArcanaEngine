#ifndef ATMOSPHERE_COMPONENT_H_
#define ATMOSPHERE_COMPONENT_H_

#include "TerrainDefines.h"

#include "GeometryComponent.h"
#include "AtmosphereRenderProcedure.h"

namespace Arcana
{

	class ARCANA_TERRAIN_API AtmosphereComponent : public GeometryComponent
	{
	public:

		AtmosphereComponent();

		virtual ~AtmosphereComponent();

		virtual void initialize() override;

		virtual void update(double elapsedTime) override;

		virtual bool createRenderProcedure() override;

	private:

		AtmosphereRenderProcedure* _atmosphereRenderProcedure;
	};

}

#endif // !ATMOSPHERE_COMPONENT_H_*/