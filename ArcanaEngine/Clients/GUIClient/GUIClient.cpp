// GUIClient.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "GUIClient.h"

#include "Application.h"

#include "WindowsWindowDefinition.h"
#include "WindowsApplicationDefinition.h"

#include "Renderer.h"
#include "EventListener.h"
#include "KeyEvent.h"
#include "Key.h"
#include "NoDataEvents.h"
#include "Globals.h"
#include "GlobalShaders.h"

#include "GUIWindow.h"
#include "Button.h"
#include "CheckBox.h"
#include "Label.h"
#include "ImagePanel.h"
#include "Slider.h"
#include "Input.h"

//FasterThanLight
//#define BUILD_LIGHTING
//#include "FasterThanLight.h"

//vld
#include <vld.h>

//dependencies
#include "CoreModule.h"
#include "EngineModule.h"
#include "GraphicsModule.h"
#include "InputModule.h"
#include "IOModule.h"
#include "ArcanaLog.h"
#include "ArcanaMath.h"
#include "SceneModule.h"

using namespace Arcana;


class MyListener : public EventListener
{
public:

	MyListener()
	{
		listenForEvent(EventID::KeyEventID);
	}

	~MyListener()
	{

	}

	virtual bool processEvent(Event& event, EventHandler& handler) override
	{
		static bool vsyncEnabled = true;

		if (event.getInt("keyCode") == KeyCode::V)
		{
			vsyncEnabled = !vsyncEnabled;
			//GEngine->getApplicationInstance()->getActiveWindow().setVerticalSync(vsyncEnabled);
		}
		else if (event.getInt("keyCode") == KeyCode::Escape || event.getInt("keyCode") == KeyCode::ControllerSpecialRight)
		{
			handler.broadcast(WindowClosedEvent());
		}

		return true;
	}
};

void sliderBinding(float value)
{
	LOGF(Info, CoreEngine, "slider value: %f", value);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	InitEngine();

	WindowsWindowDefinition windowDef;
	windowDef.setWidth(1920);
	windowDef.setHeight(1080);
	windowDef.setStyle(Style::Default);

	WindowsApplicationDefinition appDefinition;
	appDefinition.setAppName("GUI Client");
	appDefinition.setWindowClass(L"CURVE_CLIENT");
	appDefinition.setInstance(hInstance);

	appDefinition.setCommandLineArgs(lpCmdLine);
	appDefinition.setShowCommand(nCmdShow);
	appDefinition.addWindowDefinition(windowDef);

	GEngine->createApplication(appDefinition);
	GEngine->getApplicationInstance()->getEventHandler().addEventListener(std::shared_ptr<MyListener>(new MyListener()));

	Input::setStaticRelativeWindow(GEngine->getApplicationInstance()->getActiveWindow());

	RenderSettings settings;
	settings.bitsPerPixel = 32;
	settings.depthBits = 8;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 4;
	settings.minorVersion = 5;
	settings.attributeFlags = RenderSettings::Default;
	settings.sRgb = false;

	GEngine->setRenderer(settings);

	GEngine->getApplicationInstance()->getActiveWindow().setVerticalSync(false);

	World* world = new World("world");

	GUIWindow* window = new GUIWindow(GEngine->getApplicationInstance(), "window");

	Button* b = new Button();
	b->setCaption("Button 1");
	b->setIconExtraScale(0.3);
	b->setIcon(b->getTheme()->CheckIcon);
	b->setEnabled(true);
	b->setFontSize(32);
	b->setTextColor(Color(255, 255, 255, 255));
	b->setBackgroundColor(Color(0, 0, 255, 25));
	b->setVisible(true);
	b->setPosition(Vector2i(100, 100));
	b->setSize(Vector2i(200, 60));
	window->addWidget(b);

	CheckBox* cb = new CheckBox();
	cb->setCaption("check box");
	cb->setEnabled(true);
	cb->setVisible(true);
	cb->setPosition(Vector2i(100, 400));
	cb->setSize(Vector2i(60, 60));
	window->addWidget(cb);

	Button* b2 = new Button();
	b2->setCaption("Button 2");
	b2->setIconExtraScale(0.3);
	b2->setIcon(b->getTheme()->CheckIcon);
	b2->setIconPosition(Button::IconPosition::RightCentered);
	b2->setEnabled(true);
	b2->setFontSize(32);
	b2->setTextColor(Color(255, 255, 255, 255));
	b2->setVisible(true);
	b2->setPosition(Vector2i(600, 100));
	b2->setSize(Vector2i(200, 60));
	//b2->setBorder(3.0);
	window->addWidget(b2);

	Button* b3 = new Button();
	b3->setCaption("");
	b3->setEnabled(true);
	b3->setFontSize(32);
	b3->setIcon(b->getTheme()->CheckIcon);
	b3->setIconPosition(Button::IconPosition::RightCentered);
	b3->setTextColor(Color(255, 255, 255, 255));
	b3->setVisible(true);
	b3->setPosition(Vector2i(400, 400));
	b3->setSize(Vector2i(200, 60));
	window->addWidget(b3);

	Button* b4 = new Button();
	b4->setCaption("Button 3");
	b4->setEnabled(true);
	b4->setFontSize(32);
	b4->setTextColor(Color(255, 255, 255, 255));
	b4->setVisible(true);
	b4->setPosition(Vector2i(600, 100));
	b4->setSize(Vector2i(200, 60));
	b4->setSidebar(10);
	b4->setFlags(Button::Normal);
	window->addWidget(b4);

	Label* l = new Label(nullptr, "Label 1");
	l->setEnabled(true);
	l->setFontSize(32);
	l->setVisible(true);
	l->hasBackground(true);
	l->setPosition(Vector2i(800, 400));
	window->addWidget(l);

	Panel* p = new Panel();
	p->setTitle("Panel");
	p->setEnabled(true);
	p->setVisible(true);
	//p->setBackgroundColor(Color(0, 0, 255, 25));
	p->setDropShadow(false);
	p->setPosition(Vector2i(800, 400));
	p->setSize(Vector2i(400, 400));
	p->setLayout(new BoxLayout(Layout::Orientation::Vertical, Layout::Alignment::Minimum, 4, 4, 4));
	//p->getButtonPanel();// ->addChild(b3);
	Widget* buttonPanel = new Widget();
	p->addChild(buttonPanel);
	buttonPanel->setLayout(new BoxLayout(Layout::Orientation::Horizontal, Layout::Alignment::Middle, 0, 0, 4));
	buttonPanel->addChild(b3);
	p->_buttonPanel = buttonPanel;
	CheckBox* cb1 = new CheckBox();
	cb1->setCaption("check box panel");
	cb1->setEnabled(true);
	cb1->setVisible(true);
	cb1->setPosition(Vector2i(100, 400));
	cb1->setSize(Vector2i(60, 60));
	p->addChild(cb1);
	p->addChild(b4);
	window->addWidget(p);

	Panel* p2 = new Panel();
	p2->setTitle("Panel 2");
	p2->setEnabled(true);
	p2->setVisible(true);
	p2->setDropShadow(false);
	p2->setLayout(new BoxLayout(Layout::Orientation::Vertical, Layout::Alignment::Fill, 4, 4, 4));
	p2->setPosition(Vector2i(800, 400));
	p2->setSize(Vector2i(400, 400));
	window->addWidget(p2);

	/*ImagePanel* imagePanel = new ImagePanel();
	imagePanel->setEnabled(true);
	imagePanel->setVisible(true);
	imagePanel->setPosition(Vector2i(400, 800));
	imagePanel->setSize(Vector2i(400, 400));
	imagePanel->hasBackground(true);
	imagePanel->setBackgroundColor(Color(255, 255, 0));
	imagePanel->addImage(b->getTheme()->CheckIcon);
	imagePanel->addImage(b->getTheme()->CheckIcon);
	imagePanel->addImage(b->getTheme()->CheckIcon);
	imagePanel->addImage(b->getTheme()->CheckIcon);
	imagePanel->addImage(b->getTheme()->CheckIcon);
	imagePanel->addImage(b->getTheme()->CheckIcon);
	imagePanel->addImage(b->getTheme()->CheckIcon);
	p2->addChild(imagePanel);*/

	Slider* s = new Slider();
	s->setEnabled(true);
	s->setVisible(true);
	s->setPosition(Vector2i(0, 400));
	s->setSize(Vector2i(200, 40));
	//s->setColor(Color(100, 100, 100));
	s->setHighlightReversed(true);
	s->setHighlightColor(Color(54, 146, 255, 255));
	s->setVertical(true);
	//s->setRectangleKnob(true);
	//s->setSizeFactor(1.0f);
	//s->setKnobSize(Vector2f(1.0f, 0.4f));
	s->getFinalCallback().bind(sliderBinding);
	window->addWidget(s);

	Slider* s1 = new Slider();
	s1->setEnabled(true);
	s1->setVisible(true);
	s1->setPosition(Vector2i(0, 400));
	s1->setSize(Vector2i(200, 40));
	s1->setColor(Color(50, 50, 50));
	s1->setHighlightColor(Color(180, 0, 0, 255));
	//s1->setSizeFactor(1.0f);
	//s1->setKnobSize(Vector2f(0.5f, 0.5f));
	s1->setValue(0.5);
	window->addWidget(s1);

	Slider* s2 = new Slider();
	s2->setEnabled(true);
	s2->setVisible(true);
	s2->setPosition(Vector2i(0, 400));
	s2->setSize(Vector2i(200, 40));
	//s->setColor(Color(100, 100, 100));
	s2->setSizeFactor(0.5f);
	s2->setHighlightReversed(true);
	s2->setHighlightColor(Color(54, 146, 255, 255));
	window->addWidget(s2);

	window->setLayout(new BoxLayout(Layout::Orientation::Horizontal, Layout::Alignment::Minimum, 10, 10, 10));
	//gui->setLayout(new GroupLayout());
	window->updateLayout();

	//color picker
	//color wheel
	//combo box
	//message dialog
	//popup button
	//progress bar
	//slider
	//tab header
	//tabbed window
	//text box
	//scroll panel

	world->addActor(window);

	GEngine->setWorld(world);

	GEngine->start();
	GEngine->exit();

	DestroyEngine();

	return 1;
}