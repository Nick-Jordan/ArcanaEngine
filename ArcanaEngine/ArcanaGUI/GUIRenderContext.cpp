#include "GUIRenderContext.h"

namespace Arcana
{
	GUIRenderContext::GUIRenderContext() : _backgroundImage(nullptr)
	{
	}


	GUIRenderContext::~GUIRenderContext()
	{
	}

	void GUIRenderContext::initialize(const Vector2i& size)
	{
		_size = size;

		VertexFormat::Attribute attributes[1] =
		{
			VertexFormat::Attribute(VertexFormat::Position, 2)
		};

		VertexFormat format(1, attributes);
		_mesh = new Mesh(format, Mesh::TriangleStrip);

		float vertices[] =
		{
			0.0, 0.0,
			0.0, 1.0,
			1.0, 0.0,
			1.0, 1.0
		};
		_mesh->setVertexBuffer(format, 4)->setVertexData(&vertices);

		VertexFormat::Attribute instanceAttribs[6] =
		{
			VertexFormat::Attribute(VertexFormat::Position, 4),
			VertexFormat::Attribute(VertexFormat::TexCoord0, 4),
			VertexFormat::Attribute(VertexFormat::Color, 4),
			VertexFormat::Attribute(VertexFormat::Color, 4),
			VertexFormat::Attribute(VertexFormat::Color, 3),
		};

		_instanceFormat = VertexFormat(6, instanceAttribs, 1);
		_mesh->setInstanceBuffer(_instanceFormat, 0, true);

		Shader shader;
		shader.createProgram(Shader::Vertex, "resources/arcana/shaders/gui/gui_vert.glsl");
		shader.createProgram(Shader::Fragment, "resources/arcana/shaders/gui/gui_frag.glsl");

		_material = new Material("guiMaterial");
		Technique* materialTechnique = new Technique(shader);
		_material->addTechnique(materialTechnique);

		Matrix4f orthographicProjection = Matrix4f::createOrthographicOffCenter(0, size.x, size.y, -19, -1.0, 1.0);
		materialTechnique->getPass(0)->getUniform("u_OrthographicProjection").setValue(orthographicProjection);

		_shaderIcon = new Shader();
		_shaderIcon->createProgram(Shader::Vertex, "resources/arcana/shaders/gui/icon_vert.glsl");
		_shaderIcon->createProgram(Shader::Fragment, "resources/arcana/shaders/gui/icon_frag.glsl");

		_shaderIcon->getUniform("u_OrthographicProjection").setValue(orthographicProjection);

		//fix (don't start at 2)
		_currentZ = 2;//maybe find better way to handle z
		_primaryColor = Color(1, 1, 1, 1);
		_secondaryColor = Color(1, 1, 1, 1);
		_linearGradient.enabled = false;
		_boxGradient.enabled = false;


		Image<uint8> image;
		image.init("resources/arcana/textures/gui/linear_gradient.png");
		Texture::Parameters params;
		params.setMinFilter(TextureFilter::Linear);
		params.setMagFilter(TextureFilter::Linear);
		_linearGradientTexture = Texture::create1D(Texture::RGBA, image.getWidth(), Texture::RGBA8, Texture::UnsignedByte, image.getPixelsPtr(), params, true);

		image.init("resources/arcana/textures/gui/bilinear_gradient.png");
		_bilinearGradientTexture = Texture::create1D(Texture::RGBA, image.getWidth(), Texture::RGBA8, Texture::UnsignedByte, image.getPixelsPtr(), params, true);

		image.init("resources/arcana/textures/gui/box_gradient.png");
		_boxGradientTexture = Texture::create2D(Texture::RGBA, image.getWidth(), image.getHeight(), Texture::RGBA8, Texture::UnsignedByte, image.getPixelsPtr(), params, true);

		image.init("resources/arcana/textures/gui/rounded_box_gradient2.png");
		_roundedBoxGradientTexture = Texture::create2D(Texture::RGBA, image.getWidth(), image.getHeight(), Texture::RGBA8, Texture::UnsignedByte, image.getPixelsPtr(), params, true);

		image.init("resources/arcana/textures/gui/radial_gradient.png");
		_radialGradientTexture = Texture::create2D(Texture::RGBA, image.getWidth(), image.getHeight(), Texture::RGBA8, Texture::UnsignedByte, image.getPixelsPtr(), params, true);
	}

	void GUIRenderContext::setPrimaryColor(const Color& color)
	{
		_primaryColor = color;
	}

	void GUIRenderContext::setSecondaryColor(const Color& color)
	{
		_secondaryColor = color;
	}

	void GUIRenderContext::setLinearGradient(float offset, float size, bool horizontal, bool bilinear)
	{
		_linearGradient.enabled = true;
		_linearGradient.offset = offset;
		_linearGradient.size = size;
		_linearGradient.horizontal = horizontal;
		_linearGradient.bilinear = bilinear;

		_boxGradient.enabled = false;
		_radialGradient.enabled = false;
	}

	void GUIRenderContext::setLinearGradient(bool enabled)
	{
		_linearGradient.enabled = enabled;

		if (enabled)
		{
			_boxGradient.enabled = false;
			_radialGradient.enabled = false;
		}
	}

	void GUIRenderContext::setBoxGradient(float offsetX, float offsetY, float sizeX, float sizeY, bool rounded)
	{
		_boxGradient.enabled = true;
		_boxGradient.offset = Vector2f(offsetX, offsetY);
		_boxGradient.size = Vector2f(sizeX, sizeY);
		_boxGradient.rounded = rounded;

		_linearGradient.enabled = false;
		_radialGradient.enabled = false;
	}

	void GUIRenderContext::setBoxGradient(bool enabled)
	{
		_boxGradient.enabled = enabled;

		if (enabled)
		{
			_linearGradient.enabled = false;
			_radialGradient.enabled = false;
		}
	}

	void GUIRenderContext::setRadialGradient(float offsetX, float offsetY, float sizeX, float sizeY)
	{
		_radialGradient.enabled = true;
		_radialGradient.offset = Vector2f(offsetX, offsetY);
		_radialGradient.size = Vector2f(sizeX, sizeY);

		_linearGradient.enabled = false;
		_boxGradient.enabled = false;
	}

	void GUIRenderContext::setRadialGradient(bool enabled)
	{
		_radialGradient.enabled = enabled;

		if (enabled)
		{
			_linearGradient.enabled = false;
			_boxGradient.enabled = false;
		}
	}

	void GUIRenderContext::drawRect(float x, float y, float w, float h)
	{
		return drawRoundedRect(x, y, w, h, 0.0);
	}

	void GUIRenderContext::drawRoundedRect(float x, float y, float w, float h, float r)
	{
		Rectangle rect;

		rect.positionSize = Vector4f(x, y, w, h);
		rect.color = _primaryColor.asLinear().toVector4();
		rect.secondaryColor = _secondaryColor.asLinear().toVector4();
		rect.attributes = Vector3f(r, getCurrentZ(), 0);

		if (_linearGradient.enabled)
		{
			if (_linearGradient.horizontal)
			{
				rect.texCoords.x = _linearGradient.offset;
				rect.texCoords.z = _linearGradient.size;
			}
			else
			{
				rect.texCoords.y = _linearGradient.offset;
				rect.texCoords.w = _linearGradient.size;
			}

			if (_linearGradient.bilinear)
			{
				if (_linearGradient.horizontal)
					rect.attributes.z = 4;
				else
					rect.attributes.z = 3;
			}
			else
			{
				if (_linearGradient.horizontal)
					rect.attributes.z = 2;
				else
					rect.attributes.z = 1;
			}
		}
		else if (_boxGradient.enabled)
		{
			rect.texCoords.x = _boxGradient.offset.x;
			rect.texCoords.y = _boxGradient.offset.y;
			rect.texCoords.z = _boxGradient.size.x;
			rect.texCoords.w = _boxGradient.size.y;

			if (_boxGradient.rounded)
			{
				rect.attributes.z = 6;
			}
			else
			{
				rect.attributes.z = 5;
			}
		}
		else if (_radialGradient.enabled)
		{
			rect.texCoords.x = _radialGradient.offset.x;
			rect.texCoords.y = _radialGradient.offset.y;
			rect.texCoords.z = _radialGradient.size.x;
			rect.texCoords.w = _radialGradient.size.y;

			rect.attributes.z = 7;
		}
		else
		{
			rect.texCoords = Vector4f::zero();
		}

		_rectangles.push_back(rect);
	}

	void GUIRenderContext::drawIcon(GUIIcon* icon, Rectf area, Color color, float radius)
	{
		if (!icon || !icon->getTexture())
			return;

		Icon i;
		i.icon = icon;
		i.area = area;
		i.color = color;
		i.z = _currentZ++;
		i.radius = radius;

		_icons.push_back(i);
	}

	void GUIRenderContext::render()
	{
		if (_backgroundImage)
		{
			Icon i;
			i.icon = _backgroundImage;
			i.area = Rectf(0.0f, 0.0f, (float)_size.x, (float)_size.y);
			i.color = Color(255, 255, 255, 255);
			i.z = 1;
			i.radius = 0.0f;
			_icons.push_back(i);
		}

		if (!_icons.empty())
		{
			_shaderIcon->bind();
			for (auto i = _icons.begin(); i != _icons.end(); i++)
			{
				Icon& icon = *i;

				_shaderIcon->getUniform("u_Area").setValue(Vector4f(icon.area.getLeft(), icon.area.getTop(), icon.area.getSize().x, icon.area.getSize().y));
				_shaderIcon->getUniform("u_ZPosition").setValue((float)icon.z / 1000.0f);
				_shaderIcon->getUniform("u_IconTexture").setValue(icon.icon->getTexture()->bind());
				_shaderIcon->getUniform("u_Color").setValue(icon.color.asLinear().toVector4());
				_shaderIcon->getUniform("u_Radius").setValue(icon.radius);

				ObjectRenderer::drawQuad();

			}
			_shaderIcon->unbind();
		}

		if (!_rectangles.empty())
		{
			_mesh->getInstanceBuffer()->bind();
			glBufferData(GL_ARRAY_BUFFER, _instanceFormat.getVertexSize() * _rectangles.size(), &_rectangles[0].positionSize.x, GL_DYNAMIC_DRAW);
			_mesh->getInstanceBuffer()->unbind();

			_mesh->getVertexBuffer()->bind();
			
			Technique* technique = _material->getCurrentTechnique();

			if (technique)
			{
				for (uint32 i = 0; i < technique->getPassCount(); i++)
				{
					Shader* pass = technique->getPass(i);
					if (pass)
					{
						pass->bind();

						int32 unit = _linearGradientTexture->bind();
						pass->getUniform("u_LinearGradient").setValue(unit);
						unit = _bilinearGradientTexture->bind();
						pass->getUniform("u_BilinearGradient").setValue(unit);
						unit = _boxGradientTexture->bind();
						pass->getUniform("u_BoxGradient").setValue(unit);
						unit = _roundedBoxGradientTexture->bind();
						pass->getUniform("u_RoundedBoxGradient").setValue(unit);
						unit = _radialGradientTexture->bind();
						pass->getUniform("u_RadialGradient").setValue(unit);

						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
						_mesh->getInstanceBuffer()->bind();
						glDrawArraysInstanced(_mesh->getPrimitive(), 0, _mesh->getNumVertices(), _rectangles.size());
						_mesh->getInstanceBuffer()->unbind();

						pass->unbind();
					}
				}
			}

			_mesh->getVertexBuffer()->unbind();
		}

		_currentZ = 2;
		_rectangles.clear();
		_icons.clear();
	}

	float GUIRenderContext::getCurrentZ()
	{
		float c = (float)_currentZ / 1000.0f;
		_currentZ++;
		return c;
	}

	void GUIRenderContext::reset()
	{
		_primaryColor = Color(1, 1, 1, 1);
		_secondaryColor = Color(1, 1, 1, 1);
		_linearGradient.enabled = false;
		_boxGradient.enabled = false;
		_radialGradient.enabled = false;
	}
}
