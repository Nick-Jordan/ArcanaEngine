#ifndef MESH_PARTICLE_EMITTER_RENDER_PROCEDURE_H_
#define MESH_PARTICLE_EMITTER_RENDER_PROCEDURE_H_

#include "SceneDefines.h"

#include "RenderProcedure.h"
#include "MeshRenderProcedure.h"

#include "MeshParticleEmitterComponent.h"

namespace Arcana
{
	class ARCANA_SCENE_API MeshParticleEmitterRenderDataUpdate : public RenderDataUpdate
	{
	public:

		Particle* particles;
		uint32 numParticles;
		VertexFormat instanceFormat;
		Texture* texture;
	};

	class ARCANA_SCENE_API MeshParticleEmitterRenderProcedure : public RenderProcedure
	{
	public:

		MeshParticleEmitterRenderProcedure(Mesh* mesh, Material* material, const MeshRenderProperties& properties);

		virtual ~MeshParticleEmitterRenderProcedure();

		virtual bool isDirty() const override;

		virtual void markDirty(bool dirty) override;

		virtual void createRenderData() override;

		virtual void updateRenderData(const RenderDataUpdate& data) override;

		virtual RenderData* getRenderData() const override;

		virtual bool isValidProcedure() override;

	private:

		MeshRenderData* _data;
		Mesh* _mesh;
		Material* _material;
		MeshRenderProperties _properties;
	};

}

#endif // !MESH_PARTICLE_EMITTER_RENDER_PROCEDURE_H_

