#include "Slider.h"

namespace Arcana
{
	Slider::Slider(Widget* parent) : Widget(parent), 
		_value(0.0f), _range(0.0f, 1.0f), _highlightedRange(0.0f, 0.0f), _highlightColor(255, 80, 80, 70)
	{

	}

	ValueUpdateCallback& Slider::getValueUpdateCallback()
	{
		return _valueUpdateCallback;
	}

	FinalCallback& Slider::getFinalCallback()
	{
		return _finalCallback;
	}

	const Vector2f& Slider::getRange() const
	{
		return _range;
	}

	void Slider::setRange(const Vector2f& range)
	{
		_range = range;
	}

	const Vector2f& Slider::getHighlightedRange() const
	{
		return _highlightedRange;
	}

	void Slider::setHighlightedRange(const Vector2f& range)
	{
		_highlightedRange = range;
	}

	const Color& Slider::getHighlightColor() const
	{
		return _highlightColor;
	}

	void Slider::setHighlightColor(const Color& color)
	{
		_highlightColor = color;
	}

	bool Slider::isVertical() const
	{
		return _vertical;
	}

	void Slider::setVertical(bool vertical)
	{
		_vertical = vertical;
	}

	Vector2i Slider::preferredSize(GUIRenderContext& renderContext) const
	{
		return Vector2i(200, 60);
	}

	bool Slider::mouseDragEvent(const Vector2i& p, const Vector2i& rel, Key button, ModifierKeysState modifiers)
	{
		if (!isEnabled())
			return false;

		float value = 0.0f;
		
		_valueUpdateCallback.executeIfBound(value);

		return true;
	}

	bool Slider::mouseButtonEvent(const Vector2i& p, Key button, bool down, ModifierKeysState modifiers)
	{
		if (!isEnabled())
			return false;
	
		float value = 0.0f;
		
		_valueUpdateCallback.executeIfBound(value);

		if (!down)
			_finalCallback.executeIfBound(value);

		return true;
	}

	void Slider::render(GUIRenderContext& renderContext)
	{
		renderContext.reset();

		Vector2f knobPos(getAbsolutePosition().x + (_value - _range.x) /
			(_range.y - _range.x) * getSize().x,
			getAbsolutePosition().y + getSize().y * 0.2f);

		renderContext.setPrimaryColor(Color(255, 0, 0, isEnabled() ? 128 : 210));
		renderContext.drawRoundedRect(getAbsolutePosition().x, getAbsolutePosition().y, getSize().x, getSize().y, 0.05);

		//test
		renderContext.setPrimaryColor(Color(150, 150, 150, isEnabled() ? 255 : 100));
		renderContext.drawRect(knobPos.x, knobPos.y, getSize().y * 0.6f, getSize().y * 0.6f);

		/*NVGpaint knobShadow =
			nvgRadialGradient(ctx, knobPos.x(), knobPos.y(), kr - kshadow,
				kr + kshadow, Color(0, 64), mTheme->mTransparent);

		nvgBeginPath(ctx);
		nvgRect(ctx, knobPos.x() - kr - 5, knobPos.y() - kr - 5, kr * 2 + 10,
			kr * 2 + 10 + kshadow);
		nvgCircle(ctx, knobPos.x(), knobPos.y(), kr);
		nvgPathWinding(ctx, NVG_HOLE);
		nvgFillPaint(ctx, knobShadow);
		nvgFill(ctx);

		NVGpaint knob = nvgLinearGradient(ctx,
			mPos.x(), center.y() - kr, mPos.x(), center.y() + kr,
			mTheme->mBorderLight, mTheme->mBorderMedium);
		NVGpaint knobReverse = nvgLinearGradient(ctx,
			mPos.x(), center.y() - kr, mPos.x(), center.y() + kr,
			mTheme->mBorderMedium,
			mTheme->mBorderLight);

		nvgBeginPath(ctx);
		nvgCircle(ctx, knobPos.x(), knobPos.y(), kr);
		nvgStrokeColor(ctx, mTheme->mBorderDark);
		nvgFillPaint(ctx, knob);
		nvgStroke(ctx);
		nvgFill(ctx);
		nvgBeginPath(ctx);
		nvgCircle(ctx, knobPos.x(), knobPos.y(), kr / 2);
		nvgFillColor(ctx, Color(150, mEnabled ? 255 : 100));
		nvgStrokePaint(ctx, knobReverse);
		nvgStroke(ctx);
		nvgFill(ctx);*/
	}
}