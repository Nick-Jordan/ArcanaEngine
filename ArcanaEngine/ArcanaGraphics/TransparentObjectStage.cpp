#include "TransparentObjectStage.h"

#include "ObjectRenderer.h"

namespace Arcana
{

	TransparentObjectStage::TransparentObjectStage() : RenderStage("TransparentObjectStage")
	{
	}


	TransparentObjectStage::~TransparentObjectStage()
	{
	}

	void TransparentObjectStage::initialize()
	{
	}

	void TransparentObjectStage::finalize()
	{

	}

	void TransparentObjectStage::render(const RenderData& data)
	{
		for (auto i = Procedures.createConstIterator(); i; i++)
		{
			RenderProcedure* procedure = *i;

			if (procedure && procedure->isValidProcedure())
			{
				procedure->View = data.View;
				procedure->Projection = data.Projection;
				procedure->EyePosition = data.EyePosition;
				procedure->Properties.RenderState.setBlendEnabled(true);
				procedure->Properties.RenderState.setBlendSrc(RenderState::Blend::SrcAlpha);//maybe remove
				procedure->Properties.RenderState.setBlendDst(RenderState::Blend::OneMinusSrcAlpha);//maybe remove

				if (procedure->FTLResult.LightMap)
				{
					RenderProcedure::UniformParameter lightMap;
					lightMap.name = "u_LightMap";
					lightMap.value.type = Uniform::Value::Int32;
					lightMap.value.i = procedure->FTLResult.LightMap->bind();
					procedure->Uniforms.push_back(lightMap);
				}

				if (procedure->FTLResult.IndirectLightData.getData() != nullptr)
				{
					RenderProcedure::UniformParameter indirectLightData;
					indirectLightData.name = "u_IndirectLightData.data";
					indirectLightData.value.type = Uniform::Value::Int32;
					indirectLightData.value.i = procedure->FTLResult.IndirectLightData.getData()->bind();

					RenderProcedure::UniformParameter indirectLightBoundsMin;
					indirectLightBoundsMin.name = "u_IndirectLightData.boundsMin";
					indirectLightBoundsMin.value.type = Uniform::Value::Vec3f;
					indirectLightBoundsMin.value.vec3 = procedure->FTLResult.IndirectLightData.getBoundingBox().getMin();

					RenderProcedure::UniformParameter indirectLightBoundsMax;
					indirectLightBoundsMax.name = "u_IndirectLightData.boundsMax";
					indirectLightBoundsMax.value.type = Uniform::Value::Vec3f;
					indirectLightBoundsMax.value.vec3 = procedure->FTLResult.IndirectLightData.getBoundingBox().getMax();

					procedure->Uniforms.push_back(indirectLightData);
					procedure->Uniforms.push_back(indirectLightBoundsMin);
					procedure->Uniforms.push_back(indirectLightBoundsMax);
				}

				procedure->render();
			}
		}
	}
}
