#ifndef GUI_RENDER_CONTEXT_H_
#define GUI_RENDER_CONTEXT_H_

#include "GUIDefines.h"

#include "ImmediateRenderer.h"
#include "ObjectRenderer.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Rect.h"

namespace Arcana
{

	class ARCANA_GUI_API GUIRenderContext
	{
	public:

		struct Vertex
		{
			Vector4f position;
			Vector4f texCoords;
			Vector4f color;
			Vector4f positionSize;
			Vector4f stateData;
		};

		struct Rectangle
		{
			Vertex v0;
			Vertex v1;
			Vertex v2;
			Vertex v3;
		};

		struct Gradient
		{
			Vector2f extents;
			Color innerColor;
			Color outerColor;
		};

		GUIRenderContext();

		~GUIRenderContext();

		void initialize();

		void start();

		void finish();

		void beginPath();

		void draw(ObjectRenderer& renderer);

		void drawRoundedRect(float x, float y, float w, float h, float r);

		void setFillColor(Color color);

		void fill();

		void setLinearGradient(float x, float y, float ex, float ey, Color color1, Color color2);

		void fillGradient();

		void setStrokeWidth(float width);

		void setStrokeColor(Color color);

		void stroke();

		//bool clipRect(const Rectf& clip, float& x, float& y, float& width, float& height, float& u1, float& v1, float& u2, float& v2);

	private:

		ImmediateRenderer* _renderer;
		Material* _material;

	private:

		std::vector<Rectangle> _rectangles;
		Color _fillColor;
		Color _strokeColor;
		Gradient _gradient;
		float _strokeWidth;

	private:

		static uint16 __rectIndices[6];
	};

}

#endif // !GUI_RENDER_CONTEXT_H_
