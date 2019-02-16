#include "TransparentObjectStage.h"

#include "MeshIndexComponent.h"

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

	void TransparentObjectStage::render()
	{
		for (auto i = Meshes.createConstIterator(); i; i++)
		{
			MeshRenderContext context = *i;

			if (context.isValid())
			{
				context.callback.executeIfBound();

				context.renderState.bind();

				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				context.mesh->getVertexBuffer()->bind();

				uint32 componentCount = context.mesh->getNumIndexComponents();

				if (componentCount == 0)
				{
					Technique* technique = context.material->getCurrentTechnique();
					if (technique)
					{
						for (uint32 i = 0; i < technique->getPassCount(); i++)
						{
							Shader* pass = technique->getPass(i);
							if (pass)
							{
								pass->bind();

								if (technique->needsMaterialAttributes())
								{
									context.material->passMaterialAttributes(pass, technique);
								}

								//Default Uniforms
								pass->getUniform("u_ProjectionMatrix")->setValue(context.projectionMatrix.cast<float>());
								pass->getUniform("u_ViewMatrix")->setValue(context.viewMatrix.cast<float>());
								pass->getUniform("u_ModelMatrix")->setValue(context.transform.getMatrix().cast<float>());
								pass->getUniform("u_NormalMatrix")->setValue(context.transform.getMatrix().toMatrix3().inverse().transpose().cast<float>());
								pass->getUniform("u_CameraPosition")->setValue(context.eyePosition.cast<float>());

								for (uint32 j = 0; j < context.uniforms.size(); j++)
								{
									pass->getUniform(context.uniforms[j].name)->setValue(context.uniforms[j].value);
								}

								glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
								glDrawArrays(context.mesh->getPrimitive(), 0, context.mesh->getNumVertices());

								pass->unbind();
							}
						}
					}
				}
				else
				{
					if (componentCount != context.material->getTechniqueCount())
					{
						LOG(Warning, CoreEngine, "Material technique count not equal to component count");
					}

					for (uint32 c = 0; c < componentCount; c++)
					{
						Technique* technique = context.material->getTechnique(c);
						if (!technique)
						{
							technique = context.material->getCurrentTechnique();
						}

						if (technique)
						{
							MeshIndexComponent* component = context.mesh->getIndexComponent(c);
							for (uint32 i = 0; i < technique->getPassCount(); i++)
							{
								Shader* pass = technique->getPass(i);
								if (pass)
								{
									pass->bind();

									if (technique->needsMaterialAttributes())
									{
										context.material->passMaterialAttributes(pass, technique);
									}

									//Default Uniforms
									pass->getUniform("u_ProjectionMatrix")->setValue(context.projectionMatrix.cast<float>());
									pass->getUniform("u_ViewMatrix")->setValue(context.viewMatrix.cast<float>());
									pass->getUniform("u_ModelMatrix")->setValue(context.transform.getMatrix().cast<float>());
									pass->getUniform("u_NormalMatrix")->setValue(context.transform.getMatrix().toMatrix3().inverse().transpose().cast<float>());
									pass->getUniform("u_CameraPosition")->setValue(context.eyePosition.cast<float>());

									component->getIndexBuffer()->bind();
									glDrawElements(component->getPrimitive(), component->getNumIndices(), component->getIndexFormat(), 0);
									component->getIndexBuffer()->unbind();
									pass->unbind();
								}
							}
						}
					}
				}

				context.mesh->getVertexBuffer()->unbind();
				context.renderState.unbind();
			}
		}
	}
}
