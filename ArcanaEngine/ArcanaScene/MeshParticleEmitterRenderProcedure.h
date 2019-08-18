#ifndef MESH_PARTICLE_EMITTER_RENDER_PROCEDURE_H_
#define MESH_PARTICLE_EMITTER_RENDER_PROCEDURE_H_

#include "SceneDefines.h"

#include "RenderProcedure.h"
#include "MeshRenderProcedure.h"

#include "MeshParticleEmitterComponent.h"

namespace Arcana
{
	class ARCANA_SCENE_API MeshParticleEmitterRenderProcedure : public RenderProcedure
	{
	public:

		Particle* Particles;
		uint32 NumParticles;
		VertexFormat InstanceFormat;
		Texture* Texture;

		MeshParticleEmitterRenderProcedure(Mesh* mesh, Material* material, const RenderProcedure::RenderProperties& properties);

		virtual ~MeshParticleEmitterRenderProcedure();

		virtual void render() override;

		virtual bool isValidProcedure() override;

	private:

		Mesh* _mesh;
		Material* _material;
		RenderProcedure::RenderProperties _properties;
	};

}

#endif // !MESH_PARTICLE_EMITTER_RENDER_PROCEDURE_H_

