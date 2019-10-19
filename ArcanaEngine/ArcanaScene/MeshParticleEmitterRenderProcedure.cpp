#include "MeshParticleEmitterRenderProcedure.h"

#include "Vertex.h"
#include "MeshIndexComponent.h"

namespace Arcana
{

	MeshParticleEmitterRenderProcedure::MeshParticleEmitterRenderProcedure(Mesh* mesh, Material* material, const RenderProcedure::RenderProperties& properties)
		: _mesh(mesh), _material(material)
	{
		AE_REFERENCE(_mesh);
		AE_REFERENCE(_material);

		Properties = properties;
	}


	MeshParticleEmitterRenderProcedure::~MeshParticleEmitterRenderProcedure()
	{
		AE_RELEASE(_mesh);
		AE_RELEASE(_material);
	}

	void MeshParticleEmitterRenderProcedure::render()
	{
		if (NumParticles == 0)
			return;

		Properties.RenderState.bind();

		if (_mesh)
		{
			_mesh->getInstanceProperties()._numInstances = NumParticles;

			std::vector<ParticleVertex> instanceData; //change to array (faster)

			for (uint32 i = 0; i < NumParticles; i++)
			{
				Particle p = Particles[i];
				ParticleVertex vertex;
				vertex.position = p.position.cast<float>();
				vertex.size = Vector3f(p.size);
				vertex.color = p.color;
				Matrix3f rotation = Matrix3f::createRotation(p.rotationAxis.cast<float>(), (float)p.angle);
				rotation.toAxes(vertex.rotation0, vertex.rotation1, vertex.rotation2);
				instanceData.push_back(vertex);
			}

			std::sort(instanceData.begin(), instanceData.end(), [&](ParticleVertex a, ParticleVertex b)
			{
				return Vector3f::distanceSq(a.position, EyePosition.cast<float>()) > Vector3f::distanceSq(b.position, EyePosition.cast<float>());
			});

			_mesh->getInstanceBuffer()->bind();
			if (NumParticles)
			{
				glBufferData(GL_ARRAY_BUFFER, InstanceFormat.getVertexSize() * NumParticles, &instanceData[0].position.x, GL_DYNAMIC_DRAW);
			}
			else
			{
				glMapBuffer(GL_ARRAY_BUFFER, GL_MAP_INVALIDATE_BUFFER_BIT);
				_mesh->getInstanceBuffer()->unbind();
				return;
			}
			_mesh->getInstanceBuffer()->unbind();

			_mesh->getVertexBuffer()->bind();

			Mesh::InstanceProperties instanceProperties = _mesh->getInstanceProperties();

			uint32 componentCount = _mesh->getNumIndexComponents();

			if (componentCount == 0)
			{
				Technique* technique = _material->getCurrentTechnique();
				if (technique)
				{
					_material->bindMaterialTextures(technique);

					for (uint32 i = 0; i < technique->getPassCount(); i++)
					{
						Shader* pass = technique->getPass(i);
						if (pass)
						{
							pass->bind();

							_material->passMaterialAttributes(pass, technique);

							//pass FTL results

							//Default Uniforms
							pass->getUniform("u_ProjectionMatrix").setValue(Projection.cast<float>());
							pass->getUniform("u_ViewMatrix").setValue(View.cast<float>());
							pass->getUniform("u_ModelMatrix").setValue(Transform.getMatrix().cast<float>());
							pass->getUniform("u_NormalMatrix").setValue(Transform.getMatrix().toMatrix3().inverse().transpose().cast<float>());
							pass->getUniform("u_CameraPosition").setValue(EyePosition.cast<float>());

							pass->getUniform("u_ViewRight").setValue(Vector3f(View.at(0, 0), View.at(1, 0), View.at(2, 0)));
							pass->getUniform("u_ViewUp").setValue(Vector3f(View.at(0, 1), View.at(1, 1), View.at(2, 1)));
							pass->getUniform("u_Texture").setValue(Texture ? Texture->bind() : 0);

							for (uint32 j = 0; j < Uniforms.size(); j++)
							{
								pass->getUniform(Uniforms[j].name).setValue(Uniforms[j].value);
							}

							glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
							if (instanceProperties.isInstanced())
							{
								_mesh->getInstanceBuffer()->bind();
								glDrawArraysInstanced(_mesh->getPrimitive(), 0, _mesh->getNumVertices(), instanceProperties.getNumInstances());
								_mesh->getInstanceBuffer()->unbind();
							}
							else
							{
								glDrawArrays(_mesh->getPrimitive(), 0, _mesh->getNumVertices());
							}

							pass->unbind();
						}
					}
				}
			}
			else
			{
				/*if (componentCount != context.material->getTechniqueCount())
				{
					LOG(Warning, CoreEngine, "Material technique count not equal to component count");
				}*/

				for (uint32 c = 0; c < componentCount; c++)
				{
					Technique* technique = _material->getTechnique(c);
					if (!technique)
					{
						technique = _material->getCurrentTechnique();
					}

					if (technique)
					{
						_material->bindMaterialTextures(technique);

						MeshIndexComponent* component = _mesh->getIndexComponent(c);
						for (uint32 i = 0; i < technique->getPassCount(); i++)
						{
							Shader* pass = technique->getPass(i);
							if (pass)
							{
								pass->bind();

								_material->passMaterialAttributes(pass, technique);

								//Default Uniforms
								pass->getUniform("u_ProjectionMatrix").setValue(Projection.cast<float>());
								pass->getUniform("u_ViewMatrix").setValue(View.cast<float>());
								pass->getUniform("u_ModelMatrix").setValue(Transform.getMatrix().cast<float>());
								pass->getUniform("u_NormalMatrix").setValue(Transform.getMatrix().toMatrix3().inverse().transpose().cast<float>());
								pass->getUniform("u_CameraPosition").setValue(EyePosition.cast<float>());

								pass->getUniform("u_ViewRight").setValue(Vector3f(View.at(0, 0), View.at(1, 0), View.at(2, 0)));
								pass->getUniform("u_ViewUp").setValue(Vector3f(View.at(0, 1), View.at(1, 1), View.at(2, 1)));
								pass->getUniform("u_Texture").setValue(Texture ? Texture->bind() : 0);

								component->getIndexBuffer()->bind();
								if (instanceProperties.isInstanced())
								{
									_mesh->getInstanceBuffer()->bind();
									glDrawElementsInstanced(component->getPrimitive(), component->getNumIndices(), component->getIndexFormat(), 0, instanceProperties.getNumInstances());
									_mesh->getInstanceBuffer()->unbind();
								}
								else
								{
									glDrawElements(component->getPrimitive(), component->getNumIndices(), component->getIndexFormat(), 0);
								}
								component->getIndexBuffer()->unbind();
								pass->unbind();
							}
						}
					}
				}
			}

			_mesh->getVertexBuffer()->unbind();
		}

		Properties.RenderState.unbind();
	}

	bool MeshParticleEmitterRenderProcedure::isValidProcedure()
	{
		return _mesh && _material;
	}
}