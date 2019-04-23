#ifndef WIDGET_H_
#define WIDGET_H_

#include "GUIDefines.h"

#include "Object.h"
#include "Vector2.h"
#include "GlobalObjectID.h"
#include "TypeTraits.h"
#include "Key.h"
#include "ModifierKeysState.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "GUIRenderContext.h"
#include "Theme.h"

namespace Arcana
{
	class ARCANA_GUI_API GUIWindow;

	class ARCANA_GUI_API Widget : public Object
	{
	public:

		Widget(Widget* parent = nullptr);
		
		virtual ~Widget();

		Widget* getParent();

		const Widget * getParent() const;

		void setParent(Widget* parent);

		/*Layout* getLayout();

		const Layout* getLayout() const;

		void setLayout(Layout* layout);*/

		Theme* getTheme();

		const Theme* getTheme() const;

		virtual void setTheme(Theme* theme);

		const Vector2i& getPosition() const;

		void setPosition(const Vector2i& position);

		Vector2i getAbsolutePosition() const;

		const Vector2i& getSize() const;

		void setSize(const Vector2i &size);

		int32 getWidth() const;

		void setWidth(int32 width);

		int32 getHeight() const;

		void setHeight(int32 height);

		void setFixedSize(const Vector2i& fixedSize);

		const Vector2i& getFixedSize() const;

		int32 getFixedWidth() const;

		int32 getFixedHeight() const;

		void setFixedWidth(int32 width);

		void setFixedHeight(int32 height);

		bool isVisible(bool recursive) const;

		void setVisible(bool visible);

		int32 getChildCount() const;

		const std::vector<Widget*>& getChildren() const;

		virtual void addChild(Widget* widget, int32 index);

		void addChild(Widget* widget);

		void removeChild(int32 index);

		void removeChild(Widget* widget);

		const Widget* getChild(int32 index) const;

		Widget* getChild(int32 index);

		int32 getChildIndex(Widget* widget) const;

		template<typename WidgetClass, typename... Args>
		WidgetClass* add(const Args&... args);

		GUIWindow* getWindow();

		void setWindow(GUIWindow* window);

		void setId(const std::string& id);

		const GlobalObjectID& getId() const;

		bool isEnabled() const;

		void setEnabled(bool enabled);

		bool isFocused() const;

		void setFocused(bool focused);

		void requestFocus();

		const std::string& getTooltip() const;

		void setTooltip(const std::string& tooltip);

		int32 getFontSize() const;

		void setFontSize(int32 fontSize);

		bool hasFontSize() const;

		float getIconExtraScale() const;

		void setIconExtraScale(float scale);

		bool hasMouseFocus() const;

		void setMouseFocus(bool mouseFocus);

		//cursor????

		bool contains(const Vector2i& p) const;

		Widget* findWidget(const Vector2i& p);

		virtual bool mouseButtonEvent(const Vector2i &p, Key button, bool down, ModifierKeysState modifiers);

		virtual bool mouseMotionEvent(const Vector2i &p, const Vector2i &rel);

		virtual bool mouseDragEvent(const Vector2i &p, const Vector2i &rel, Key button, ModifierKeysState modifiers);

		virtual bool mouseEnterEvent(const Vector2i &p, bool enter);

		virtual bool scrollEvent(const Vector2i &p, float delta, MouseEvent::Wheel wheel);

		virtual bool focusEvent(bool focused);

		virtual bool keyEvent(Key key, KeyEvent::Type action, ModifierKeysState modifiers);

		virtual Vector2i preferredSize(GUIRenderContext& renderContext) const;

		virtual void performLayout(GUIRenderContext& renderContext);

		virtual void render(GUIRenderContext& renderContext);

	private:

		GUIWindow* _window;

		Widget* _parent;
		Theme* _theme;
		//Layout* _layout;
		GlobalObjectID _id;
		Vector2i _position;
		Vector2i _size;
		Vector2i _fixedSize;
		std::vector<Widget *> _children;

		bool _visible;

		bool _enabled;
		bool _focused;
		bool _mouseFocus;
		std::string _tooltip;
		int32 _fontSize;
		float _iconExtraScale;
	};
	
	template<typename WidgetClass, typename... Args>
	WidgetClass* Widget::add(const Args&... args)
	{
		if (!IsBaseOf<Widget, WidgetClass>::Value)
		{
			WidgetClass* widget = new WidgetClass(this, args);
			addChild(widget);
		}

		return nullptr;
	}
}

#endif // !WIDGET_H_

