#ifndef SLIDER_H_
#define SLIDER_H_

#include "GUIDefines.h"

#include "Widget.h"
#include "Callback.h"

namespace Arcana
{
	REGISTER_CALLBACK(ValueUpdateCallback, float);
	REGISTER_CALLBACK(FinalCallback, float);

	class ARCANA_GUI_API Slider : public Widget
	{
	public:

		Slider(Widget* parent = nullptr);

		ValueUpdateCallback& getValueUpdateCallback();

		FinalCallback& getFinalCallback();

		const Vector2f& getRange() const;

		void setRange(const Vector2f& range);

		const Color& getHighlightColor() const;

		void setHighlightColor(const Color& color);

		const Color& getColor() const;

		void setColor(const Color& color);

		bool isVertical() const;

		void setVertical(bool vertical);

		bool isHighlightReversed() const;

		void setHighlightReversed(bool reverseHighlight);

		float getValue() const;

		void setValue(float value);

		float getSizeFactor() const;

		void setSizeFactor(float factor);

		const Vector2f& getKnobSize() const;

		void setKnobSize(const Vector2f& size);

		bool hasRectangleKnob() const;

		void setRectangleKnob(bool rectangle);

		virtual Vector2i preferredSize(GUIRenderContext& renderContext) const override;

		virtual bool mouseDragEvent(const Vector2i& p, const Vector2i& rel, Key button, ModifierKeysState modifiers) override;

		virtual bool mouseButtonEvent(const Vector2i& p, Key button, bool down, ModifierKeysState modifiers) override;

		virtual void render(GUIRenderContext& renderContext) override;
		
	private:

		float _value;

		Vector2f _range;
		Color _highlightColor;
		Color _color;
		ValueUpdateCallback _valueUpdateCallback;
		FinalCallback _finalCallback;
		float _sizeFactor;
		Vector2f _knobSize;
		bool _rectangleKnob;
		bool _vertical;
		bool _reverseHighlight;
		bool _updating;
	};
}

#endif // !SLIDER_H_