#ifndef PANEL_H_
#define PANEL_H_

#include "GUIDefines.h"

#include "Widget.h"

namespace Arcana
{

	class ARCANA_GUI_API Panel : public Widget
	{
		//friend class Popup;
	public:

		Panel(Widget* parent = nullptr, const std::string& title = "Untitled");

		~Panel();

		const std::string& getTitle() const;

		void setTitle(const std::string& title);

		bool isModal() const;

		void setModal(bool modal);

		bool hasDropShadow() const;

		void setDropShadow(bool dropShadow);

		const Color& getBackgroundColor() const;

		void setBackgroundColor(const Color& backgroundColor);

		Widget* getButtonPanel();

		void center();

		void dispose();

		virtual void render(GUIRenderContext& renderContext) override;

		virtual bool mouseDragEvent(const Vector2i &p, const Vector2i &rel, Key button, ModifierKeysState modifiers) override;

		virtual bool mouseButtonEvent(const Vector2i &p, Key button, bool down, ModifierKeysState modifiers) override;

		virtual bool scrollEvent(const Vector2i &p, float delta, MouseEvent::Wheel wheel) override;

		virtual Vector2i preferredSize(GUIRenderContext& renderContext) const override;

		virtual void performLayout(GUIRenderContext& renderContext) override;

	protected:

		virtual void refreshRelativePlacement();

	public:

		std::string _title;
		Widget* _buttonPanel;
		bool _modal;
		bool _drag;
		bool _dropShadow;
		Color _backgroundColor;
	};

}
#endif // !PANEL_H_