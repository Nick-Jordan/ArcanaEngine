#include "Slider.h"

namespace Arcana
{
	Slider::Slider(Widget* parent) : Widget(parent), 
		_value(0.0f), _range(0.0f, 1.0f), _highlightColor(255, 80, 80, 255), _updating(false), _reverseHighlight(false), _vertical(false), _color(255, 255, 255)
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

	const Color& Slider::getHighlightColor() const
	{
		return _highlightColor;
	}

	void Slider::setHighlightColor(const Color& color)
	{
		_highlightColor = color;
	}

	const Color& Slider::getColor() const
	{
		return _color;
	}

	void Slider::setColor(const Color& color)
	{
		_color = color;
	}

	bool Slider::isVertical() const
	{
		return _vertical;
	}

	void Slider::setVertical(bool vertical)
	{
		_vertical = vertical;
	}

	bool Slider::isHighlightReversed() const
	{
		return _reverseHighlight;
	}

	void Slider::setHighlightReversed(bool reverseHighlight)
	{
		_reverseHighlight = reverseHighlight;
	}

	float Slider::getValue() const
	{
		return _value;
	}

	void Slider::setValue(float value)
	{
		_value = value;
	}

	Vector2i Slider::preferredSize(GUIRenderContext& renderContext) const
	{
		if (_vertical)
		{
			return Vector2i(30, 200);
		}
		return Vector2i(200, 30);
	}

	bool Slider::mouseDragEvent(const Vector2i& p, const Vector2i& rel, Key button, ModifierKeysState modifiers)
	{
		if (!isEnabled())
			return false;

		if (_updating)
		{
			float v0 = (float)(_vertical ? p.y : p.x);
			float v1 = (float)(_vertical ? getAbsolutePosition().y : getAbsolutePosition().x);
			float v2 = (float)(_vertical ? getSize().y : getSize().x);

			_value = (v0 - v1) / v2;
			_value = _value * (_range.y - _range.x) + _range.x;
			_value = Math::min(Math::max(_value, _range.x), _range.y);

			_valueUpdateCallback.executeIfBound(_value);
		}

		return true;
	}

	bool Slider::mouseButtonEvent(const Vector2i& p, Key button, bool down, ModifierKeysState modifiers)
	{
		if (!isEnabled())
			return false;

		if (down)
			_updating = true;

		if (_updating)
		{
			float v0 = (float)(_vertical ? p.y : p.x);
			float v1 = (float)(_vertical ? getAbsolutePosition().y : getAbsolutePosition().x);
			float v2 = (float)(_vertical ? getSize().y : getSize().x);

			_value = (v0 - v1) / v2;
			_value = _value * (_range.y - _range.x) + _range.x;
			_value = Math::min(Math::max(_value, _range.x), _range.y);

			_valueUpdateCallback.executeIfBound(_value);
		}

		if (!down)
		{
			_finalCallback.executeIfBound(_value);
			_updating = false;
		}

		return true;
	}

	void Slider::render(GUIRenderContext& renderContext)
	{
		renderContext.reset();

		Vector2f knobPos;
		float knobSize;

		Color secondary = (_color.asLinear() * 0.8f).quantize().withAlpha(_color.A);

		if (_vertical)
		{
			knobPos = Vector2f(getAbsolutePosition().x, getAbsolutePosition().y + (_value - _range.x) /
				(_range.y - _range.x) * (getSize().y - getSize().x));
			knobSize = getSize().x;

			renderContext.setPrimaryColor(_color);
			renderContext.setSecondaryColor(secondary);
			renderContext.setLinearGradient(0, 1, true, false);
			renderContext.drawRoundedRect(getAbsolutePosition().x + getSize().x / 4.0, getAbsolutePosition().y + getSize().x / 2, getSize().x / 2, getSize().y - getSize().x, 0.5);

			renderContext.setPrimaryColor(_highlightColor);
			renderContext.setSecondaryColor(_highlightColor.withAlpha((uint8)((float)_highlightColor.A * 0.6f)));
			if (_reverseHighlight)
			{
				renderContext.drawRoundedRect(getAbsolutePosition().x + (_value - _range.x) /
					(_range.y - _range.x) * (getSize().x - getSize().y) + getSize().y / 2, getAbsolutePosition().y + getSize().y / 4, (getSize().x - getSize().y) - (_value - _range.x) /
					(_range.y - _range.x) * (getSize().x - getSize().y), getSize().y / 2, 0.5);

				renderContext.drawRoundedRect(getAbsolutePosition().x + getSize().x / 4.0, getAbsolutePosition().y + (_value - _range.x) /
					(_range.y - _range.x) * (getSize().y - getSize().x) + getSize().x / 2, getSize().x / 2, (getSize().y - getSize().x) - (_value - _range.x) /
					(_range.y - _range.x) * (getSize().y - getSize().x), 0.5);
			}
			else
			{
				renderContext.drawRoundedRect(getAbsolutePosition().x + getSize().x / 4.0, getAbsolutePosition().y + getSize().x / 2, getSize().x / 2, 
					(_value - _range.x) / (_range.y - _range.x) * (getSize().y - getSize().x), 0.5);
			}
		}
		else
		{
			knobPos = Vector2f(getAbsolutePosition().x + (_value - _range.x) /
				(_range.y - _range.x) * (getSize().x - getSize().y),
				getAbsolutePosition().y);
			knobSize = getSize().y;

			renderContext.setPrimaryColor(_color);
			renderContext.setSecondaryColor(_color.withAlpha((uint8)((float)_highlightColor.A * 0.8f)));
			renderContext.setLinearGradient(0, 1, false, false);
			renderContext.drawRoundedRect(getAbsolutePosition().x + getSize().y / 2.0, getAbsolutePosition().y + getSize().y / 4, getSize().x - getSize().y, getSize().y / 2, 0.5);

			renderContext.setPrimaryColor(_highlightColor);
			renderContext.setSecondaryColor(_highlightColor.withAlpha((uint8)((float)_highlightColor.A * 0.6f)));
			if (_reverseHighlight)
			{
				renderContext.drawRoundedRect(getAbsolutePosition().x + (_value - _range.x) /
					(_range.y - _range.x) * (getSize().x - getSize().y) + getSize().y / 2, getAbsolutePosition().y + getSize().y / 4, (getSize().x - getSize().y) - (_value - _range.x) /
					(_range.y - _range.x) * (getSize().x - getSize().y), getSize().y / 2, 0.5);
			}
			else
			{
				renderContext.drawRoundedRect(getAbsolutePosition().x + getSize().y / 2.0, getAbsolutePosition().y + getSize().y / 4, (_value - _range.x) /
					(_range.y - _range.x) * (getSize().x - getSize().y), getSize().y / 2, 0.5);
			}
		}

		renderContext.setPrimaryColor(_color);
		renderContext.setSecondaryColor(secondary);
		renderContext.setRadialGradient(0, 0, 1, 1);
		renderContext.drawRoundedRect(knobPos.x, knobPos.y, knobSize, knobSize, 1.0);

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