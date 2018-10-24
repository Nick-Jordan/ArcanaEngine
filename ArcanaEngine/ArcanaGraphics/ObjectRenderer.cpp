#include "ObjectRenderer.h"

namespace Arcana
{
	//test numQueued
	ObjectRenderer::ObjectRenderer() : _updateMeshQueue(false), numQueued(0)
	{

	}

	ObjectRenderer::~ObjectRenderer()
	{

	}


	void ObjectRenderer::render()// Camera& camera, RenderTarget* renderTarget)
	{
		if (_updateMeshQueue)
		{
			std::sort(_queuedMeshes.begin(), _queuedMeshes.end(), 
				[](const MeshRenderContext& a, const MeshRenderContext& b) 
					{ 
						//temporary!!!!! (should be sorting based on required texture binds)
						return a.material->getId().getId() > b.material->getId().getId(); 
					}
			);
			_updateMeshQueue = false;
		}

		std::vector<MeshRenderContext>::iterator iter;
		for (iter = _queuedMeshes.begin(); iter != _queuedMeshes.end(); iter++)
		{
			MeshRenderContext& context = *iter;

			context.renderState.bind();
			context.mesh->getVertexBuffer()->bind();

			uint32 componentCount = context.mesh->getNumIndexComponents();

			if (componentCount == 0)
			{
				Technique* technique = context.material->getCurrentTechnique();
				if (technique)
				{
					for (uint32 i = 0; i < technique->getPassCount(); ++i)
					{
						Shader* pass = technique->getPass(i);
						if (pass)
						{
							pass->bind();
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
						/*MeshIndexComponent* component = context.mesh->getIndexComponent(c);
						for (uint32 i = 0; i < technique->getPassCount(); ++i)
						{
							Shader* pass = technique->getPass(i);
							if (pass)
							{
								pass->bind();
								component->getIndexBuffer()->bind();
								glDrawElements(component->getPrimitive(), component->getNumIndices(), component->getIndexFormat(), 0);
								component->getIndexBuffer()->unbind();
								pass->unbind();
							}
						}*/
					}
				}
			}

			context.mesh->getVertexBuffer()->unbind();
			context.renderState.unbind();
		}
	}

	void ObjectRenderer::queueMesh(const MeshRenderContext& context)
	{
		if (context.isValid())
		{
			_queuedMeshes.push_back(context);
			numQueued++;
		}

		_updateMeshQueue = true;
	}
}