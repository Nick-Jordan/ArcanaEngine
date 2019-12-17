#include "Slider.h"

namespace Arcana
{
	Slider::Slider(Widget* parent) : Widget(parent),
		_value(0.0f), _range(0.0f, 1.0f), _highlightColor(255, 80, 80, 255), _updating(false),
		_reverseHighlight(false), _vertical(false), _color(255, 255, 255), _rectangleKnob(false),
		_sizeFactor(0.5f), _knobSize(1.0f, 1.0f)
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

	float Slider::getSizeFactor() const
	{
		return _sizeFactor;
	}

	void Slider::setSizeFactor(float factor)
	{
		_sizeFactor = factor;
	}

	const Vector2f& Slider::getKnobSize() const
	{
		return _knobSize;
	}

	void Slider::setKnobSize(const Vector2f& size)
	{
		_knobSize = size;
	}

	bool Slider::hasRectangleKnob() const
	{
		return _rectangleKnob;
	}

	void Slider::setRectangleKnob(bool rectangle)
	{
		_rectangleKnob = rectangle;
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
			float add = 0.0f;// _rectangleKnob ? (1.0f - _knobSize.y) * getSize().x * 0.5f : 0.0f;

			knobPos = Vector2f(getAbsolutePosition().x + (1.0f - _knobSize.x) * getSize().x * 0.5f, getAbsolutePosition().y + (_value - _range.x) /
				(_range.y - _range.x) * (getSize().y - getSize().x) + (1.0f - _knobSize.y) * getSize().x * 0.5f);
			knobSize = getSize().x;

			renderContext.setPrimaryColor(_color);
			renderContext.setSecondaryColor(secondary);
			renderContext.setLinearGradient(0, 1, true, false);
			renderContext.drawRoundedRect(getAbsolutePosition().x + getSize().x * (1.0f - _sizeFactor) * 0.5f,
				getAbsolutePosition().y + getSize().x * _sizeFactor * 0.5f + (1.0f - _knobSize.y) * getSize().x * 0.5f,
				getSize().x * _sizeFactor,
				getSize().y - getSize().x, 0.5);

			renderContext.setPrimaryColor(_highlightColor);
			renderContext.setSecondaryColor(_highlightColor.withAlpha((uint8)((float)_highlightColor.A * 0.6f)));
			if (_reverseHighlight)
			{
				renderContext.drawRoundedRect(getAbsolutePosition().x + getSize().x * _sizeFactor / 2.0f, 
					getAbsolutePosition().y + (_value - _range.x) / (_range.y - _range.x) * (getSize().y - getSize().x) + getSize().x * _sizeFactor,
					getSize().x * _sizeFactor, 
					(getSize().y - getSize().x) - (_value - _range.x) / (_range.y - _range.x) * (getSize().y - getSize().x), 0.5);
			}
			else
			{
				renderContext.drawRoundedRect(getAbsolutePosition().x + getSize().x * _sizeFactor / 2.0f,
					getAbsolutePosition().y + getSize().x * _sizeFactor,
					getSize().x * _sizeFactor,
					(_value - _range.x) / (_range.y - _range.x) * (getSize().y - getSize().x), 0.5);
			}
		}
		else
		{
			float add = 0.0f;// _rectangleKnob ? (1.0f - _knobSize.x) * getSize().y * 0.5f : 0.0f;

			knobPos = Vector2f(getAbsolutePosition().x + (_value - _range.x) /
				(_range.y - _range.x) * (getSize().x - getSize().y) + (1.0f - _knobSize.x) * getSize().y * 0.5f,
				getAbsolutePosition().y + (1.0f - _knobSize.y) * getSize().y * 0.5f);
			knobSize = getSize().y;

			renderContext.setPrimaryColor(_color);
			renderContext.setSecondaryColor(_color.withAlpha((uint8)((float)_highlightColor.A * 0.8f)));
			renderContext.setLinearGradient(0, 1, false, false);
			renderContext.drawRoundedRect(getAbsolutePosition().x + getSize().y * _sizeFactor * 0.5f + (1.0f - _knobSize.x) * getSize().y * 0.5f,
				getAbsolutePosition().y + getSize().y * (1.0f - _sizeFactor) * 0.5f,
				getSize().x - getSize().y,
				getSize().y * _sizeFactor, 0.5);

			renderContext.setPrimaryColor(_highlightColor);
			renderContext.setSecondaryColor(_highlightColor.withAlpha((uint8)((float)_highlightColor.A * 0.6f)));
			if (_reverseHighlight)
			{
				renderContext.drawRoundedRect(getAbsolutePosition().x + (_value - _range.x) / (_range.y - _range.x) * (getSize().x - getSize().y) + getSize().y * _sizeFactor * 0.5f + (1.0f - _knobSize.x) * getSize().y * 0.5f,
					getAbsolutePosition().y + getSize().y * (1.0f - _sizeFactor) * 0.5f,
					(getSize().x - getSize().y) - (_value - _range.x) / (_range.y - _range.x) * (getSize().x - getSize().y),
					getSize().y * _sizeFactor, 0.5);
			}
			else
			{
				renderContext.drawRoundedRect(getAbsolutePosition().x + getSize().y * _sizeFactor * 0.5f + (1.0f - _knobSize.x) * getSize().y * 0.5f,
					getAbsolutePosition().y + getSize().y * (1.0f - _sizeFactor) * 0.5f,
					(_value - _range.x) / (_range.y - _range.x) * (getSize().x - getSize().y),
					getSize().y * _sizeFactor, 0.5);
			}
		}

		renderContext.setPrimaryColor(_color);
		renderContext.setSecondaryColor(secondary);
		float knobRadius;
		if (_rectangleKnob)
		{
			knobRadius = 0.167f;
			renderContext.setBoxGradient(0, 0, 1, 1, true);
		}
		else
		{
			knobRadius = 1.0f;
			renderContext.setRadialGradient(0, 0, 1, 1);
		}
		renderContext.drawRoundedRect(knobPos.x, knobPos.y, knobSize * _knobSize.x, knobSize * _knobSize.y, knobRadius);
	}
}