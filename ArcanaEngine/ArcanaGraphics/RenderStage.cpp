#include "RenderStage.h"

namespace Arcana
{

	RenderStage::RenderStage(std::string identifier) : _identifier(identifier)
	{
	}


	RenderStage::~RenderStage()
	{
	}

	void RenderStage::addProcedure(RenderProcedure* procedure)
	{
		if (procedure)
		{
			Procedures.add(procedure);
		}
	}

	void RenderStage::addLight(const RenderLight& light)
	{
		Lights.add(light);
	}

	void RenderStage::clearProcedures()
	{
		Procedures.clear();
	}

	void RenderStage::clearLights()
	{
		Lights.clear();
	}

	const GlobalObjectID& RenderStage::getId() const
	{
		return _identifier;
	}
}
