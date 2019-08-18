#include "MeshRenderProcedure.h"

#include "ResourceManager.h"
#include "MeshIndexComponent.h"

namespace Arcana
{

	MeshRenderProcedure::MeshRenderProcedure(Mesh* mesh, Material* material, const RenderProcedure::RenderProperties& properties)
		: _mesh(mesh), _material(material)
	{
		AE_REFERENCE(_mesh);
		AE_REFERENCE(_material);

		Properties = properties;
	}


	MeshRenderProcedure::~MeshRenderProcedure()
	{
		AE_RELEASE(_mesh);
		AE_RELEASE(_material);
	}

	void MeshRenderProcedure::render()
	{
		Properties.RenderState.bind();

		if (_mesh)
		{
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

	void MeshRenderProcedure::renderWithShader(const Shader& shader, bool bindRenderState)
	{
		if(bindRenderState)
			Properties.RenderState.bind();

		_mesh->getVertexBuffer()->bind();

		uint32 componentCount = _mesh->getNumIndexComponents();

		Mesh::InstanceProperties instanceProperties = _mesh->getInstanceProperties();

		if (componentCount == 0)
		{
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
		}
		else
		{
			for (uint32 c = 0; c < componentCount; c++)
			{
				MeshIndexComponent* component = _mesh->getIndexComponent(c);

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
			}
		}

		_mesh->getVertexBuffer()->unbind();

		if(bindRenderState)
			Properties.RenderState.unbind();
	}

	bool MeshRenderProcedure::isValidProcedure()
	{
		return _mesh && _material;
	}
}