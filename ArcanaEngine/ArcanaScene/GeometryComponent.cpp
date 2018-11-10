#include "GeometryComponent.h"

namespace Arcana
{

	//test texture Cache
	TextureCache GeometryComponent::CachedTextures = TextureCache();

	GeometryComponent::GeometryComponent()
	{
		//TEST

		test = new Material("test");
		test->addAttribute("diffuse", Vector3f(1.0f, 1.0f, 0.0f));
		test->addAttribute("specular", Vector3f(1.0f, 0.5f, 0.0f));
		test->addAttribute("shininess", 0.5f);
		Shader shader;

		shader.createProgram(Shader::Vertex, "resources/test_shader_vert.glsl");
		shader.createProgram(Shader::Fragment, "resources/test_lighting_shader_frag.glsl");

		Technique* technique = new Technique(shader);
		test->addTechnique(technique);
		testRenderState.setCullEnabled(true);
		testRenderState.setDepthTestEnabled(true);
		testRenderState.setBlendEnabled(true);
		testRenderState.setBlendSrc(RenderState::Blend::SrcAlpha);
		testRenderState.setBlendDst(RenderState::Blend::OneMinusSrcAlpha);

		VertexFormat::Attribute attribs[] =
		{
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 3),
			VertexFormat::Attribute(VertexFormat::Semantic::Color, 4)
		};
		VertexFormat format(2, attribs);
		mesh = new Mesh(format, Mesh::TriangleStrip);
		float vertices[] = {
			-1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f,   1.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f
		};
		mesh->setVertexBuffer(format, 4)->setVertexData(vertices);

		Image<uint8> image;
		image.init("../TextureClient/resources/texture.png");

		/*testTexture = CachedTextures.get("test_texture");
		if (!testTexture)
		{
			testTexture = Texture::create2D(Texture::RGBA, 308, 308, Texture::RGBA8, Texture::UnsignedByte, image.getPixelsPtr());
			CachedTextures.addTexture("test_texture", testTexture);
		}*/

		testTexture = Texture::create2D(Texture::RGBA, 308, 308, Texture::RGBA8, Texture::UnsignedByte, image.getPixelsPtr());

		test->addAttribute("test_texture", testTexture);


		//TEST
	}

	GeometryComponent::~GeometryComponent()
	{
		AE_DELETE(test);
		AE_DELETE(mesh);
		AE_RELEASE(testTexture);
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
