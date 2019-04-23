#ifndef GUI_WINDOW_H_
#define GUI_WINDOW_H_

#include "GUIDefines.h"

#include "Actor.h"
#include "Application.h"
#include "Widget.h"

namespace Arcana
{

	class ARCANA_GUI_API GUIWindow : public Actor, public EventListener
	{
	public:

		GUIWindow(Application* application, const std::string& name, int32 width = -1, int32 height = -1);

		~GUIWindow();

		virtual void initialize(std::string name, const Actor* templateActor = nullptr) override;

		virtual void update(double elapsedTime) override;

		virtual void render(ObjectRenderer& renderer, Matrix4d view, Matrix4d projection, Vector3d eyePosition) override;

		virtual void destroyed() override;


		virtual bool processEvent(Event& event, EventHandler& handler) override;


		int32 getWidgetCount() const;

		const std::vector<Widget*>& getWidgets() const;

		void addWidget(Widget* widget, int32 index);

		void addWidget(Widget* widget);

		void removeWidget(int32 index);

		void removeWidget(Widget* widget);

		const Widget* getWidget(int32 index) const;

		Widget* getWidget(int32 index);

		int32 getWidgetIndex(Widget* widget) const;

		template<typename WidgetClass, typename... Args>
		WidgetClass* add(const Args&... args);

	private:

		Vector2i _lastMouse;
		bool _firstMouseEvent;

		std::vector<Widget*> _widgets;

		GUIRenderContext _renderContext;
	};

	template<typename WidgetClass, typename... Args>
	WidgetClass* GUIWindow::add(const Args&... args)
	{
		if (!IsBaseOf<Widget, WidgetClass>::Value)
		{
			WidgetClass* widget = new WidgetClass(this, args);
			addChild(widget);
		}

		return nullptr;
	}
}

#endif // !GUI_WINDOW_H_

