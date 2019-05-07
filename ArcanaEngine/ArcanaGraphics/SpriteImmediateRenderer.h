#ifndef SPRITE_IMMEDIATE_RENDERER_H_
#define SPRITE_IMMEDIATE_RENDERER_H_

#include "GraphicsDefines.h"

#include "ImmediateRenderer.h"
#include "Rect.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Color.h"
#include "Texture.h"
#include "Matrix4.h"
#include <string>

namespace Arcana
{

	class ARCANA_GRAPHICS_API SpriteImmediateRenderer
	{
	public:

		static int32 DefaultSize;

		static float GrowFactor;

	public:

		SpriteImmediateRenderer(uint32 initialCapacity = 0, Texture* texture = nullptr);

		SpriteImmediateRenderer(const SpriteImmediateRenderer& copy);

		virtual ~SpriteImmediateRenderer();

		void start();

		bool isStarted() const;

		void draw(const Rectf& dst, const Rectf& src, const Color& color = Color(255, 255, 255, 255));

		void draw(const Vector3f& dst, const Rectf& src, const Vector2f& scale, const Color& color = Color(255, 255, 255, 255));

		void draw(const Vector3f& dst, const Rectf& src, const Vector2f& scale, const Color& color,
			const Vector2f& rotationPoint, float rotationAngle);

		void draw(const Vector3f& dst, float width, float height, float u1, float v1, float u2, float v2, const Color& color,
			const Vector2f& rotationPoint, float rotationAngle, bool positionIsCenter = false);

		void draw(float x, float y, float z, float width, float height, float u1, float v1, float u2, float v2, const Color& color,
			const Vector2f& rotationPoint, float rotationAngle, bool positionIsCenter = false);

		void draw(const Vector3f& position, const Vector3f& right, const Vector3f& forward, float width, float height,
			float u1, float v1, float u2, float v2, const Color& color, const Vector2f& rotationPoint, float rotationAngle);

		void draw(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const Color& color);

		void draw(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const Color& color, const Rectf& clip);

		void draw(float x, float y, float z, float width, float height, float u1, float v1, float u2, float v2, const Color& color, const Rectf& clip);

		void draw(float x, float y, float z, float width, float height, float u1, float v1, float u2, float v2, const Color& color, bool positionIsCenter = false);


		struct SpriteVertex
		{
			Vector3f position;
			Vector2f texCoords;
			Vector4f color;

			SpriteVertex() {};

			SpriteVertex(float vx, float vy, float vz, float vu, float vv, float vr, float vg, float vb, float va)
				: position(vx, vy, vz), texCoords(vu, vv), color(vr, vg, vb, va)
			{

			};
		};

		void setTexture(Texture* texture);

		Texture* getTexture() const;

		void draw(SpriteVertex* vertices, unsigned int vertexCount, unsigned short* indices, unsigned int indexCount);

		void finish();

		void setMaterial(Material* material);

		Material* getMaterial() const;

	private:

		void addSprite(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const Color& color, SpriteVertex* vertices);

		void addSprite(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const Color& color, const Rectf& clip, SpriteVertex* vertices);

		bool clipSprite(const Rectf& clip, float& x, float& y, float& width, float& height, float& u1, float& v1, float& u2, float& v2);

		ImmediateRenderer* _renderer;
		Texture* _texture;
		float _textureWidthRatio;
		float _textureHeightRatio;
	};

}

#endif