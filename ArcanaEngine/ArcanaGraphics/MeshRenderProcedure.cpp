#include "MeshRenderProcedure.h"

#include "ResourceManager.h"

namespace Arcana
{

	MeshRenderProcedure::MeshRenderProcedure() : _data(nullptr)
	{
		//TEST

		/*test = new Material("test");
		test->addAttribute("diffuse", Vector3f(1.0f, 1.0f, 0.0f));
		test->addAttribute("specular", Vector3f(1.0f, 0.5f, 0.0f));
		test->addAttribute("shininess", 0.5f);
		Shader shader;

		shader.createProgram(Shader::Vertex, "resources/test_shader_vert.glsl");
		shader.createProgram(Shader::Fragment, "resources/test_lighting_shader_frag.glsl");

		Technique* technique = new Technique(shader);
		test->addTechnique(technique);*/

		test = ResourceManager::instance().loadResource<Material>("material_0");

		LOG(Debug, CoreEngine, "Done creating material");

		testRenderState.setCullEnabled(true);
		testRenderState.setDepthTestEnabled(true);
		testRenderState.setBlendEnabled(true);
		testRenderState.setBlendSrc(RenderState::Blend::SrcAlpha);
		testRenderState.setBlendDst(RenderState::Blend::OneMinusSrcAlpha);

		VertexFormat::Attribute attribs[] =
		{
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 3),
			VertexFormat::Attribute(VertexFormat::Semantic::Normal, 3)
		};
		VertexFormat format(2, attribs);
		mesh = new Mesh(format, Mesh::Triangles);
		float size = 5.0f;
		
		float vertices[] = {
			//front			
			-size, -size, size, 0.0f, 0.0f, 1.0f,
			size, size, size, 0.0f, 0.0f, 1.0f,
			-size, size, size, 0.0f, 0.0f, 1.0f,
			size, size, size, 0.0f, 0.0f, 1.0f,
			-size, -size, size, 0.0f, 0.0f, 1.0f,
			size, -size, size, 0.0f, 0.0f, 1.0f,

			//back
			size, -size, -size, 0.0f, 0.0f, -1.0f,	
			-size, -size, -size, 0.0f, 0.0f, -1.0f,	
			size, size, -size, 0.0f, 0.0f, -1.0f,
			-size, size, -size, 0.0f, 0.0f, -1.0f,
			size, size, -size, 0.0f, 0.0f, -1.0f,
			-size, -size, -size, 0.0f, 0.0f, -1.0f,

			//right
			size, -size, size, 1.0f, 0.0f, 0.0f,
			size, size, -size, 1.0f, 0.0f, 0.0f,
			size, size, size, 1.0f, 0.0f, 0.0f,
			size, size, -size, 1.0f, 0.0f, 0.0f,
			size, -size, size, 1.0f, 0.0f, 0.0f,
			size, -size, -size, 1.0f, 0.0f, 0.0f,

			//left
			-size, -size, -size, -1.0f, 0.0f, 0.0f,
			-size, -size, size, -1.0f, 0.0f, 0.0f,		
			-size, size, -size, -1.0f, 0.0f, 0.0f,	
			-size, size, size, -1.0f, 0.0f, 0.0f,
			-size, size, -size, -1.0f, 0.0f, 0.0f,
			-size, -size, size, -1.0f, 0.0f, 0.0f,

			//top
			-size, size, size, 0.0f, 1.0f, 0.0f,
			size, size, -size, 0.0f, 1.0f, 0.0f,
			-size, size, -size, 0.0f, 1.0f, 0.0f,
			size, size, -size, 0.0f, 1.0f, 0.0f,
			-size, size, size, 0.0f, 1.0f, 0.0f,
			size, size, size, 0.0f, 1.0f, 0.0f,

			//bottom
			size, -size, size, 0.0f, -1.0f, 0.0f,
			-size, -size, size, 0.0f, -1.0f, 0.0f,		
			size, -size, -size, 0.0f, -1.0f, 0.0f,
			-size, -size, -size, 0.0f, -1.0f, 0.0f,
			size, -size, -size, 0.0f, -1.0f, 0.0f,
			-size, -size, size, 0.0f, -1.0f, 0.0f,
		};
		mesh->setVertexBuffer(format, 36)->setVertexData(vertices);

		//TEST
	}


	MeshRenderProcedure::~MeshRenderProcedure()
	{
		if (_data)
		{
			AE_DELETE(_data);
		}


		//test
		AE_RELEASE(test);
		AE_DELETE(mesh);
	}

	bool MeshRenderProcedure::isDirty() const
	{
		return true;
	}

	void MeshRenderProcedure::markDirty(bool dirty)
	{
		//nothing
	}

	void MeshRenderProcedure::createRenderData()
	{
		if (_data)
		{
			AE_DELETE(_data);
		}

		_data = new MeshRenderData();

		_data->context.mesh = mesh;
		_data->context.material = test;
		_data->context.renderState = testRenderState;
		_data->context.transform.setIdentity();
	}

	void MeshRenderProcedure::updateRenderData(const RenderDataUpdate& data)
	{
		if (isValidProcedure())
		{
			_data->context.projectionMatrix = data.projection;
			_data->context.viewMatrix = data.view;
		}
	}

	RenderData* MeshRenderProcedure::getRenderData() const
	{
		return _data;
	}

	bool MeshRenderProcedure::isValidProcedure()
	{
		return _data != nullptr;
	}


	//MeshRenderData

	void MeshRenderData::render(ObjectRenderer& renderer)
	{
		renderer.queueMesh(context);
	}
}