#include "stdafx.h"

#include "ResourceManager.h"
#include "ResourceCreator.h"
#include "Serializable.h"
#include "Types.h"

#include "FileOutputStream.h"
#include "XMLResourceDatabase.h"
#include "Engine.h"
#include "Globals.h"
#include "Renderer.h"
#include "World.h"
#include "Application.h"
#include "WindowsApplicationDefinition.h"
#include "WindowsWindowDefinition.h"
#include <iostream>

using namespace Arcana;

class Test2 : public Serializable
{
public:

	Test2() {};

	double d;
	std::string str;

	void writeObject(Archive& archive)
	{
		archive.writeDouble(d);
		archive.writeString(str);
	}

	void readObject(Archive& archive)
	{
		d = archive.readDouble();
		str = archive.readString();
	}

	int64 getObjectSize()
	{
		return 12 + str.size();
	}
};

class Test : public Object
{
public:

	Test() {};

	int a;
	float b;
	Test2 c;
};

class TestResource : public ResourceCreator<Test>
{
public:

	TestResource(const std::string& name, const std::string& type, const ResourceData& data)
		: ResourceCreator<Test>(name, type, data)
	{
		a = data.getInt32Parameter("a");
		b = data.getFloatParameter("b");
		//c = data.getObjectParameter<Test2>("c");
	}
};

Resource::Type<TestResource> testResource("test");

class Test3 : public Serializable
{
public:

	Test3() : d(0.0), f(0.0) {};

	double d;
	float f;

	void writeObject(Archive& archive)
	{
		archive.writeDouble(d);
		archive.writeFloat(f);
	}

	void readObject(Archive& archive)
	{
		d = archive.readDouble();
		f = archive.readFloat();
	}

	int64 getObjectSize()
	{
		return Types::Double.Size + Types::Float.Size;
	}
};

class AnotherResource
{
public:

	AnotherResource() {};

	std::string s;
	double d;
};

class AnotherResourceCreator : public ResourceCreator<AnotherResource>
{
public:

	AnotherResourceCreator(const std::string& name, const std::string& type, const ResourceData& data)
		: ResourceCreator<AnotherResource>(name, type, data)
	{
		s = data.getStringParameter("s");
		d = data.getDoubleParameter("d");
	}
};

Resource::Type<AnotherResourceCreator> anotherResource("anotherResource");

class BasicResource
{
public:

	BasicResource() : dep1(nullptr), dep2(nullptr) {};

	int32 i1;
	int32 i2;
	Test3 o;

	AnotherResource* dep1;
	AnotherResource* dep2;
};

class BasicResourceCreator : public ResourceCreator<BasicResource>
{
public:

	BasicResourceCreator(const std::string& name, const std::string& type, const ResourceData& data)
		: ResourceCreator<BasicResource>(name, type, data)
	{
		i1 = data.getInt32Parameter("i1");
		i2 = data.getInt32Parameter("i2");
		//o = data.getObjectParameter<Test3>("object");
		const ResourceData* object = data.getAdditionalData("object");
		if (object)
		{
			o.d = object->getDoubleParameter("d");
			o.f = object->getFloatParameter("f");
		}

		LoadResourceTask<AnotherResource>* task = ResourceManager::instance().loadResource<AnotherResource>(data.getResourceDependency("dependency1"));
		task->wait();
		dep1 = task->get();

		task = ResourceManager::instance().loadResource<AnotherResource>(data.getResourceDependency("dependency1"));
		task->wait();
		dep2 = task->get();
	}
};

Resource::Type<BasicResourceCreator> basicResource("basic");

class TestTask : public Task
{
public:

	TestTask(const std::string& string) : Task("TestTask"), _string(string)
	{

	}

	virtual void run() override
	{
		LOGF(Info, CoreEngine, "test task run, %s", _string.c_str());
	}

	virtual void done() override
	{
		LOGF(Info, CoreEngine, "test task done, %s", _string.c_str());
	}

private:

	std::string _string;
};

class MyActor : public Actor
{
public:

	double test;
	double test2;
};

class MyActorResource : public ResourceCreator<MyActor>
{
public:

	MyActorResource(const std::string& name, const std::string& type, const ResourceData& data)
		: ResourceCreator<MyActor>(name, type, data)
	{
		test = data.getDoubleParameter("test");
		test2 = data.getDoubleParameter("test2");
	}
};

Resource::Type<MyActorResource> myActorResource("myactor");

void testLoadedCallback(World* world)
{
	LOGF(Info, CoreEngine, "WORLD CALLBACK: %p", world);
}

int main()
{
	/*Archive archive;
	archive.writeInt32(100);
	archive.writeInt32(1234);
	Test3 o;
	o.d = 1002.03;
	o.f = 293.4f;
	o.writeObject(archive);
	FileOutputStream ostream;
	ostream.open("resource_data.dat");
	archive.writeToOutputStream(ostream);*/

	Types::registerType<Test3>("Test3", "Test Class Number 3");

	//new

	XMLResourceDatabase* database = XMLResourceDatabase::create("resource_database.xml");

	ResourceManager::instance().initialize(database);

	GlobalObjectID id("resource_0");
	LoadResourceTask<BasicResource>* resource = ResourceManager::instance().loadResource<BasicResource>(id);

	resource->wait();
	LOGF(Info, CoreEngine, "DONE");
	
	BasicResource* object = resource->get();

	if (object)
	{
		LOGF(Info, ResourceLog, "Object: %d, %d, %f, %f", object->i1, object->i2, object->o.d, object->o.f);
		LOGF(Info, ResourceLog, "Object dep1: %s, %f", object->dep1->s.c_str(), object->dep1->d);
		LOGF(Info, ResourceLog, "Object dep2: %s, %f", object->dep2->s.c_str(), object->dep2->d);
	}

	AE_DELETE(resource);

	/*std::cout << std::endl << "TASK TEST" << std::endl << std::endl;

	Scheduler* scheduler = new Scheduler();

	TestTask* t1 = new TestTask("1");
	t1->addDependency(new TestTask("2"));
	t1->addDependency(new TestTask("3"));
	t1->addDependency(new TestTask("4"));
	t1->addDependency(new TestTask("5"));
	t1->addDependency(new TestTask("6"));
	TestTask* t2 = new TestTask("7");
	t2->addDependency(new TestTask("8"));
	t2->addDependency(new TestTask("9"));
	t2->addDependency(new TestTask("10"));
	t1->addDependency(t2);

	scheduler->schedule(t1);

	while (!t1->isDone()) {
		//std::cout << "not done" << std::endl;
	}

	AE_DELETE(t1);
	AE_DELETE(scheduler);*/

	InitEngine();

	WindowsWindowDefinition windowDef;
	windowDef.setWidth(1920);
	windowDef.setHeight(1080);
	windowDef.setStyle(Style::Default);

	WindowsApplicationDefinition appDefinition;
	appDefinition.setAppName("Resource Client");
	appDefinition.setWindowClass(L"RESOURCE_CLIENT");
	appDefinition.setInstance(0);
	appDefinition.setCommandLineArgs(0);
	appDefinition.setShowCommand(0);
	appDefinition.addWindowDefinition(windowDef);

	GEngine->createApplication(appDefinition);

	RenderSettings settings;
	settings.bitsPerPixel = 32;
	settings.depthBits = 8;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 4;
	settings.minorVersion = 5;
	settings.attributeFlags = RenderSettings::Default;
	settings.splashScreenLength = 8.0f;
	settings.sRgb = false;

	GEngine->setRenderer(settings);

	LoadResourceTask<Shader>* shaderTask = ResourceManager::instance().loadResource<Shader>(GlobalObjectID("shader1"));
	shaderTask->wait();
	Shader* s = shaderTask->get();

	GlobalObjectID idWorld("world1");

	ResourceLoadedCallback<World> callback;
	callback.bind(testLoadedCallback);
	LoadResourceTask<World>* resourceWorld = ResourceManager::instance().loadResource<World>(idWorld, callback);

	resourceWorld->wait();
	LOGF(Info, CoreEngine, "DONE WORLD");

	World* world = resourceWorld->get();

	GEngine->start();

	DestroyEngine();

	return 0;
}