#ifndef LAYOUT_H_
#define LAYOUT_H_

#include "GUIDefines.h"

#include "Object.h"
#include "GUIRenderContext.h"

#include <unordered_map>

namespace Arcana
{
	class ARCANA_GUI_API Widget;

	class ARCANA_GUI_API Layout : public Object
	{
	public:

		enum class Alignment : uint8
		{
			Minimum = 0,
			Middle,
			Maximum,
			Fill
		};

		enum class Orientation
		{
			Horizontal = 0,
			Vertical
		};

		virtual void performLayout(GUIRenderContext& renderContext, Widget *widget) const = 0;

		virtual Vector2i preferredSize(GUIRenderContext& renderContext, const Widget *widget) const = 0;

	protected:

		virtual ~Layout() { }
	};

	class ARCANA_GUI_API BoxLayout : public Layout
	{
	public:

		BoxLayout(Orientation orientation, Alignment alignment = Alignment::Middle,
			int32 hmargin = 0, int32 vmargin = 0, int32 spacing = 0);

		Orientation getOrientation() const;

		void setOrientation(Orientation orientation);

		Alignment getAlignment() const;

		void setAlignment(Alignment alignment);

		int32 getHorizontalMargin() const;

		void setHorizontalMargin(int32 margin);

		int32 getVerticalMargin() const;

		void setVerticalMargin(int32 margin);

		int32 getSpacing() const;

		void setSpacing(int32 spacing);

		virtual Vector2i preferredSize(GUIRenderContext& renderContext, const Widget *widget) const override;

		virtual void performLayout(GUIRenderContext& renderContext, Widget *widget) const override;

	private:

		Orientation _orientation;
		Alignment _alignment;
		int32 _hmargin;
		int32 _vmargin;
		int32 _spacing;
	};

	class ARCANA_GUI_API GroupLayout : public Layout
	{
	public:

		GroupLayout(int32 margin = 15, int32 spacing = 6, int32 groupSpacing = 14, int32 groupIndent = 20);

		int32 getMargin() const;

		void setMargin(int32 margin);

		int32 getSpacing() const;

		void setSpacing(int32 spacing);

		int32 setGroupIndent() const;

		void setGroupIndent(int32 groupIndent);

		int32 setGroupSpacing();

		void setGroupSpacing(int32 groupSpacing);

		virtual Vector2i preferredSize(GUIRenderContext& renderContext, const Widget *widget) const override;

		virtual void performLayout(GUIRenderContext& renderContext, Widget *widget) const override;

	private:

		int32 _margin;
		int32 _spacing;
		int32 _groupSpacing;
		int32 _groupIndent;
	};
}

#endif // !LAYOUT_H_