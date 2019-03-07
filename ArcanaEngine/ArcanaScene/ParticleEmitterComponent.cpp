#include "ParticleEmitterComponent.h"

namespace Arcana
{

	ParticleEmitterComponent::ParticleEmitterComponent()
	{
	}

	ParticleEmitterComponent::ParticleEmitterComponent(uint32 maxParticles, const ParticleEmitterProperties& properties, const Shader& shader)
	{
		VertexFormat::Attribute attribs[] =
		{
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 3),
		};
		VertexFormat format(1, attribs);
		Mesh* mesh = new Mesh(format, Mesh::TriangleStrip);
		float vertices[] =
		{
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f
		};
		mesh->setVertexBuffer(format, 4)->setVertexData(vertices);

		MeshParticleEmitterComponent::initialize(mesh, maxParticles, properties, shader);
	}

	ParticleEmitterComponent::~ParticleEmitterComponent()
	{
	}
}
