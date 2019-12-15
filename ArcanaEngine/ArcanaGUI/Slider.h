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

		const Vector2f& getHighlightedRange() const;

		void setHighlightedRange(const Vector2f& range);

		const Color& getHighlightColor() const;

		void setHighlightColor(const Color& color);

		bool isVertical() const;

		void setVertical(bool vertical);

		virtual Vector2i preferredSize(GUIRenderContext& renderContext) const override;

		virtual bool mouseDragEvent(const Vector2i& p, const Vector2i& rel, Key button, ModifierKeysState modifiers) override;

		virtual bool mouseButtonEvent(const Vector2i& p, Key button, bool down, ModifierKeysState modifiers) override;

		virtual void render(GUIRenderContext& renderContext) override;
		
	private:

		float _value;

		Vector2f _range;
		Vector2f _highlightedRange;
		Color _highlightColor;
		ValueUpdateCallback _valueUpdateCallback;
		FinalCallback _finalCallback;
		bool _vertical;
	};
}

#endif // !SLIDER_H_