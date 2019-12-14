#include "GraphicalUserInterfaceStage.h"

#include "ObjectRenderer.h"

namespace Arcana
{

	GraphicalUserInterfaceStage::GraphicalUserInterfaceStage() : RenderStage("GraphicalUserInterfaceStage")
	{
	}


	GraphicalUserInterfaceStage::~GraphicalUserInterfaceStage()
	{
	}

	void GraphicalUserInterfaceStage::initialize()
	{

	}

	void GraphicalUserInterfaceStage::finalize()
	{

	}

	void GraphicalUserInterfaceStage::render(const RenderData& data)
	{
		for (auto i = Procedures.createConstIterator(); i; i++)
		{
			RenderProcedure* procedure = *i;

			if (procedure && procedure->isValidProcedure())
			{
				procedure->render();
			}
		}
	}
}
