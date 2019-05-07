#ifndef CHECK_BOX_H_
#define CHECK_BOX_H_

#include "GUIDefines.h"

#include "Widget.h"
#include "Callback.h"

namespace Arcana
{
	REGISTER_CALLBACK(OnCheckedCallback, bool);

	class ARCANA_GUI_API CheckBox : public Widget
	{
	public:

		CheckBox(Widget* parent = nullptr, const std::string& caption = "Untitled");

		~CheckBox();

		const std::string& getCaption() const;

		void setCaption(const std::string& caption);

		const bool& isChecked() const;

		void setChecked(const bool& checked);

		const bool& isPushed() const;

		void setPushed(const bool& pushed);

		OnCheckedCallback& getOnCheckedCallback();



		virtual Vector2i preferredSize(GUIRenderContext& renderContext) const override;

		virtual bool mouseButtonEvent(const Vector2i &p, Key button, bool down, ModifierKeysState modifiers) override;

		virtual void render(GUIRenderContext& renderContext) override;

	private:

		std::string _caption;

		bool _pushed;

		bool _checked;

		OnCheckedCallback _onCheckedCallback;
	};
}

#endif // !CHECK_BOX_H_