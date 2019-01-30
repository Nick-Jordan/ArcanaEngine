#ifndef TEST_ClASS_H_
#define TEST_ClASS_H_

#include "Serializable.h"
#include <string>

using namespace Arcana;

namespace Test
{
	class OtherClass : public Serializable
	{
	public:
			
		OtherClass() : f(0.0f), str("") {}
		OtherClass(float f1, std::string s) : f(f1), str(s) {}
		
		virtual void writeObject(Archive& archive) override
		{
			archive.writeFloat(f);
			archive.writeString(str);
		}
		
		virtual void readObject(Archive& archive) override
		{
			f = archive.readFloat();
			str = archive.readString();
		}
			
		virtual int64 getObjectSize() override
		{
			return 4 + 8 + str.size();
		}
			
		float f;
		std::string str;
	};
	
	class TestClass : public Serializable
	{
	public:
	
		TestClass() : integer(0), integer2(0) {}
		TestClass(int i1, int i2) : integer(i1), integer2(i2) {}
		
		virtual void writeObject(Archive& archive) override
		{
			archive.writeInt32(integer);
			archive.writeInt32(integer2);
			other.writeObject(archive);
		}
		
		virtual void readObject(Archive& archive) override
		{
			integer = archive.readInt32();
			integer2 = archive.readInt32();
			other.readObject(archive);
		}
		
		virtual int64 getObjectSize() override
		{
			return 4 + 4 + other.getObjectSize();
		}
			
		int integer;
		int integer2;
		
		OtherClass other;
	
	};
}

#endif // !TEST_ClASS_H_