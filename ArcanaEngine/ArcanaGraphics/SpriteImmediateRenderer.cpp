#include "SpriteImmediateRenderer.h"

namespace Arcana
{
	int32 SpriteImmediateRenderer::DefaultSize = 128;

	float SpriteImmediateRenderer::GrowFactor = 2.0f;

	SpriteImmediateRenderer::SpriteImmediateRenderer(uint32 initialCapacity, Texture* texture) : _renderer(nullptr), _texture(nullptr)
	{
		VertexFormat::Attribute attribs[3] =
		{
			VertexFormat::Attribute(VertexFormat::Position, 3),
			VertexFormat::Attribute(VertexFormat::TexCoord0, 2),
			VertexFormat::Attribute(VertexFormat::Color, 4)
		};
		VertexFormat vertexFormat(3, attribs);

		_renderer = new ImmediateRenderer(vertexFormat, Mesh::Triangles, true, initialCapacity > 0 ? initialCapacity : DefaultSize);

		setTexture(texture);
	}

	SpriteImmediateRenderer::SpriteImmediateRenderer(const SpriteImmediateRenderer& copy)
	{

	}

	SpriteImmediateRenderer::~SpriteImmediateRenderer()
	{
		AE_DELETE(_renderer);
		AE_RELEASE(_texture);
	}

	void SpriteImmediateRenderer::start()
	{
		_renderer->start();
	}

	bool SpriteImmediateRenderer::isStarted() const
	{
		return _renderer->isStarted();
	}

	void SpriteImmediateRenderer::draw(const Rectf& dst, const Rectf& src, const Color& color)
	{
		float u1 = _textureWidthRatio * src.getLeft();
		float v1 = 1.0f - _textureHeightRatio * src.getTop();
		float u2 = u1 + _textureWidthRatio * src.getSize().x;
		float v2 = v1 - _textureHeightRatio * src.getSize().y;

		draw(dst.getLeft(), dst.getTop(), dst.getSize().x, dst.getSize().y, u1, v1, u2, v2, color);
	}

	void SpriteImmediateRenderer::draw(const Vector3f& dst, const Rectf& src, const Vector2f& scale, const Color& color)
	{
		float u1 = _textureWidthRatio * src.getLeft();
		float v1 = 1.0f - _textureHeightRatio * src.getTop();
		float u2 = u1 + _textureWidthRatio * src.getSize().x;
		float v2 = v1 - _textureHeightRatio * src.getSize().y;

		draw(dst.x, dst.y, dst.z, scale.x, scale.y, u1, v1, u2, v2, color);
	}

	void SpriteImmediateRenderer::draw(const Vector3f& dst, const Rectf& src, const Vector2f& scale, const Color& color,
		const Vector2f& rotationPoint, float rotationAngle)
	{
		float u1 = _textureWidthRatio * src.getLeft();
		float v1 = 1.0f - _textureHeightRatio * src.getTop();
		float u2 = u1 + _textureWidthRatio * src.getSize().x;
		float v2 = v1 - _textureHeightRatio * src.getSize().y;

		draw(dst, scale.x, scale.y, u1, v1, u2, v2, color, rotationPoint, rotationAngle);
	}

	void SpriteImmediateRenderer::draw(const Vector3f& dst, float width, float height, float u1, float v1, float u2, float v2, const Color& color,
		const Vector2f& rotationPoint, float rotationAngle, bool positionIsCenter)
	{
		draw(dst.x, dst.y, dst.z, width, height, u1, v1, u2, v2, color, rotationPoint, rotationAngle, positionIsCenter);
	}

	void SpriteImmediateRenderer::draw(float x, float y, float z, float width, float height, float u1, float v1, float u2, float v2, const Color& color,
		const Vector2f& rotationPoint, float rotationAngle, bool positionIsCenter)
	{
		if (positionIsCenter)
		{
			x -= 0.5f * width;
			y -= 0.5f * height;
		}

		float x2 = x + width;
		float y2 = y + height;

		Vector2f upLeft(x, y);
		Vector2f upRight(x2, y);
		Vector2f downLeft(x, y2);
		Vector2f downRight(x2, y2);

		if (rotationAngle != 0)
		{
			Vector2f pivotPoint(rotationPoint);
			//pivotPoint.x *= width;
			//pivotPoint.y *= height;
			//pivotPoint.x += x;
			//pivotPoint.y += y;
			pivotPoint.x -= width * 0.5f;
			pivotPoint.y -= height * 0.5f;
			/*upLeft.rotate(pivotPoint, rotationAngle);
			upRight.rotate(pivotPoint, rotationAngle);
			downLeft.rotate(pivotPoint, rotationAngle);
			downRight.rotate(pivotPoint, rotationAngle);*/
		}
		
		Vector4f c = color.asLinear().toVector4();
	
		static SpriteVertex v[4];
		v[0] = SpriteVertex(upLeft.x, upLeft.y, z, u1, v1, c.x, c.y, c.z, c.w);
		v[1] = SpriteVertex(downLeft.x, downLeft.y, z, u1, v2, c.x, c.y, c.z, c.w);
		v[2] = SpriteVertex(upRight.x, upRight.y, z, u2, v1, c.x, c.y, c.z, c.w);
		v[3] = SpriteVertex(downRight.x, downRight.y, z, u2, v2, c.x, c.y, c.z, c.w);

		static uint16 indices[6] = { 0, 1, 2, 2, 3, 1 };

		_renderer->add(v, 4, indices, 6);
	}

	void SpriteImmediateRenderer::draw(const Vector3f& position, const Vector3f& right, const Vector3f& forward, float width, float height,
		float u1, float v1, float u2, float v2, const Color& color, const Vector2f& rotationPoint, float rotationAngle)
	{
		Vector3f tRight(right);
		tRight *= width * 0.5f;
		Vector3f tForward(forward);
		tForward *= height * 0.5f;

		Vector3f p0 = position;
		p0 -= tRight;
		p0 -= tForward;

		Vector3f p1 = position;
		p1 += tRight;
		p1 -= tForward;

		tForward = forward;
		tForward *= height;
		Vector3f p2 = p0;
		p2 += tForward;
		Vector3f p3 = p1;
		p3 += tForward;

		if (rotationAngle != 0)
		{
			Vector3f rp = p0;
			tRight = right;
			tRight *= width * rotationPoint.x;
			tForward *= rotationPoint.y;
			rp += tRight;
			rp += tForward;

			static Vector3f u = Vector3f::cross(right, forward);
			static Matrix4f rotation;
			rotation = Matrix4f::createRotation(u, rotationAngle);
			p0 -= rp;
			p0 = rotation * p0;
			p0 += rp;
			p1 -= rp;
			p1 = rotation * p1;
			p1 += rp;
			p2 -= rp;
			p2 = rotation * p2;
			p2 += rp;
			p3 -= rp;
			p3 = rotation * p3;
			p3 += rp;
		}

		Vector4f c = color.asLinear().toVector4();

		static SpriteVertex v[4];
		v[0] = SpriteVertex(p0.x, p0.y, p0.z, u1, v1, c.x, c.y, c.z, c.w);
		v[1] = SpriteVertex(p1.x, p1.y, p1.z, u2, v1, c.x, c.y, c.z, c.w);
		v[2] = SpriteVertex(p2.x, p2.y, p2.z, u1, v2, c.x, c.y, c.z, c.w);
		v[3] = SpriteVertex(p3.x, p3.y, p3.z, u2, v2, c.x, c.y, c.z, c.w);

		static const uint16 indices[6] = { 0, 1, 2, 2, 3, 1 };
		_renderer->add(v, 4, const_cast<uint16*>(indices), 6);
	}

	void SpriteImmediateRenderer::draw(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const Color& color)
	{
		draw(x, y, 0, width, height, u1, v1, u2, v2, color);
	}

	void SpriteImmediateRenderer::draw(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const Color& color, const Rectf& clip)
	{
		draw(x, y, 0, width, height, u1, v1, u2, v2, color, clip);
	}

	void SpriteImmediateRenderer::draw(float x, float y, float z, float width, float height, float u1, float v1, float u2, float v2, const Color& color, const Rectf& clip)
	{
		if (clipSprite(clip, x, y, width, height, u1, v1, u2, v2))
		{
			draw(x, y, z, width, height, u1, v1, u2, v2, color);
		}
	}

	void SpriteImmediateRenderer::addSprite(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const Color& color, SpriteVertex* vertices)
	{
		if (vertices)
		{
			Vector4f c = color.asLinear().toVector4();

			const float x2 = x + width;
			const float y2 = y + height;
			vertices[0] = SpriteVertex(x, y, 0, u1, v1, c.x, c.y, c.z, c.w);
			vertices[1] = SpriteVertex(x, y2, 0, u1, v2, c.x, c.y, c.z, c.w);
			vertices[2] = SpriteVertex(x2, y, 0, u2, v1, c.x, c.y, c.z, c.w);
			vertices[3] = SpriteVertex(x2, y2, 0, u2, v2, c.x, c.y, c.z, c.w);
		}
	}

	void SpriteImmediateRenderer::addSprite(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const Color& color, const Rectf& clip, SpriteVertex* vertices)
	{
		if (vertices)
		{
			if (clipSprite(clip, x, y, width, height, u1, v1, u2, v2))
			{
				Vector4f c = color.asLinear().toVector4();

				const float x2 = x + width;
				const float y2 = y + height;
				vertices[0] = SpriteVertex(x, y, 0, u1, v1, c.x, c.y, c.z, c.w);
				vertices[1] = SpriteVertex(x, y2, 0, u1, v2, c.x, c.y, c.z, c.w);
				vertices[2] = SpriteVertex(x2, y, 0, u2, v1, c.x, c.y, c.z, c.w);
				vertices[3] = SpriteVertex(x2, y2, 0, u2, v2, c.x, c.y, c.z, c.w);
			}
		}
	}

	void SpriteImmediateRenderer::draw(SpriteImmediateRenderer::SpriteVertex* vertices, unsigned int vertexCount, unsigned short* indices, unsigned int indexCount)
	{
		if (vertices && indices)
		{
			_renderer->add(vertices, vertexCount, indices, indexCount);
		}
	}

	void SpriteImmediateRenderer::draw(float x, float y, float z, float width, float height, float u1, float v1, float u2, float v2, const Color& color, bool positionIsCenter)
	{
		if (positionIsCenter)
		{
			x -= 0.5f * width;
			y -= 0.5f * height;
		}

		Vector4f c = color.asLinear().toVector4();

		const float x2 = x + width;
		const float y2 = y + height;
		static SpriteVertex v[4];
		v[0] = SpriteVertex(x, y, 0, u1, v1, c.x, c.y, c.z, c.w);
		v[1] = SpriteVertex(x, y2, 0, u1, v2, c.x, c.y, c.z, c.w);
		v[2] = SpriteVertex(x2, y, 0, u2, v1, c.x, c.y, c.z, c.w);
		v[3] = SpriteVertex(x2, y2, 0, u2, v2, c.x, c.y, c.z, c.w);

		static uint16 indices[6] = { 0, 1, 2, 2, 3, 1 };

		_renderer->add(v, 4, indices, 6);
	}

	void SpriteImmediateRenderer::finish()
	{
		_renderer->finish();
		_renderer->draw();
	}

	void SpriteImmediateRenderer::setMaterial(Material* material)
	{
		_renderer->setMaterial(material);
	}

	Material* SpriteImmediateRenderer::getMaterial() const
	{
		return _renderer->getMaterial();
	}

	void SpriteImmediateRenderer::setTexture(Texture* texture)
	{
		if (_texture == texture)
			return;

		AE_RELEASE(_texture);

		_texture = texture;
		
		if (_texture)
		{
			_texture->reference();

			_textureWidthRatio = 1.0f / (float)texture->getWidth();
			_textureHeightRatio = 1.0f / (float)texture->getHeight();
		}
	}

	Texture* SpriteImmediateRenderer::getTexture() const
	{
		return _texture;
	}

	bool SpriteImmediateRenderer::clipSprite(const Rectf& clip, float& x, float& y, float& width, float& height, float& u1, float& v1, float& u2, float& v2)
	{
		if (x + width < clip.getLeft() || x > clip.getLeft() + clip.getSize().x ||
			y + height < clip.getTop() || y > clip.getTop() + clip.getSize().y)
		{
			return false;
		}

		float uvWidth = u2 - u1;
		float uvHeight = v2 - v1;

		if (x < clip.getLeft())
		{
			const float percent = (clip.getLeft() - x) / width;
			const float dx = clip.getLeft() - x;
			const float du = uvWidth * percent;
			x = clip.getLeft();
			width -= dx;
			u1 += du;
			uvWidth -= du;
		}

		if (y < clip.getTop())
		{
			const float percent = (clip.getTop() - y) / height;
			const float dy = clip.getTop() - y;
			const float dv = uvHeight * percent;
			y = clip.getTop();
			height -= dy;
			v1 += dv;
			uvHeight -= dv;
		}

		const float clipX2 = clip.getLeft() + clip.getSize().x;
		float x2 = x + width;
		if (x2 > clipX2)
		{
			const float percent = (x2 - clipX2) / width;
			width = clipX2 - x;
			u2 -= uvWidth * percent;
		}

		const float clipY2 = clip.getTop() + clip.getSize().y;
		float y2 = y + height;
		if (y2 > clipY2)
		{
			const float percent = (y2 - clipY2) / height;
			height = clipY2 - y;
			v2 -= uvHeight * percent;
		}

		return true;
	}
}