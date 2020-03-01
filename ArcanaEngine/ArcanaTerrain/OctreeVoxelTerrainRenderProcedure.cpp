#include "OctreeVoxelTerrainRenderProcedure.h"

#include "Profiler.h"
#include "MarchingCubes.h"
#include "Noise.h"

namespace Arcana
{
	OctreeVoxelTerrainRenderProcedure::OctreeVoxelTerrainRenderProcedure(VoxelTerrain* terrain)
		: _terrain(terrain), _testMesh(nullptr), _terrainMaterial(nullptr)
	{
		AE_REFERENCE(_terrain);

		_terrainMaterial = new Material("terrain");
		Shader shader;
		shader.createProgram(Shader::Vertex, "resources/terrain/voxel_terrain_vert.glsl");
		Shader::Defines defines;
		//defines.addDefine("DEBUG_QUADTREE");
		shader.createProgram(Shader::Fragment, "resources/terrain/voxel_terrain_frag.glsl", defines);

		Technique* technique = new Technique(shader);
		_terrainMaterial->addTechnique(technique);

		VertexFormat::Attribute attribsTest[] =
		{
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 3),
		};
		VertexFormat formatTest(1, attribsTest);
		_testMesh = new Mesh(formatTest, Mesh::Lines);

		std::vector<Vector3f> verticesTest =
		{
			Vector3f(0, 0, 0),
			Vector3f(1, 0, 0),
			Vector3f(1, 0, 1),
			Vector3f(0, 0, 1),
			Vector3f(0, 1, 0),
			Vector3f(1, 1, 0),
			Vector3f(1, 1, 1),
			Vector3f(0, 1, 1),
		};
		std::vector<uint32> indicesTest =
		{
			//bottom
			0, 1, 1, 2, 2, 3, 3, 0,
			//top
			4, 5, 5, 6, 6, 7, 7, 4,
			//left
			0, 4, 7, 3,
			//right
			1, 5, 6, 2
		};
		_testMesh->setVertexBuffer(formatTest, verticesTest.size())->setVertexData(&verticesTest[0]);
		_testMesh->addIndexComponent(Mesh::Lines)->setIndexBuffer(IndexBuffer::Index32, indicesTest.size(), false, &indicesTest[0]);



		Properties.LightProperties.CastsDynamicShadow = false;
		Properties.RendererStage = "TransparentObjectStage";//environment
		Properties.RenderState.setCullEnabled(true);
		Properties.RenderState.setCullFaceSide(RenderState::Back);
		Properties.RenderState.setDepthTestEnabled(true);
		Properties.RenderState.setBlendEnabled(true);
		Properties.RenderState.setBlendSrc(RenderState::Blend::SrcAlpha);
		Properties.RenderState.setBlendDst(RenderState::Blend::OneMinusSrcAlpha);
	}

	OctreeVoxelTerrainRenderProcedure::~OctreeVoxelTerrainRenderProcedure()
	{
		if (_testMesh)
		{
			AE_DELETE(_testMesh);
		}

		if (_terrainMaterial)
		{
			AE_RELEASE(_terrainMaterial);
		}
	}

	void OctreeVoxelTerrainRenderProcedure::render()
	{
		updateTerrain();
		renderTerrain();
	}

	bool OctreeVoxelTerrainRenderProcedure::isValidProcedure()
	{
		return _terrain != nullptr && _terrainMaterial != nullptr;
	}

	void OctreeVoxelTerrainRenderProcedure::updateTerrain()
	{
		//_terrain->_transform = Transform.getMatrix();
		//_terrainMutex.lock();
		_terrain->update(Transform.getMatrix(), Projection, View, EyePosition);
		//_terrainMutex.unlock();
		/*{
			PROFILE("TerrainNode update");
			_terrain->_terrainNode->update(
				Transform.getMatrix(),
				Projection,
				View,
				EyePosition);
		}
		{
			PROFILE("TerrainNode setUniforms");
			_terrain->_terrainNode->getDeformation()->setUniforms(
				Transform.getMatrix(),
				Projection, View, EyePosition,
				_terrain->_terrainNode, _terrainMaterial->getCurrentTechnique()->getPass(0));
		}

		/*{
			PROFILE("Terrain getTerrainQuadVector");
			_data->_meshQueueMutex.lock();
			_data->_terrain->getTerrainQuadVector(_data->_meshes, _data->_context);
			_data->_meshQueueMutex.unlock();
		}*/
	}

	void OctreeVoxelTerrainRenderProcedure::renderTerrain()
	{
		Properties.RenderState.bind();

		Technique* technique = _terrainMaterial->getCurrentTechnique();
		if (technique)
		{
			for (uint32 i = 0; i < technique->getPassCount(); i++)
			{
				Shader* pass = technique->getPass(i);
				if (pass)
				{
					pass->bind();

					//Default Uniforms
					pass->getUniform("u_CameraPosition").setValue(EyePosition.cast<float>());
					pass->getUniform("u_ProjectionMatrix").setValue(Projection.cast<float>());
					pass->getUniform("u_ViewMatrix").setValue(View.cast<float>());

					//_terrainMutex.lock();
					_terrain->checkNodeData();
					drawOctreeNode(_terrain->_octree, pass);
					//_terrainMutex.unlock();

					pass->unbind();
				}
			}
		}

		_testMesh->getVertexBuffer()->bind();

		for (uint32 c = 0; c < 1; c++)
		{
			Technique* technique = _terrainMaterial->getTechnique(c);
			if (!technique)
			{
				technique = _terrainMaterial->getCurrentTechnique();
			}

			if (technique)
			{
				MeshIndexComponent* component = _testMesh->getIndexComponent(c);
				for (uint32 i = 0; i < technique->getPassCount(); i++)
				{
					Shader* pass = technique->getPass(i);
					if (pass)
					{
						pass->bind();

						//Default Uniforms
						pass->getUniform("u_CameraPosition").setValue(EyePosition.cast<float>());
						pass->getUniform("u_ProjectionMatrix").setValue(Projection.cast<float>());
						pass->getUniform("u_ViewMatrix").setValue(View.cast<float>());

						//_terrainMutex.lock();
						drawOctreeNodeTest(_terrain->_octree, component, pass);
						//_terrainMutex.unlock();

						pass->unbind();
					}
				}
			}
		}

		_testMesh->getVertexBuffer()->unbind();
		Properties.RenderState.unbind();
	}

	void OctreeVoxelTerrainRenderProcedure::drawOctreeNode(TerrainOctree* octree, Shader* shader)
	{
		if (octree->isLeaf())
		{
			Matrix4f transformMatrix = Matrix4f::createScale(Vector3f(1.1035 * octree->getPhysicalSize())); //test value 1.1035
			transformMatrix *= Matrix4f::createTranslation(octree->getPhysicalXCoordinate(), octree->getPhysicalYCoordinate(), octree->getPhysicalZCoordinate());

			shader->getUniform("u_ModelMatrix").setValue(transformMatrix);

			octree->getData()->render(shader);
		}
		else
		{
			for (int32 i = 0; i < 8; i++)
				drawOctreeNode(octree->getChild(i), shader);
		}
	}

	void OctreeVoxelTerrainRenderProcedure::drawOctreeNodeTest(TerrainOctree* octree, MeshIndexComponent* component, Shader* shader)
	{
		if (octree->isLeaf())
		{
			Matrix4f transformMatrix = Matrix4f::createScale(Vector3f(octree->getPhysicalSize()));
			transformMatrix *= Matrix4f::createTranslation(octree->getPhysicalXCoordinate(), octree->getPhysicalYCoordinate(), octree->getPhysicalZCoordinate());

			shader->getUniform("u_ModelMatrix").setValue(transformMatrix);

			component->getIndexBuffer()->bind();
			glDrawElements(component->getPrimitive(), component->getNumIndices(), component->getIndexFormat(), 0);
			component->getIndexBuffer()->unbind();
		}
		else
		{
			for (int32 i = 0; i < 8; i++)
				drawOctreeNodeTest(octree->getChild(i), component, shader);
		}
	}
}
