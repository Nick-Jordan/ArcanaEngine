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

	void GUIRenderContext::initialize(const Vector2i& size)
	{
		//Vec4 - vertex position + type
		//Vec4 - two sets of texture coords
		//Vec4 - color
		//Vec4 - secondary color
		//Vec4 - Position/Size of object
		//Vec4 - state data
		//Vec4 - clip
		VertexFormat::Attribute attributes[7] =
		{
			VertexFormat::Attribute(VertexFormat::Position, 4),
			VertexFormat::Attribute(VertexFormat::TexCoord0, 4),
			VertexFormat::Attribute(VertexFormat::Color, 4),
			VertexFormat::Attribute(VertexFormat::Color, 4),
			VertexFormat::Attribute(VertexFormat::Position, 4),
			VertexFormat::Attribute(VertexFormat::Position, 4),
			VertexFormat::Attribute(VertexFormat::Position, 4),
		};

		VertexFormat vertexFormat(7, attributes);
		_renderer = new ImmediateRenderer(vertexFormat, Mesh::Triangles, true);

		Shader shader;
		shader.createProgram(Shader::Vertex, "resources/arcana/shaders/gui/gui_vert.glsl");
		shader.createProgram(Shader::Fragment, "resources/arcana/shaders/gui/gui_frag.glsl");

		_material = new Material("guiMaterial");
		Technique* materialTechnique = new Technique(shader);
		_material->addTechnique(materialTechnique);

		_renderer->setMaterial(_material);

		LOGF(Info, CoreEngine, "Size: %d, %d", size.x, size.y);

		Matrix4f orthographicProjection = Matrix4f::createOrthographicOffCenter(0, size.x, size.y, -19, -1.0, 1.0);
		materialTechnique->getPass(0)->getUniform("u_OrthographicProjection").setValue(orthographicProjection);

		_renderState.setCullEnabled(false);
		_renderState.setCullFaceSide(RenderState::Back);
		_renderState.setDepthTestEnabled(false);
		_renderState.setBlendEnabled(true);
		_renderState.setBlendSrc(RenderState::SrcAlpha);
		_renderState.setBlendDst(RenderState::OneMinusSrcAlpha);

		_shaderIcon = new Shader();
		_shaderIcon->createProgram(Shader::Vertex, "resources/arcana/shaders/gui/icon_vert.glsl");
		_shaderIcon->createProgram(Shader::Fragment, "resources/arcana/shaders/gui/icon_frag.glsl");

		_shaderIcon->getUniform("u_OrthographicProjection").setValue(orthographicProjection);
	}

	void GUIRenderContext::start()
	{
		_renderer->start();
		_rectangles.clear();
		_currentZ = 0;
		removeClip();
	}

	void GUIRenderContext::finish()
	{
		_renderer->finish();
	}

	void GUIRenderContext::beginPath()
	{
		_rectangles.clear();
	}

	void GUIRenderContext::draw()
	{
		_renderState.bind();

		_renderer->draw();

		_renderState.unbind();
	}

	void GUIRenderContext::drawRoundedRect(float x, float y, float w, float h, float r)
	{
		Rectangle rect;

		rect.v0.position = Vector4f(x, y, (float)_currentZ/1000.0f, 0.0);
		rect.v0.texCoords = Vector4f(0.0, 0.0, 0.0, 0.0);
		rect.v0.color = Vector4f::zero();
		rect.v0.positionSize = Vector4f(x, y, w, h);
		rect.v0.stateData = Vector4f(r, -1.0, 0.0, 0.0);
		rect.v0.clip = -Vector4f::one();

		rect.v1.position = Vector4f(x + w, y, (float)_currentZ / 1000.0f, 0.0);
		rect.v1.texCoords = Vector4f(0.0, 0.0, 0.0, 0.0);
		rect.v1.color = Vector4f::zero();
		rect.v1.positionSize = Vector4f(x, y, w, h);
		rect.v1.stateData = Vector4f(r, -1.0, 0.0, 0.0);
		rect.v1.clip = -Vector4f::one();
		
		rect.v2.position = Vector4f(x + w, y + h, (float)_currentZ / 1000.0f, 0.0);
		rect.v2.texCoords = Vector4f(0.0, 0.0, 0.0, 0.0);
		rect.v2.color = Vector4f::zero();
		rect.v2.positionSize = Vector4f(x, y, w, h);
		rect.v2.stateData = Vector4f(r, -1.0, 0.0, 0.0);
		rect.v2.clip = -Vector4f::one();

		rect.v3.position = Vector4f(x, y + h, (float)_currentZ / 1000.0f, 0.0);
		rect.v3.texCoords = Vector4f(0.0, 0.0, 0.0, 0.0);
		rect.v3.color = Vector4f::zero();
		rect.v3.positionSize = Vector4f(x, y, w, h);
		rect.v3.stateData = Vector4f(r, -1.0, 0.0, 0.0);
		rect.v3.clip = -Vector4f::one();

		_rectangles.push_back(rect);

		_currentZ++;
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

			if (_hasClipRect)
			{
				_rectangles[i].v0.clip = _clipRect;
				_rectangles[i].v1.clip = _clipRect;
				_rectangles[i].v2.clip = _clipRect;
				_rectangles[i].v3.clip = _clipRect;
			}

			_renderer->add<Vertex>(&_rectangles[i].v0, 4, __rectIndices, 6);
		}
	}

	void GUIRenderContext::setLinearGradient(float x, float y, float ex, float ey, Color color1, Color color2)
	{
		_linearGradient.extents.x = ex - x;
		_linearGradient.extents.y = ey - y;
	//	_linearGradient.extents.normalize();
		_linearGradient.innerColor = color1;
		_linearGradient.outerColor = color2;
	}

	void GUIRenderContext::setBoxGradient(float x, float y, float ex, float ey, float factor, Color innerColor, Color outerColor)
	{
		_boxGradient.box = Vector4f(x, y, ex, ey);
		_boxGradient.factor = factor;
		_boxGradient.innerColor = innerColor;
		_boxGradient.outerColor = outerColor;
	}

	void GUIRenderContext::fillLinearGradient()
	{
		Vector4f color1 = _linearGradient.innerColor.asLinear().toVector4();
		Vector4f color2 = _linearGradient.outerColor.asLinear().toVector4();

		float mag = _linearGradient.extents.magnitudeSq();

		//LOGF(Info, CoreEngine, "extents: %f, %f", _linearGradient.extents.x, _linearGradient.extents.y);

		if (mag == 0.0f)
			return;

		for (uint32 i = 0; i < _rectangles.size(); i++)
		{
			/*float mixValue = Vector2f::distanceSq(_linearGradient.point0, Vector2f(_rectangles[i].v0.position.x, _rectangles[i].v0.position.y))
			LOGF(Info, CoreEngine, "mixvalue: %f", mixValue);
			_rectangles[i].v0.color = Math::lerp(color1, color2, Math::clamp(mixValue, 0.0f, 1.0f));*/

			/*mixValue = Vector2f::distance(_linearGradient.extents, (Vector2f(_rectangles[i].v1.position.x, _rectangles[i].v1.position.y) - Vector2f(_rectangles[i].v1.positionSize.x, _rectangles[i].v1.positionSize.y)));
			LOGF(Info, CoreEngine, "mixvalue: %f", mixValue);
			_rectangles[i].v1.color = Math::lerp(color1, color2, Math::clamp(mixValue, 0.0f, 1.0f));

			mixValue = Vector2f::distance(_linearGradient.extents, (Vector2f(_rectangles[i].v2.position.x, _rectangles[i].v2.position.y) - Vector2f(_rectangles[i].v2.positionSize.x, _rectangles[i].v2.positionSize.y)));
			LOGF(Info, CoreEngine, "mixvalue: %f", mixValue);
			_rectangles[i].v2.color = Math::lerp(color1, color2, Math::clamp(mixValue, 0.0f, 1.0f));

			mixValue = Vector2f::distance(_linearGradient.extents, (Vector2f(_rectangles[i].v3.position.x, _rectangles[i].v3.position.y) - Vector2f(_rectangles[i].v3.positionSize.x, _rectangles[i].v3.positionSize.y)));
			LOGF(Info, CoreEngine, "mixvalue: %f", mixValue);
			_rectangles[i].v3.color = Math::lerp(color1, color2, Math::clamp(mixValue, 0.0f, 1.0f));*/

			Vector2f vec2 = Vector2f(_rectangles[i].v0.position.x, _rectangles[i].v0.position.y) - Vector2f(_rectangles[i].v0.positionSize.x, _rectangles[i].v0.positionSize.y);
			Vector2f v = Vector2f::proj(vec2, _linearGradient.extents);
			float mixValue = v.magnitudeSq() / mag;
			//LOGF(Info, CoreEngine, "mixvalue: %f, %f, %f, %f, %f", mixValue, v.x, v.y, vec2.x, vec2.y);
			_rectangles[i].v0.color = Math::lerp(color1, color2, Math::clamp(mixValue, 0.0f, 1.0f));

			vec2 = Vector2f(_rectangles[i].v1.position.x, _rectangles[i].v1.position.y) - Vector2f(_rectangles[i].v1.positionSize.x, _rectangles[i].v1.positionSize.y);
			v = Vector2f::proj(vec2, _linearGradient.extents);
			mixValue = v.magnitudeSq() / mag;
			//LOGF(Info, CoreEngine, "mixvalue: %f, %f, %f, %f, %f", mixValue, v.x, v.y, vec2.x, vec2.y);
			_rectangles[i].v1.color = Math::lerp(color1, color2, Math::clamp(mixValue, 0.0f, 1.0f));

			vec2 = Vector2f(_rectangles[i].v2.position.x, _rectangles[i].v2.position.y) - Vector2f(_rectangles[i].v2.positionSize.x, _rectangles[i].v2.positionSize.y);
			v = Vector2f::proj(vec2, _linearGradient.extents);
			mixValue = v.magnitudeSq() / mag;
			//LOGF(Info, CoreEngine, "mixvalue: %f, %f, %f, %f, %f", mixValue, v.x, v.y, vec2.x, vec2.y);
			_rectangles[i].v2.color = Math::lerp(color1, color2, Math::clamp(mixValue, 0.0f, 1.0f));

			vec2 = Vector2f(_rectangles[i].v3.position.x, _rectangles[i].v3.position.y) - Vector2f(_rectangles[i].v3.positionSize.x, _rectangles[i].v3.positionSize.y);
			v = Vector2f::proj(vec2, _linearGradient.extents);
			mixValue = v.magnitudeSq() / mag;
			//LOGF(Info, CoreEngine, "mixvalue: %f, %f, %f, %f, %f", mixValue, v.x, v.y, vec2.x, vec2.y);
			_rectangles[i].v3.color = Math::lerp(color1, color2, Math::clamp(mixValue, 0.0f, 1.0f));

			if (_hasClipRect)
			{
				_rectangles[i].v0.clip = _clipRect;
				_rectangles[i].v1.clip = _clipRect;
				_rectangles[i].v2.clip = _clipRect;
				_rectangles[i].v3.clip = _clipRect;
			}

			_renderer->add<Vertex>(&_rectangles[i].v0, 4, __rectIndices, 6);
		}
	}

	void GUIRenderContext::fillBoxGradient()
	{
		Vector4f color1 = _boxGradient.innerColor.asLinear().toVector4();
		Vector4f color2 = _boxGradient.outerColor.asLinear().toVector4();

		Vector4f box = _boxGradient.box;

		for (uint32 i = 0; i < _rectangles.size(); i++)
		{
			_rectangles[i].v0.texCoords = box;
			_rectangles[i].v0.color = color1;
			_rectangles[i].v0.secondaryColor = color2;
			_rectangles[i].v0.stateData.z = _boxGradient.factor;

			_rectangles[i].v1.texCoords = box;
			_rectangles[i].v1.color = color1;
			_rectangles[i].v1.secondaryColor = color2;
			_rectangles[i].v1.stateData.z = _boxGradient.factor;

			_rectangles[i].v2.texCoords = box;
			_rectangles[i].v2.color = color1;
			_rectangles[i].v2.secondaryColor = color2;
			_rectangles[i].v2.stateData.z = _boxGradient.factor;

			_rectangles[i].v3.texCoords = box;
			_rectangles[i].v3.color = color1;
			_rectangles[i].v3.secondaryColor = color2;
			_rectangles[i].v3.stateData.z = _boxGradient.factor;

			if (_hasClipRect)
			{
				_rectangles[i].v0.clip = _clipRect;
				_rectangles[i].v1.clip = _clipRect;
				_rectangles[i].v2.clip = _clipRect;
				_rectangles[i].v3.clip = _clipRect;
			}

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

			if (_hasClipRect)
			{
				_rectangles[i].v0.clip = _clipRect;
				_rectangles[i].v1.clip = _clipRect;
				_rectangles[i].v2.clip = _clipRect;
				_rectangles[i].v3.clip = _clipRect;
			}

			_renderer->add<Vertex>(&_rectangles[i].v0, 4, __rectIndices, 6);
		}
	}

	void GUIRenderContext::drawIcon(GUIIcon* icon, Rectf area, Color color)
	{
		if (!icon || !icon->getTexture())
			return;

		_renderState.bind();

		_shaderIcon->bind();
		
		_shaderIcon->getUniform("u_Area").setValue(Vector4f(area.getLeft(), area.getTop(), area.getSize().x, area.getSize().y));
		_shaderIcon->getUniform("u_ZPosition").setValue((float)_currentZ/1000.0f);
		_shaderIcon->getUniform("u_IconTexture").setValue(icon->getTexture()->bind());
		_shaderIcon->getUniform("u_Color").setValue(color.asLinear().toVector4());

		ObjectRenderer::drawQuad();

		_shaderIcon->unbind();

		_renderState.unbind();

		_currentZ++;
	}

	void GUIRenderContext::setClipRect(float x, float y, float w, float h)
	{
		_hasClipRect = true;
		_clipRect = Vector4f(x, y, w, h);
	}

	void GUIRenderContext::removeClip()
	{
		_hasClipRect = false;
	}
}
