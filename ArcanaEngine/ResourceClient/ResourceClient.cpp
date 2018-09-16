#include "stdafx.h"

#include "ResourceManager.h"
#include "ResourceCreator.h"
#include "Serializable.h"
#include "Types.h"

#include "FileOutputStream.h"

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

class Test
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

	TestResource(const std::string& name, ResourceData& data)
		: ResourceCreator<Test>(name, data)
	{
		a = data.getInt32Parameter("a");
		b = data.getFloatParameter("b");
		c = data.getObjectParameter<Test2>("c");
	}
};

Resource::Type<TestResource> testResource("test");

class Test3 : public Serializable
{
public:

	Test3() {};

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
		return Types::Double.size + Types::Float.size;
	}
};

class BasicResource
{
public:

	BasicResource() {};

	int32 i1;
	int32 i2;
	Test3 o;
};

class BasicResourceCreator : public ResourceCreator<BasicResource>
{
public:

	BasicResourceCreator(const std::string& name, ResourceData& data)
		: ResourceCreator<BasicResource>(name, data)
	{
		i1 = data.getInt32Parameter("i1");
		i2 = data.getInt32Parameter("i2");
		o = data.getObjectParameter<Test3>("object");
	}
};

Resource::Type<BasicResourceCreator> basicResource("basic");

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

	Types::registerType(12, "Test3", "Test Class Number 3");

	ResourceManager::instance().addDefaultHeader("test_header.xml");
	ResourceManager::instance().initialize("test_database.xml");

	BasicResource object = ResourceManager::instance().loadResource<BasicResource>("resource_0");

	LOGF(Info, ResourceLog, "Object: %d, %d, %f, %f", object.i1, object.i2, object.o.d, object.o.f);

	system("pause");

	return 0;
}