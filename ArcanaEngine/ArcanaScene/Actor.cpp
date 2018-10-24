#include "Actor.h"

///test
#include "MeshRenderContext.h"
#include "ObjectRenderer.h"

namespace Arcana
{

	Actor::Actor() : _shape(nullptr)
	{

		//TEST

		test = new Material("test");
		Shader shader;
		LOG(Info, CoreEngine, "CREATING SHADER");
		shader.createProgram(Shader::Vertex, "resources/test_shader_vert.glsl");
		shader.createProgram(Shader::Fragment, "resources/test_shader_frag.glsl");
		LOG(Info, CoreEngine, "DONE CREATING SHADERS");
		Technique technique(shader);
		test->addTechnique(technique);
		testRenderState.setCullEnabled(false);
		testRenderState.setDepthTestEnabled(false);

		LOG(Info, CoreEngine, "DONE CREATING MATERIAL");

		//TEST
	}

	Actor::Actor(const Actor& actor) : _shape(actor._shape), _transform(actor._transform)
	{

	}

	Actor::~Actor()
	{
		//TEST

		AE_DELETE(test);

		//TEST
	}


	void Actor::update(double elapsedTime)
	{
		LOGF(Info, CoreEngine, "Actor updated: %f", elapsedTime);
	}

	void Actor::render(ObjectRenderer& renderer)
	{
		//tEST

		if (_shape)
		{
			MeshRenderContext context;
			context.mesh = getShape()->getMesh();
			context.material = test;
			context.renderState = testRenderState;
			context.transform.setIdentity();

			renderer.queueMesh(context);
		}
	}


	Transform& Actor::getTransform()
	{
		return _transform;
	}

	void Actor::setTransform(const Transform& transform)
	{
		_transform = Transform(transform);
	}

	Shape* Actor::getShape() const
	{
		return _shape;
	}

	void Actor::setShape(Shape* shape)
	{
		_shape = shape;
	}

}
