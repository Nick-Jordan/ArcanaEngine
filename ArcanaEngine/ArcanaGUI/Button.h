#ifndef BUTTON_H_
#define BUTTON_H_

#include "GUIDefines.h"

#include "Widget.h"
#include "Callback.h"

#undef min
#undef max

namespace Arcana
{
	REGISTER_CALLBACK(OnPressedCallback);
	REGISTER_CALLBACK(OnChangedCallback, bool);

	class ARCANA_GUI_API Button : public Widget
	{
	public:

		enum Type
		{
			Normal = 1 << 0,
			Radio = 1 << 1,
			Toggle = 1 << 2,
			Popup = 1 << 3
		};


		enum class IconPosition
		{
			Left,
			LeftCentered,
			RightCentered,
			Right
		};


		Button(Widget* parent = nullptr, const std::string& caption = "Untitled", GUIIcon* icon = nullptr);

		~Button();

		const std::string& getCaption() const;

		void setCaption(const std::string& caption);

		const Color& getBackgroundColor() const;

		void setBackgroundColor(const Color& backgroundColor);

		const Color& getTextColor() const;

		void setTextColor(const Color& textColor);

		GUIIcon* getIcon() const;

		void setIcon(GUIIcon* icon);

		int32 getFlags() const;

		void setFlags(int32 buttonFlags);

		IconPosition getIconPosition() const;

		void setIconPosition(IconPosition iconPosition);

		void setSidebar(int32 sidebar);

		int32 getSidebar() const;

		void setBorder(float size);

		float getBorder() const;

		bool isPushed() const;

		void setPushed(bool pushed);

		OnPressedCallback& getOnPressedCallback();

		OnChangedCallback& getOnChangedCallback();

		Font::Justify getFontJustify() const;

		void setFontJustify(Font::Justify justify);

		void setButtonGroup(const std::vector<Button*>& buttonGroup);

		const std::vector<Button*>& getButtonGroup() const;

		virtual Vector2i preferredSize(GUIRenderContext& renderContext) const override;

		virtual bool mouseButtonEvent(const Vector2i &p, Key button, bool down, ModifierKeysState modifiers) override;

		virtual void render(GUIRenderContext& renderContext) override;

	private:

		std::string _caption;

		GUIIcon* _icon;

		IconPosition _iconPosition;

		bool _pushed;

		int32 _sidebarSize;

		float _borderSize;

		int32 _flags;

		Color _backgroundColor;

		Color _textColor;

		Font::Justify _fontJustify;

		OnPressedCallback _onPressedCallback;

		OnChangedCallback _onChangedCallback;

		std::vector<Button*> _buttonGroup;
	};
}

#endif // !BUTTON_H_