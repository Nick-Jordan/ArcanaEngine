#include "stdafx.h"
#include "CppUnitTest.h"

#include "StringUtils.h";
#include "Timeline.h"
#include "UnitTestToString.h"

using namespace Arcana;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CoreUnitTests
{		
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
	};
}