#ifndef PARTICLE_EMITTER_COMPONENT_H_
#define PARTICLE_EMITTER_COMPONENT_H_

#include "SceneDefines.h"

#include "MeshParticleEmitterComponent.h"

namespace Arcana
{
	class ARCANA_SCENE_API ParticleEmitterComponent : public MeshParticleEmitterComponent
	{
	public:

		ParticleEmitterComponent();

		ParticleEmitterComponent(uint32 maxParticles, const ParticleEmitterProperties& properties, const Shader& shader);

		~ParticleEmitterComponent();
	};
}

#endif // !PARTICLE_EMITTER_COMPONENT_H_

