#include "GeometryComponent.h"

namespace Arcana
{
	GeometryComponent::GeometryComponent()
	{
		//TEST

		test = new Material("test");
		Shader shader;

		shader.createProgram(Shader::Vertex, "resources/test_shader_vert.glsl");
		shader.createProgram(Shader::Fragment, "resources/test_shader_frag.glsl");

		Technique technique(shader);
		test->addTechnique(technique);
		testRenderState.setCullEnabled(false);
		testRenderState.setDepthTestEnabled(false);
		testRenderState.setBlendEnabled(true);
		testRenderState.setBlendSrc(RenderState::Blend::SrcAlpha);
		testRenderState.setBlendDst(RenderState::Blend::OneMinusSrcAlpha);

		VertexFormat::Attribute attribs[] =
		{
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 3),
			VertexFormat::Attribute(VertexFormat::Semantic::Color, 4)
		};
		VertexFormat format(2, attribs);
		mesh = new Mesh(format, Mesh::Triangles);
		float vertices[] = {
			-1.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f
		};
		mesh->setVertexBuffer(format, 3)->setVertexData(vertices);

		//TEST
	}

	GeometryComponent::~GeometryComponent()
	{
		//delete test;
		delete mesh;
	}

	bool GeometryComponent::hasRenderObject() const
	{
		return true;
	}


	void GeometryComponent::render(ObjectRenderer& renderer, Matrix4f view, Matrix4f projection)
	{
		MeshRenderContext context;
		context.mesh = mesh;
		context.material = test;
		context.renderState = testRenderState;
		context.transform.setIdentity();
		context.viewMatrix = view;
		context.projectionMatrix = projection;

		renderer.queueMesh(context);
	}
}
