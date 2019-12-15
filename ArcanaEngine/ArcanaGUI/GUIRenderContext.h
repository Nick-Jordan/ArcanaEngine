#ifndef GUI_RENDER_CONTEXT_H_
#define GUI_RENDER_CONTEXT_H_

#include "GUIDefines.h"

#include "ImmediateRenderer.h"
#include "ObjectRenderer.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Rect.h"
#include "GUIIcon.h"

namespace Arcana
{

	class ARCANA_GUI_API GUIRenderContext
	{
	public:

		struct Rectangle
		{
			Vector4f positionSize;
			Vector4f texCoords;
			Vector4f color;
			Vector4f secondaryColor;
			Vector3f attributes;
		};

		struct Icon
		{
			GUIIcon* icon;
			Rectf area;
			Color color;
			int32 z;
			float radius;
		};

		struct LinearGradient
		{
			bool enabled;
			bool horizontal;
			bool bilinear;
			float offset;
			float size;
		};

		struct BoxGradient
		{
			bool enabled;
			bool rounded;
			Vector2f offset;
			Vector2f size;
		};

		struct RadialGradient
		{
			bool enabled;
			Vector2f offset;
			Vector2f size;
		};

		GUIRenderContext();

		~GUIRenderContext();

		void initialize(const Vector2i& size);

		void drawIcon(GUIIcon* icon, Rectf area, Color color = Color(255, 255, 255, 255), float radius = 0.0);

		void render();

		void setPrimaryColor(const Color& color);

		void setSecondaryColor(const Color& color);

		void setLinearGradient(float offset, float size, bool horizontal, bool bilinear);

		void setLinearGradient(bool enabled);

		void setBoxGradient(float offsetX, float offsetY, float sizeX, float sizeY, bool rounded);

		void setBoxGradient(bool enabled);

		void setRadialGradient(float offsetX, float offsetY, float sizeX, float sizeY);

		void setRadialGradient(bool enabled);

		void drawRect(float x, float y, float w, float h);

		void drawRoundedRect(float x, float y, float w, float h, float radius);

		float getCurrentZ();

		void reset();

		//bool clipRect(const Rectf& clip, float& x, float& y, float& width, float& height, float& u1, float& v1, float& u2, float& v2);

	private:

		Material* _material;
		Shader* _shaderIcon;
		int32 _currentZ;

		std::vector<Rectangle> _rectangles;
		std::vector<Icon> _icons;

		Color _primaryColor;
		Color _secondaryColor;
		LinearGradient _linearGradient;
		BoxGradient _boxGradient;
		RadialGradient _radialGradient;

		VertexFormat _instanceFormat;

		Mesh* _mesh;
		Texture* _linearGradientTexture;
		Texture* _bilinearGradientTexture;
		Texture* _boxGradientTexture;
		Texture* _roundedBoxGradientTexture;
		Texture* _radialGradientTexture;
	};

}

#endif // !GUI_RENDER_CONTEXT_H_
