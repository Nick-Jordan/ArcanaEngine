#include "MeshParticleEmitterRenderProcedure.h"

#include "Vertex.h"

namespace Arcana
{

	MeshParticleEmitterRenderProcedure::MeshParticleEmitterRenderProcedure(Mesh* mesh, Material* material, const MeshRenderProperties& properties)
		: _data(nullptr), _mesh(mesh), _material(material), _properties(properties)
	{
		AE_REFERENCE(_mesh);
		AE_REFERENCE(_material);
	}


	MeshParticleEmitterRenderProcedure::~MeshParticleEmitterRenderProcedure()
	{
		if (_data)
		{
			AE_DELETE(_data);
		}

		AE_RELEASE(_mesh);
		AE_RELEASE(_material);
	}

	bool MeshParticleEmitterRenderProcedure::isDirty() const
	{
		return true;
	}

	void MeshParticleEmitterRenderProcedure::markDirty(bool dirty)
	{
		//nothing
	}

	void MeshParticleEmitterRenderProcedure::createRenderData()
	{
		if (_data)
		{
			AE_DELETE(_data);
		}

		_data = new MeshRenderData();

		_data->context.mesh = _mesh;
		_data->context.material = _material;
		_data->context.renderProperties = _properties;
		_data->context.transform.setIdentity();
	}

	void MeshParticleEmitterRenderProcedure::updateRenderData(const RenderDataUpdate& data)
	{
		if (isValidProcedure())
		{
			const MeshParticleEmitterRenderDataUpdate& update = static_cast<const MeshParticleEmitterRenderDataUpdate&>(data);

			_data->context.projectionMatrix = update.projection;
			_data->context.viewMatrix = update.view;
			_data->context.eyePosition = update.eyePosition;
			_data->context.transform.set(update.transform);

			//update particles

			_data->context.mesh->getInstanceProperties()._numInstances = update.numParticles;

			std::vector<ParticleVertex> instanceData; //change to array (faster)

			for (uint32 i = 0; i < update.numParticles; i++)
			{
				Particle p = update.particles[i];
				ParticleVertex vertex;
				vertex.position = p.position.cast<float>();
				vertex.size = Vector3f(p.size);
				vertex.color = p.color;
				Matrix3f rotation = Matrix3f::createRotation(p.rotationAxis.cast<float>(), (float)p.angle);
				rotation.toAxes(vertex.rotation0, vertex.rotation1, vertex.rotation2);
				instanceData.push_back(vertex);
			}

			//std::sort(instanceData.begin(), instanceData.end(), [&](ParticleVertex a, ParticleVertex b)
			//{
			//	return Vector3f::distanceSq(a.position, update.eyePosition.cast<float>()) < Vector3f::distanceSq(b.position, update.eyePosition.cast<float>());
			//});

			MeshRenderContext::UniformParameter right;
			right.name = "u_ViewRight";
			right.value.type = Uniform::Value::Vec3f;
			right.value.vec3 = Vector3f(update.view.at(0, 0), update.view.at(1, 0), update.view.at(2, 0));
			MeshRenderContext::UniformParameter up;
			up.name = "u_ViewUp";
			up.value.type = Uniform::Value::Vec3f;
			up.value.vec3 = Vector3f(update.view.at(0, 1), update.view.at(1, 1), update.view.at(2, 1));
			MeshRenderContext::UniformParameter texture;
			texture.name = "u_Texture";
			texture.value.type = Uniform::Value::Int32;
			texture.value.i = update.texture ? update.texture->bind() : 0;

			_data->context.uniforms.clear();
			_data->context.uniforms.push_back(right);
			_data->context.uniforms.push_back(up);
			_data->context.uniforms.push_back(texture);

			_data->context.mesh->getInstanceBuffer()->bind();
			if (update.numParticles)
			{
				glBufferData(GL_ARRAY_BUFFER, update.instanceFormat.getVertexSize() * update.numParticles, &instanceData[0].position.x, GL_DYNAMIC_DRAW);
			}
			else
			{
				glMapBuffer(GL_ARRAY_BUFFER, GL_MAP_INVALIDATE_BUFFER_BIT);
			}
			_data->context.mesh->getInstanceBuffer()->unbind();
		}
	}

	RenderData* MeshParticleEmitterRenderProcedure::getRenderData() const
	{
		return _data;
	}

	bool MeshParticleEmitterRenderProcedure::isValidProcedure()
	{
		return _data != nullptr;
	}
}