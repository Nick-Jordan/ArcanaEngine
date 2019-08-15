#include "stdafx.h"
#include "CppUnitTest.h"

#include "StringUtils.h";
#include "Timeline.h"
#include "UnitTestToString.h"

using namespace Arcana;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CoreUnitTests
{
	template<typename Value>
	void AssertArrayElementsEqual(const Array<Value>& a, const Array<Value>& b)
	{
		Assert::AreEqual(a.size(), b.size());

		for (int i = 0; i < a.size(); i++)
		{
			Assert::AreEqual(a[i], b[i]);
		}
	}

	template<typename Value>
	void AssertArrayElementsEqual(const Array<Value>& a, Value* b)
	{
		for (int i = 0; i < a.size(); i++)
		{
			Assert::AreEqual(a[i], b[i]);
		}

		delete[] b;
	}

	class TestObject
	{
	public:

		float f;
		float f1;
		int i;
	};

	TEST_CLASS(ArrayUnitTests)
	{
	public:

		TEST_METHOD(CreateArray)
		{
			Array<int> array;
			Assert::AreEqual(0, array.size());
			
			array.add(10);
			array.add(20);

			Array<int> copy(array);

			Assert::AreEqual(array.size(), copy.max());
			Assert::AreEqual(0, copy.getSlack());
			AssertArrayElementsEqual(array, copy);

			Array<int> copySlack(array, 10);

			Assert::AreEqual(array.size() + 10, copySlack.max());
			Assert::AreEqual(10, copySlack.getSlack());
			AssertArrayElementsEqual(array, copySlack);
		}

		TEST_METHOD(GetTypeSize)
		{
			Array<int> intArray;
			Array<double> doubleArray;
			Array<TestObject> objectArray;

			Assert::AreEqual((uint32)4, intArray.getTypeSize());
			Assert::AreEqual((uint32)8, doubleArray.getTypeSize());
			Assert::AreEqual((uint32)12, objectArray.getTypeSize());
		}

		TEST_METHOD(GetAllocatedSize)
		{
			Array<int> array;

			Assert::AreEqual((uint32)0, array.getAllocatedSize());

			array.add(10);

			Assert::AreEqual((uint32)16, array.getAllocatedSize());

			array.add(1);
			array.add(2);
			array.add(3);

			Assert::AreEqual((uint32)16, array.getAllocatedSize());

			array.add(4);

			Assert::AreEqual((uint32)88, array.getAllocatedSize());
		}

		TEST_METHOD(GetSlack)
		{
			Array<int> array;

			Assert::AreEqual((int32)0, array.getSlack());

			array.add(10);

			Assert::AreEqual((int32)3, array.getSlack());

			array.add(1);
			array.add(2);
			array.add(3);

			Assert::AreEqual((int32)0, array.getSlack());

			array.add(4);

			Assert::AreEqual((int32)17, array.getSlack());
		}

		TEST_METHOD(IsValidIndex)
		{
			Array<int> array;

			Assert::IsFalse(array.isValidIndex(0));
			Assert::IsFalse(array.isValidIndex(-1));
			Assert::IsFalse(array.isValidIndex(1));

			array.add(1);
			array.add(2);

			Assert::IsTrue(array.isValidIndex(0));
			Assert::IsTrue(array.isValidIndex(1));
			Assert::IsFalse(array.isValidIndex(-1));
			Assert::IsFalse(array.isValidIndex(2));
		}

		TEST_METHOD(Size)
		{
			Array<int> array;

			Assert::AreEqual(0, array.size());

			array.add(1);
			array.add(2);

			Assert::AreEqual(2, array.size());
		}

		TEST_METHOD(Max)
		{
			Array<int> array;

			Assert::AreEqual(0, array.max());

			array.add(1);
			array.add(2);

			Assert::AreEqual(4, array.max());

			array.add(3);
			array.add(4);
			array.add(5);

			Assert::AreEqual(22, array.max());
		}

		TEST_METHOD(OffsetOperator)
		{
			Array<int> array;

			array.add(1);
			array.add(2);

			Assert::AreEqual(1, array[0]);
			Assert::AreEqual(2, array[1]);
		}

		TEST_METHOD(Pop)
		{
			Array<int> array;

			array.add(1);
			array.add(2);
			array.add(3);
			array.add(4);
			array.add(5);

			Assert::AreEqual(5, array.size());
			Assert::AreEqual(22, array.max());

			int p = array.pop(false);

			Assert::AreEqual(p, 5);
			Assert::AreEqual(4, array.size());
			Assert::AreEqual(22, array.max());
			AssertArrayElementsEqual(array, new int[4] {1, 2, 3, 4});

			p = array.pop(true);

			Assert::AreEqual(p, 4);
			Assert::AreEqual(3, array.size());
			Assert::AreEqual(22, array.max());
			AssertArrayElementsEqual(array, new int[3] {1, 2, 3});

			p = array.pop();
			p = array.pop();
			p = array.pop();

			Assert::AreEqual(p, 1);
			Assert::AreEqual(0, array.size());
			Assert::AreEqual(0, array.max());
		}

		TEST_METHOD(AddAndPush)
		{
			Array<int> array;

			Assert::AreEqual(0, array.size());
			Assert::AreEqual(0, array.max());

			array.push(1);

			Assert::AreEqual(1, array.size());
			Assert::AreEqual(4, array.max());
			Assert::AreEqual(1, array[0]);

			array.add(2);

			Assert::AreEqual(2, array.size());
			Assert::AreEqual(4, array.max());
			Assert::AreEqual(1, array[0]);
			Assert::AreEqual(2, array[1]);
		}

		TEST_METHOD(GetTop)
		{
			Array<int> array;

			array.add(1);

			Assert::AreEqual(1, array.getTop());

			array.add(2);

			Assert::AreEqual(2, array.getTop());
		}

		TEST_METHOD(GetLast)
		{
			Array<int> array;

			array.add(1);

			Assert::AreEqual(1, array.getLast());

			array.add(2);

			Assert::AreEqual(2, array.getLast());

			array.add(3);
			array.add(4);
			array.add(5);

			Assert::AreEqual(5, array.getLast());
			Assert::AreEqual(4, array.getLast(1));
			Assert::AreEqual(3, array.getLast(2));
		}

		TEST_METHOD(Shrink)
		{
			Array<int> array;

			array.add(1);

			Assert::AreEqual(1, array.size());
			Assert::AreEqual(4, array.max());
			array.shrink();
			Assert::AreEqual(1, array.size());
			Assert::AreEqual(1, array.max());

			array.add(2);
			array.add(3);
			array.add(4);
			array.add(5);

			Assert::AreEqual(5, array.size());
			Assert::AreEqual(18, array.max());
			array.shrink();
			Assert::AreEqual(5, array.size());
			Assert::AreEqual(5, array.max());
		}

		TEST_METHOD(Find)
		{
			Array<int> array;
			array.add(1);
			array.add(2);
			array.add(3);
			array.add(4);
			array.add(5);
			array.add(1);
			array.add(2);
			array.add(3);
			array.add(4);
			array.add(5);

			int32 i;
			for (int j = 1; j <= 5; j++)
			{
				Assert::IsTrue(array.find(j, i));
				Assert::AreEqual(j - 1, i);
			}
		
			Assert::IsFalse(array.find(6, i));
			Assert::AreEqual(INDEX_NONE, i);

			for (int j = 1; j <= 5; j++)
			{
				Assert::AreEqual(j - 1, array.find(j));
			}
			Assert::AreEqual(INDEX_NONE, array.find(6));
		}

		TEST_METHOD(FindLast)
		{
			Array<int> array;
			array.add(1);
			array.add(2);
			array.add(3);
			array.add(4);
			array.add(5);
			array.add(1);
			array.add(2);
			array.add(3);
			array.add(4);
			array.add(5);

			int32 i;
			for (int j = 1; j <= 5; j++)
			{
				Assert::IsTrue(array.findLast(j, i));
				Assert::AreEqual(array.size() - (6 - j), i);
			}

			Assert::IsFalse(array.findLast(6, i));
			Assert::AreEqual(INDEX_NONE, i);

			for (int j = 1; j <= 5; j++)
			{
				Assert::AreEqual(array.size() - (6 - j), array.findLast(j));
			}
			Assert::AreEqual(INDEX_NONE, array.findLast(6));
		}

		TEST_METHOD(EqualsOperator)
		{
			Array<int> array1;
			array1.add(1);
			array1.add(2);
			array1.add(3);
			array1.add(4);
			array1.add(5);
			Array<int> array2;
			array2.add(1);
			array2.add(2);
			array2.add(3);
			array2.add(4);
			array2.add(5);

			Assert::IsTrue(array1 == array2);
			array2[4] = 10;
			Assert::IsFalse(array1 == array2);
			array2.add(10);
			Assert::IsFalse(array1 == array2);
		}

		TEST_METHOD(NotEqualsOperator)
		{
			Array<int> array1;
			array1.add(1);
			array1.add(2);
			array1.add(3);
			array1.add(4);
			array1.add(5);
			Array<int> array2;
			array2.add(1);
			array2.add(2);
			array2.add(3);
			array2.add(4);
			array2.add(5);

			Assert::IsFalse(array1 != array2);
			array2[4] = 10;
			Assert::IsTrue(array1 != array2);
			array2.add(10);
			Assert::IsTrue(array1 != array2);
		}
	};

	TEST_CLASS(UnionUnitTests)
	{
	public:

		struct TestStruct
		{
			float a;
			float b;
		};

		TEST_METHOD(SetGet)
		{
			Union<int, double, float> u;

			u.set(10);
			Assert::AreEqual(10, u.get<int>());

			u.set(11.0);
			Assert::AreEqual(11.0, u.get<double>());

			u.set(12.0f);
			Assert::AreEqual(12.0f, u.get<float>());
		}

		TEST_METHOD(SubtypeObject)
		{
			Union<int, double, float, TestStruct> u;

			u.set(10);
			Assert::AreEqual(10, u.get<int>());

			u.set(11.0);
			Assert::AreEqual(11.0, u.get<double>());

			u.set(12.0f);
			Assert::AreEqual(12.0f, u.get<float>());

			TestStruct s;
			s.a = 13.0f;
			s.b = 14.0f;
			u.set(s);
			Assert::AreEqual(13.0f, u.get<TestStruct>().a);
			Assert::AreEqual(14.0f, u.get<TestStruct>().b);
		}
	};

	TEST_CLASS(StringUtilsUnitTests)
	{
	public:
		
		TEST_METHOD(Split)
		{
			std::string s0 = "first.second";
			std::string s1 = "a.,/b.,/c.,/d";
			std::string s2 = "a.";
			std::string s3 = "a..b";

			std::vector<std::string> v0 = StringUtils::split(s0, ".");
			std::vector<std::string> v1 = StringUtils::split(s1, ".,/");
			std::vector<std::string> v2 = StringUtils::split(s2, ".");
			std::vector<std::string> v3 = StringUtils::split(s3, ".");

			Assert::AreEqual(2, (int)v0.size());
			Assert::AreEqual("first", v0[0].c_str());
			Assert::AreEqual("second", v0[1].c_str());

			Assert::AreEqual(4, (int)v1.size());
			Assert::AreEqual("a", v1[0].c_str());
			Assert::AreEqual("b", v1[1].c_str());
			Assert::AreEqual("c", v1[2].c_str());
			Assert::AreEqual("d", v1[3].c_str());

			Assert::AreEqual(1, (int)v2.size());
			Assert::AreEqual("a", v2[0].c_str());

			Assert::AreEqual(3, (int)v3.size());
			Assert::AreEqual("a", v3[0].c_str());
			Assert::AreEqual("", v3[1].c_str());
			Assert::AreEqual("b", v3[2].c_str());
		}

		TEST_METHOD(Substring)
		{
			std::string str = "this is a string";

			std::string s0 = StringUtils::substring(str, 2, 6);
			std::string s1 = StringUtils::substring(str, 8);
			std::string s2 = StringUtils::substring(str, 5, 3);
			std::string s3 = StringUtils::substring(str, 10, 100);
			std::string s4 = StringUtils::substring(str, 100, 6);

			Assert::AreEqual("is i", s0.c_str());
			Assert::AreEqual("a string", s1.c_str());
			Assert::AreEqual("", s2.c_str());
			Assert::AreEqual("string", s3.c_str());
			Assert::AreEqual("", s4.c_str());
		}

		TEST_METHOD(Replace)
		{
			std::string str = "string string string";

			StringUtils::replace(str, "ing", "aaa");
			std::string fixed = StringUtils::replaceFixed(str, "ing", "aaa");

			Assert::AreEqual("straaa string string", str.c_str());
			Assert::AreEqual("straaa straaa string", fixed.c_str());
		}

		TEST_METHOD(ReplaceAll)
		{
			std::string str = "string string string";

			StringUtils::replaceAll(str, "ing", "aaa");
			std::string fixed = StringUtils::replaceAllFixed(str, "ing", "aaa");

			Assert::AreEqual("straaa straaa straaa", str.c_str());
			Assert::AreEqual("straaa straaa straaa", fixed.c_str());
		}

		TEST_METHOD(StartsWith)
		{
			std::string str = "prefixstring";

			Assert::AreEqual(true, StringUtils::startsWith(str, "prefix"));
			Assert::AreEqual(false, StringUtils::startsWith(str, "prefixa"));
		}

		TEST_METHOD(EndsWith)
		{
			std::string str = "suffixstring";

			Assert::AreEqual(true, StringUtils::endsWith(str, "ring"));
			Assert::AreEqual(false, StringUtils::endsWith(str, "ringa"));
		}

		TEST_METHOD(ToUpper)
		{
			std::string str = "lower CaSe sTring";

			Assert::AreEqual("LOWER CASE STRING", StringUtils::toUpper(str).c_str());
		}

		TEST_METHOD(ToLower)
		{
			std::string str = "UpPer CaSe sTring";

			Assert::AreEqual("upper case string", StringUtils::toLower(str).c_str());
		}

		TEST_METHOD(Trim)
		{
			std::string s0 = "  string ";
			std::string s1 = "str ing  ";
			std::string s2 = "  str  ing";
			std::string s3 = "	string	string	s ";

			Assert::AreEqual("string", StringUtils::trim(s0).c_str());
			Assert::AreEqual("str ing", StringUtils::trim(s1).c_str());
			Assert::AreEqual("str ing", StringUtils::trim(s2).c_str());
			Assert::AreEqual("string	string	s", StringUtils::trim(s3).c_str());
		}

		TEST_METHOD(AbbreviateMaxWidth)
		{
			Assert::AreEqual("str...", StringUtils::abbreviate("string", 6).c_str());
			Assert::AreEqual("st...", StringUtils::abbreviate("string", 5).c_str());
			Assert::AreEqual("", StringUtils::abbreviate("string", 3).c_str());
		}
	};

	TEST_CLASS(TimelineUnitTests)
	{
	public:

		bool b;

		void testFunc()
		{
			b = true;
		}

		TEST_METHOD(Trigger)
		{
			b = false;
			Timeline timeline;
			TimelineTrigger trigger;
			trigger.bind(this, &TimelineUnitTests::testFunc);
			timeline.addTrigger(0.5, trigger);
			timeline.setTimelineLengthMode(Timeline::LengthMode::LastKeyFrame);

			timeline.play();
			timeline.updateTimeline(0.4);
			Assert::IsFalse(b);

			timeline.updateTimeline(0.1);
			Assert::IsTrue(b);
			b = false;
		}

		TEST_METHOD(InterpolatedFloatOneEntry)
		{
			Timeline timeline;
			timeline.play();
			timeline.addFloat(1.0, 5.0f);
			timeline.setTimelineLengthMode(Timeline::LengthMode::TimelineLength);
			timeline.setTimelineLength(10.0);

			Assert::AreEqual(0.0f, timeline.getCurrentFloat());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(0.0f, timeline.getCurrentFloat());
			timeline.updateTimeline(1.0);
			Assert::AreEqual(5.0f, timeline.getCurrentFloat());
			timeline.updateTimeline(1.0);
			Assert::AreEqual(5.0f, timeline.getCurrentFloat());
		}

		TEST_METHOD(InterpolatedFloatMultipleEntries)
		{
			Timeline timeline;
			timeline.play();
			timeline.addFloat(1.0, 5.0f);
			timeline.addFloat(2.0, 6.0f);
			timeline.addFloat(3.0, 7.0f);
			timeline.setTimelineLengthMode(Timeline::LengthMode::TimelineLength);
			timeline.setTimelineLength(10.0);

			Assert::AreEqual(0.0f, timeline.getCurrentFloat());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(0.0f, timeline.getCurrentFloat());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(5.0f, timeline.getCurrentFloat());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(5.5f, timeline.getCurrentFloat());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(6.0f, timeline.getCurrentFloat());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(6.5f, timeline.getCurrentFloat());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(7.0f, timeline.getCurrentFloat());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(7.0f, timeline.getCurrentFloat());
		}


		TEST_METHOD(InterpolatedVectorOneEntry)
		{
			Timeline timeline;
			timeline.play();
			timeline.addVector(1.0, Vector3f(2.0f, 4.0f, 1.0f));
			timeline.setTimelineLengthMode(Timeline::LengthMode::TimelineLength);
			timeline.setTimelineLength(10.0);

			Assert::AreEqual(Vector3f::zero(), timeline.getCurrentVector());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f::zero(), timeline.getCurrentVector());
			timeline.updateTimeline(1.0);
			Assert::AreEqual(Vector3f(2.0f, 4.0f, 1.0f), timeline.getCurrentVector());
			timeline.updateTimeline(1.0);
			Assert::AreEqual(Vector3f(2.0f, 4.0f, 1.0f), timeline.getCurrentVector());
		}

		TEST_METHOD(InterpolatedVectorMultipleEntries)
		{
			Timeline timeline;
			timeline.play();
			timeline.addVector(1.0, Vector3f(2.0f, 4.0f, 1.0f));
			timeline.addVector(2.0, Vector3f(3.0f, 5.0f, 2.0f));
			timeline.addVector(3.0, Vector3f(4.0f, 6.0f, 3.0f));
			timeline.setTimelineLengthMode(Timeline::LengthMode::TimelineLength);
			timeline.setTimelineLength(10.0);

			Assert::AreEqual(Vector3f::zero(), timeline.getCurrentVector());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f::zero(), timeline.getCurrentVector());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f(2.0f, 4.0f, 1.0f), timeline.getCurrentVector());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f(2.5f, 4.5f, 1.5f), timeline.getCurrentVector());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f(3.0f, 5.0f, 2.0f), timeline.getCurrentVector());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f(3.5f, 5.5f, 2.5f), timeline.getCurrentVector());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f(4.0f, 6.0f, 3.0f), timeline.getCurrentVector());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f(4.0f, 6.0f, 3.0f), timeline.getCurrentVector());
		}

		TEST_METHOD(InterpolatedLinearColorOneEntry)
		{
			Timeline timeline;
			timeline.play();
			timeline.addLinearColor(1.0, LinearColor(0.5f, 0.3f, 1.0f));
			timeline.setTimelineLengthMode(Timeline::LengthMode::TimelineLength);
			timeline.setTimelineLength(10.0);

			Assert::AreEqual(Vector3f::zero(), timeline.getCurrentLinearColor().toVector3());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f::zero(), timeline.getCurrentLinearColor().toVector3());
			timeline.updateTimeline(1.0);
			Assert::AreEqual(Vector3f(0.5f, 0.3f, 1.0f), timeline.getCurrentLinearColor().toVector3());
			timeline.updateTimeline(1.0);
			Assert::AreEqual(Vector3f(0.5f, 0.3f, 1.0f), timeline.getCurrentLinearColor().toVector3());
		}

		TEST_METHOD(InterpolatedLinearColorMultipleEntries)
		{
			Timeline timeline;
			timeline.play();
			timeline.addLinearColor(1.0, LinearColor(0.2f, 0.3f, 1.0f));
			timeline.addLinearColor(2.0, LinearColor(0.4f, 0.6f, 0.5f));
			timeline.addLinearColor(3.0, LinearColor(0.6f, 0.9f, 0.0f));
			timeline.setTimelineLengthMode(Timeline::LengthMode::TimelineLength);
			timeline.setTimelineLength(10.0);

			Assert::AreEqual(Vector3f::zero(), timeline.getCurrentLinearColor().toVector3());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f::zero(), timeline.getCurrentLinearColor().toVector3());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f(0.2f, 0.3f, 1.0f), timeline.getCurrentLinearColor().toVector3());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f(0.3f, 0.45f, 0.75f), timeline.getCurrentLinearColor().toVector3());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f(0.4f, 0.6f, 0.5f), timeline.getCurrentLinearColor().toVector3());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f(0.5f, 0.75f, 0.25f), timeline.getCurrentLinearColor().toVector3());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f(0.6f, 0.9f, 0.0f), timeline.getCurrentLinearColor().toVector3());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f(0.6f, 0.9f, 0.0f), timeline.getCurrentLinearColor().toVector3());
		}

		TEST_METHOD(InterpolatedFloatOneEntryReversed)
		{
			Timeline timeline;
			timeline.addFloat(9.0, 5.0f);
			timeline.setTimelineLengthMode(Timeline::LengthMode::TimelineLength);
			timeline.setTimelineLength(10.0);
			timeline.reverseFromEnd();

			Assert::AreEqual(0.0f, timeline.getCurrentFloat());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(0.0f, timeline.getCurrentFloat());
			timeline.updateTimeline(1.0);
			Assert::AreEqual(5.0f, timeline.getCurrentFloat());
			timeline.updateTimeline(1.0);
			Assert::AreEqual(5.0f, timeline.getCurrentFloat());
		}

		TEST_METHOD(InterpolatedFloatMultipleEntriesReversed)
		{
			Timeline timeline;
			timeline.addFloat(9.0, 5.0f);
			timeline.addFloat(8.0, 6.0f);
			timeline.addFloat(7.0, 7.0f);
			timeline.setTimelineLengthMode(Timeline::LengthMode::TimelineLength);
			timeline.setTimelineLength(10.0);
			timeline.reverseFromEnd();

			Assert::AreEqual(0.0f, timeline.getCurrentFloat());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(0.0f, timeline.getCurrentFloat());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(5.0f, timeline.getCurrentFloat());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(5.5f, timeline.getCurrentFloat());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(6.0f, timeline.getCurrentFloat());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(6.5f, timeline.getCurrentFloat());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(7.0f, timeline.getCurrentFloat());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(7.0f, timeline.getCurrentFloat());
		}


		TEST_METHOD(InterpolatedVectorOneEntryReversed)
		{
			Timeline timeline;
			timeline.addVector(9.0, Vector3f(2.0f, 4.0f, 1.0f));
			timeline.setTimelineLengthMode(Timeline::LengthMode::TimelineLength);
			timeline.setTimelineLength(10.0);
			timeline.reverseFromEnd();

			Assert::AreEqual(Vector3f::zero(), timeline.getCurrentVector());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f::zero(), timeline.getCurrentVector());
			timeline.updateTimeline(1.0);
			Assert::AreEqual(Vector3f(2.0f, 4.0f, 1.0f), timeline.getCurrentVector());
			timeline.updateTimeline(1.0);
			Assert::AreEqual(Vector3f(2.0f, 4.0f, 1.0f), timeline.getCurrentVector());
		}

		TEST_METHOD(InterpolatedVectorMultipleEntriesReversed)
		{
			Timeline timeline;
			timeline.addVector(9.0, Vector3f(2.0f, 4.0f, 1.0f));
			timeline.addVector(8.0, Vector3f(3.0f, 5.0f, 2.0f));
			timeline.addVector(7.0, Vector3f(4.0f, 6.0f, 3.0f));
			timeline.setTimelineLengthMode(Timeline::LengthMode::TimelineLength);
			timeline.setTimelineLength(10.0);
			timeline.reverseFromEnd();

			Assert::AreEqual(Vector3f::zero(), timeline.getCurrentVector());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f::zero(), timeline.getCurrentVector());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f(2.0f, 4.0f, 1.0f), timeline.getCurrentVector());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f(2.5f, 4.5f, 1.5f), timeline.getCurrentVector());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f(3.0f, 5.0f, 2.0f), timeline.getCurrentVector());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f(3.5f, 5.5f, 2.5f), timeline.getCurrentVector());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f(4.0f, 6.0f, 3.0f), timeline.getCurrentVector());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f(4.0f, 6.0f, 3.0f), timeline.getCurrentVector());
		}

		TEST_METHOD(InterpolatedLinearColorOneEntryReversed)
		{
			Timeline timeline;
			timeline.addLinearColor(9.0, LinearColor(0.5f, 0.3f, 1.0f));
			timeline.setTimelineLengthMode(Timeline::LengthMode::TimelineLength);
			timeline.setTimelineLength(10.0);
			timeline.reverseFromEnd();

			Assert::AreEqual(Vector3f::zero(), timeline.getCurrentLinearColor().toVector3());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f::zero(), timeline.getCurrentLinearColor().toVector3());
			timeline.updateTimeline(1.0);
			Assert::AreEqual(Vector3f(0.5f, 0.3f, 1.0f), timeline.getCurrentLinearColor().toVector3());
			timeline.updateTimeline(1.0);
			Assert::AreEqual(Vector3f(0.5f, 0.3f, 1.0f), timeline.getCurrentLinearColor().toVector3());
		}

		TEST_METHOD(InterpolatedLinearColorMultipleEntriesReversed)
		{
			Timeline timeline;
			timeline.addLinearColor(9.0, LinearColor(0.2f, 0.3f, 1.0f));
			timeline.addLinearColor(8.0, LinearColor(0.4f, 0.6f, 0.5f));
			timeline.addLinearColor(7.0, LinearColor(0.6f, 0.9f, 0.0f));
			timeline.setTimelineLengthMode(Timeline::LengthMode::TimelineLength);
			timeline.setTimelineLength(10.0);
			timeline.reverseFromEnd();

			Assert::AreEqual(Vector3f::zero(), timeline.getCurrentLinearColor().toVector3());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f::zero(), timeline.getCurrentLinearColor().toVector3());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f(0.2f, 0.3f, 1.0f), timeline.getCurrentLinearColor().toVector3());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f(0.3f, 0.45f, 0.75f), timeline.getCurrentLinearColor().toVector3());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f(0.4f, 0.6f, 0.5f), timeline.getCurrentLinearColor().toVector3());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f(0.5f, 0.75f, 0.25f), timeline.getCurrentLinearColor().toVector3());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f(0.6f, 0.9f, 0.0f), timeline.getCurrentLinearColor().toVector3());
			timeline.updateTimeline(0.5);
			Assert::AreEqual(Vector3f(0.6f, 0.9f, 0.0f), timeline.getCurrentLinearColor().toVector3());
		}
	};
}