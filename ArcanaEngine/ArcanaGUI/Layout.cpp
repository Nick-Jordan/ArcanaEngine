#include "Layout.h"

#include "GUIWindow.h"
#include "Label.h"
#include "Panel.h"

namespace Arcana
{
	BoxLayout::BoxLayout(Orientation orientation, Alignment alignment,
		int32 hmargin, int32 vmargin, int32 spacing) 
		: _orientation(orientation), _alignment(alignment), 
		_hmargin(hmargin), _vmargin(vmargin), _spacing(spacing)
	{

	}

	Layout::Orientation BoxLayout::getOrientation() const
	{
		return _orientation;
	}

	void BoxLayout::setOrientation(Orientation orientation)
	{
		_orientation = orientation;
	}

	Layout::Alignment BoxLayout::getAlignment() const
	{
		return _alignment;
	}

	void BoxLayout::setAlignment(Alignment alignment)
	{
		_alignment = alignment;
	}

	int32 BoxLayout::getHorizontalMargin() const
	{
		return _hmargin;
	}

	void BoxLayout::setHorizontalMargin(int32 margin)
	{
		_hmargin = margin;
	}

	int32 BoxLayout::getVerticalMargin() const
	{
		return _vmargin;
	}

	void BoxLayout::setVerticalMargin(int32 margin)
	{
		_vmargin = margin;
	}

	int32 BoxLayout::getSpacing() const
	{
		return _spacing;
	}

	void BoxLayout::setSpacing(int32 spacing)
	{
		_spacing = spacing;
	}

	Vector2i BoxLayout::preferredSize(GUIRenderContext& renderContext, const Widget *widget) const
	{
		int32 margin[] = { _hmargin , _vmargin };
		int32 size[] = { 2 * _hmargin, 2 * _vmargin };

		int32 yOffset = 0;

		const Panel* window = dynamic_cast<const Panel*>(widget);  ///PANEL

		if (window && !window->getTitle().empty())
		{
			if (_orientation == Orientation::Vertical)
			{
				size[1] += widget->getTheme()->PanelHeaderHeight - _vmargin / 2;
			}
			else
			{
				yOffset = widget->getTheme()->PanelHeaderHeight;
			}
		}

		bool first = true;
		int32 axis1 = (int32)_orientation;
		int32 axis2 = ((int32)_orientation + 1) % 2;

		for (auto w : widget->getChildren())
		{
			if (!w->isVisible(false))
				continue;

			if (first)
			{
				first = false;
			}
			else
			{
				size[axis1] += _spacing;
			}

			Vector2i ps = w->preferredSize(renderContext);
			Vector2i fs = w->getFixedSize();
			int32 targetSize[] = {
				fs.x ? fs.x : ps.x,
				fs.y ? fs.y : ps.y
			};

			size[axis1] += targetSize[axis1];
			size[axis2] = Math::max(size[axis2], targetSize[axis2] + 2 * margin[axis2]);
			first = false;
		}
		return Vector2i(size[0], size[1] + yOffset);
	}

	void BoxLayout::performLayout(GUIRenderContext& renderContext, Widget *widget) const
	{
		int32 margin[] = { _hmargin , _vmargin };

		Vector2i fs_w = widget->getFixedSize();
		int32 containerSize[] = {
			fs_w.x ? fs_w.x : widget->getWidth(),
			fs_w.y ? fs_w.y : widget->getHeight()
		};

		int32 axis1 = (int32)_orientation;
		int32 axis2 = ((int32)_orientation + 1) % 2;
		int32 position = margin[axis1];//_margin;
		int32 yOffset = 0;

		const Panel* window = dynamic_cast<const Panel*>(widget);//PANELL??
		if (window && !window->getTitle().empty())
		{
			if (_orientation == Orientation::Vertical)
			{
				position += widget->getTheme()->PanelHeaderHeight - _vmargin / 2;
			}
			else 
			{
				yOffset = widget->getTheme()->PanelHeaderHeight;
				containerSize[1] -= yOffset;
			}
		}

		bool first = true;
		for (auto w : widget->getChildren())
		{
			if (!w->isVisible(false))
				continue;

			if (first)
			{
				first = false;
			}
			else
			{
				position += _spacing;
			}

			Vector2i ps = w->preferredSize(renderContext);
			int32 fs[] = { w->getFixedSize().x, w->getFixedSize().y };
			int32 targetSize[] = {
				fs[0] ? fs[0] : ps.x,
				fs[1] ? fs[1] : ps.y
			};
			int32 pos[] = { 0, yOffset };

			pos[axis1] = position;

			switch (_alignment)
			{
			case Alignment::Minimum:
				pos[axis2] += margin[axis2];
				break;
			case Alignment::Middle:
				pos[axis2] += (containerSize[axis2] - targetSize[axis2]) / 2;
				break;
			case Alignment::Maximum:
				pos[axis2] += containerSize[axis2] - targetSize[axis2] - margin[axis2] * 2;
				break;
			case Alignment::Fill:
				pos[axis2] += margin[axis2];
				targetSize[axis2] = fs[axis2] ? fs[axis2] : (containerSize[axis2] - margin[axis2] * 2);
				break;
			}

			w->setPosition(Vector2i(pos[0], pos[1]));
			w->setSize(Vector2i(targetSize[0], targetSize[1]));
			w->performLayout(renderContext);
			position += targetSize[axis1];
		}
	}



	GroupLayout::GroupLayout(int32 margin, int32 spacing, int32 groupSpacing, int32 groupIndent)
		: _margin(margin), _spacing(spacing), _groupSpacing(groupSpacing), _groupIndent(groupIndent)
	{

	}

	int32 GroupLayout::getMargin() const
	{
		return _margin;
	}

	void GroupLayout::setMargin(int32 margin)
	{
		_margin = margin;
	}

	int32 GroupLayout::getSpacing() const
	{
		return _spacing;
	}

	void GroupLayout::setSpacing(int32 spacing)
	{
		_spacing = spacing;
	}

	int32 GroupLayout::setGroupIndent() const
	{
		return _groupIndent;
	}

	void GroupLayout::setGroupIndent(int32 groupIndent)
	{
		_groupIndent = groupIndent;
	}

	int32 GroupLayout::setGroupSpacing()
	{
		return _groupSpacing;
	}

	void GroupLayout::setGroupSpacing(int32 groupSpacing)
	{
		_groupSpacing = groupSpacing;
	}

	Vector2i GroupLayout::preferredSize(GUIRenderContext& renderContext, const Widget *widget) const
	{
		int32 height = _margin;
		int32 width = 2 * _margin;

		const Panel* window = dynamic_cast<const Panel*>(widget);
		if (window && !window->getTitle().empty())
		{
			height += widget->getTheme()->PanelHeaderHeight - _margin / 2;
		}

		bool first = true, indent = false;
		for (auto c : widget->getChildren())
		{
			if (!c->isVisible(false))
				continue;

			const Label* label = dynamic_cast<const Label*>(c);
			if (!first)
			{
				height += (label == nullptr) ? _spacing : _groupSpacing;
			}

			first = false;

			Vector2i ps = c->preferredSize(renderContext);
			Vector2i fs = c->getFixedSize();
			Vector2i targetSize(
				fs.x ? fs.x : ps.x,
				fs.y ? fs.y : ps.y
			);

			bool indentCur = indent && label == nullptr;
			height += targetSize.y;
			width = Math::max(width, targetSize.x + 2 * _margin + (indentCur ? _groupIndent : 0));

			if (label)
				indent = !label->getCaption().empty();
		}

		height += _margin;

		return Vector2i(width, height);
	}

	void GroupLayout::performLayout(GUIRenderContext& renderContext, Widget *widget) const
	{
		int32 height = _margin;
		int32 availableWidth = (widget->getFixedWidth() ? widget->getFixedWidth() : widget->getWidth()) - 2 * _margin;

		const Panel* window = dynamic_cast<const Panel*>(widget);
		if (window && !window->getTitle().empty())
		{
			height += widget->getTheme()->PanelHeaderHeight - _margin / 2;
		}

		bool first = true, indent = false;
		for (auto c : widget->getChildren())
		{
			if (!c->isVisible(false))
				continue;

			const Label* label = dynamic_cast<const Label*>(c);

			if (!first)
			{
				height += (label == nullptr) ? _spacing : _groupSpacing;
			}

			first = false;

			bool indentCur = indent && label == nullptr;
			Vector2i ps = Vector2i(availableWidth - (indentCur ? _groupIndent : 0),
				c->preferredSize(renderContext).y);
			Vector2i fs = c->getFixedSize();

			Vector2i targetSize(
				fs.x ? fs.x : ps.x,
				fs.y ? fs.y : ps.y
			);

			c->setPosition(Vector2i(_margin + (indentCur ? _groupIndent : 0), height));
			c->setSize(targetSize);
			c->performLayout(renderContext);

			height += targetSize.y;

			if (label)
				indent = !label->getCaption().empty();
		}
	}
}