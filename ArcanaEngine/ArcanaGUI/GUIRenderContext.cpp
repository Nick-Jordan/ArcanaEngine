#include "GUIRenderContext.h"

namespace Arcana
{
	uint16 GUIRenderContext::__rectIndices[6] =
	{
		0, 1, 2, 2, 3, 0
	};

	GUIRenderContext::GUIRenderContext() : _renderer(nullptr)
	{
	}


	GUIRenderContext::~GUIRenderContext()
	{
		AE_DELETE(_renderer);
	}

	void GUIRenderContext::initialize()
	{
		//Vec4 - vertex position + type
		//Vec4 - two sets of texture coords
		//Vec4 - color
		//Vec4 - Position/Size of object
		//Vec4 - state data
		VertexFormat::Attribute attributes[5] =
		{
			VertexFormat::Attribute(VertexFormat::Position, 4),
			VertexFormat::Attribute(VertexFormat::TexCoord0, 4),
			VertexFormat::Attribute(VertexFormat::Color, 4),
			VertexFormat::Attribute(VertexFormat::Position, 4),
			VertexFormat::Attribute(VertexFormat::Position, 4),
		};

		VertexFormat vertexFormat(5, attributes);
		_renderer = new ImmediateRenderer(vertexFormat, Mesh::Triangles, true);

		Shader shader;
		shader.createProgram(Shader::Vertex, "resources/arcana/shaders/gui/gui_vert.glsl");
		shader.createProgram(Shader::Fragment, "resources/arcana/shaders/gui/gui_frag.glsl");

		_material = new Material("guiMaterial");
		Technique* materialTechnique = new Technique(shader);
		_material->addTechnique(materialTechnique);

		Matrix4f orthographicProjection = Matrix4f::createOrthographicOffCenter(0.0, 1920.0, 1080.0, 0.0, -1.0, 1.0);
		materialTechnique->getPass(0)->getUniform("u_OrthographicProjection").setValue(orthographicProjection);
	}

	void GUIRenderContext::start()
	{
		_renderer->start();
	}

	void GUIRenderContext::finish()
	{
		_renderer->finish();
	}

	void GUIRenderContext::beginPath()
	{
		_rectangles.clear();
	}

	void GUIRenderContext::draw(ObjectRenderer& renderer)
	{
		Mesh* mesh = _renderer->draw();

		MeshRenderContext context;

		context.mesh = mesh;
		context.material = _material;
		context.renderProperties.rendererStage = "GraphicalUserInterfaceStage";
		context.renderProperties.renderState.setCullEnabled(false);
		context.renderProperties.renderState.setCullFaceSide(RenderState::Back);
		context.renderProperties.renderState.setDepthTestEnabled(false);
		context.renderProperties.renderState.setBlendEnabled(true);
		context.renderProperties.renderState.setBlendSrc(RenderState::SrcAlpha);
		context.renderProperties.renderState.setBlendDst(RenderState::OneMinusSrcAlpha);

		renderer.addMesh(context);
	}

	void GUIRenderContext::drawRoundedRect(float x, float y, float w, float h, float r)
	{
		Rectangle rect;

		rect.v0.position = Vector4f(x, y, 0.0, 0.0);
		rect.v0.texCoords = Vector4f(0.0, 0.0, 0.0, 0.0);
		rect.v0.color = Vector4f::zero();
		rect.v0.positionSize = Vector4f(x, y, w, h);
		rect.v0.stateData = Vector4f(r, -1.0, 0.0, 0.0);

		rect.v1.position = Vector4f(x + w, y, 0.0, 0.0);
		rect.v1.texCoords = Vector4f(0.0, 0.0, 0.0, 0.0);
		rect.v1.color = Vector4f::zero();
		rect.v1.positionSize = Vector4f(x, y, w, h);
		rect.v1.stateData = Vector4f(r, -1.0, 0.0, 0.0);

		rect.v2.position = Vector4f(x + w, y + h, 0.0, 0.0);
		rect.v2.texCoords = Vector4f(0.0, 0.0, 0.0, 0.0);
		rect.v2.color = Vector4f::zero();
		rect.v2.positionSize = Vector4f(x, y, w, h);
		rect.v2.stateData = Vector4f(r, -1.0, 0.0, 0.0);

		rect.v3.position = Vector4f(x, y + h, 0.0, 0.0);
		rect.v3.texCoords = Vector4f(0.0, 0.0, 0.0, 0.0);
		rect.v3.color = Vector4f::zero();
		rect.v3.positionSize = Vector4f(x, y, w, h);
		rect.v3.stateData = Vector4f(r, -1.0, 0.0, 0.0);

		_rectangles.push_back(rect);
	}

	void GUIRenderContext::setFillColor(Color color)
	{
		_fillColor = color;
	}

	void GUIRenderContext::fill()
	{
		Vector4f fillColor = _fillColor.asLinear().toVector4();

		for (uint32 i = 0; i < _rectangles.size(); i++)
		{
			_rectangles[i].v0.color = fillColor;
			_rectangles[i].v1.color = fillColor;
			_rectangles[i].v2.color = fillColor;
			_rectangles[i].v3.color = fillColor;

			_renderer->add<Vertex>(&_rectangles[i].v0, 4, __rectIndices, 6);
		}
	}

	void GUIRenderContext::setLinearGradient(float x, float y, float ex, float ey, Color color1, Color color2)
	{
		_gradient.extents.x = ex - x;
		_gradient.extents.y = ey - y;
		_gradient.extents.normalize();
		_gradient.innerColor = color1;
		_gradient.outerColor = color2;
	}

	void GUIRenderContext::fillGradient()
	{
		Vector4f color1 = _gradient.innerColor.asLinear().toVector4();
		Vector4f color2 = _gradient.outerColor.asLinear().toVector4();

		for (uint32 i = 0; i < _rectangles.size(); i++)
		{
			float mixValue = Vector2f::distance(_gradient.extents, Vector2f::normalize(Vector2f(_rectangles[i].v0.position.x, _rectangles[i].v0.position.y)));
			_rectangles[i].v0.color = Math::lerp(color1, color2, Math::clamp(mixValue, 0.0f, 1.0f));

			mixValue = Vector2f::distance(_gradient.extents, Vector2f::normalize(Vector2f(_rectangles[i].v1.position.x, _rectangles[i].v1.position.y)));
			_rectangles[i].v1.color = Math::lerp(color1, color2, Math::clamp(mixValue, 0.0f, 1.0f));

			mixValue = Vector2f::distance(_gradient.extents, Vector2f::normalize(Vector2f(_rectangles[i].v2.position.x, _rectangles[i].v2.position.y)));
			_rectangles[i].v2.color = Math::lerp(color1, color2, Math::clamp(mixValue, 0.0f, 1.0f));

			mixValue = Vector2f::distance(_gradient.extents, Vector2f::normalize(Vector2f(_rectangles[i].v3.position.x, _rectangles[i].v3.position.y)));
			_rectangles[i].v3.color = Math::lerp(color1, color2, Math::clamp(mixValue, 0.0f, 1.0f));

			_renderer->add<Vertex>(&_rectangles[i].v0, 4, __rectIndices, 6);
		}
	}

	void GUIRenderContext::setStrokeWidth(float width)
	{
		_strokeWidth = width;
	}

	void GUIRenderContext::setStrokeColor(Color color)
	{
		_strokeColor = color;
	}

	void GUIRenderContext::stroke()
	{
		Vector4f strokeColor = _strokeColor.asLinear().toVector4();

		for (uint32 i = 0; i < _rectangles.size(); i++)
		{
			_rectangles[i].v0.stateData.y = _strokeWidth;
			_rectangles[i].v1.stateData.y = _strokeWidth;
			_rectangles[i].v2.stateData.y = _strokeWidth;
			_rectangles[i].v3.stateData.y = _strokeWidth;

			_rectangles[i].v0.color = strokeColor;
			_rectangles[i].v1.color = strokeColor;
			_rectangles[i].v2.color = strokeColor;
			_rectangles[i].v3.color = strokeColor;

			_renderer->add<Vertex>(&_rectangles[i].v0, 4, __rectIndices, 6);
		}
	}
}
