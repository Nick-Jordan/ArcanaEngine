#include "stdafx.h"
#include "CppUnitTest.h"

#include "ResourceManager.h"
#include "XMLResourceDatabase.h"
#include "Texture.h"
#include "OpenGLContext.h"
#include "StringUtils.h"

using namespace Arcana;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> static std::wstring ToString<Texture::Format>(const Texture::Format& f) {
				std::string s = "RGBA";
				return std::wstring(s.begin(), s.end());
			}
			template<> static std::wstring ToString<Texture::InternalFormat>(const Texture::InternalFormat& f) {
				std::string s = "RGBA8";
				return std::wstring(s.begin(), s.end());
			}
			template<> static std::wstring ToString<Texture::PixelType>(const Texture::PixelType& f) {
				std::string s = "UnsignedByte";
				return std::wstring(s.begin(), s.end());
			}
			template<> static std::wstring ToString<Texture::Type>(const Texture::Type& f) {
				std::string s = "Texture2D";
				return std::wstring(s.begin(), s.end());
			}
			template<> static std::wstring ToString<int64>(const int64& f) {
				std::string s = StringUtils::convertInt64ToString(f);
				return std::wstring(s.begin(), s.end());
			}
		}
	}
}

namespace ResourceUnitTests
{
	namespace ResourceCreatorUnitTests
	{
		GLContext* context;
		const std::string databasePath = "C:/Users/ndjor/Documents/Visual Studio 2017/Andromeda/ArcanaEngine/UnitTests/resources/resource_database.xml";

		TEST_MODULE_INITIALIZE(Setup)
		{
			RenderSettings settings;
			settings.majorVersion = 4;
			settings.minorVersion = 5;
			settings.attributeFlags = RenderSettings::Core;
			context = OpenGLContext::create();
		}

		TEST_MODULE_CLEANUP(Cleanup)
		{
			AE_DELETE(context);
		}

		TEST_CLASS(TextureResourceCreatorUnitTests)
		{
		public:

			TEST_METHOD(TextureResourceCreator2DImage)
			{
				XMLResourceDatabase* database = XMLResourceDatabase::create(databasePath);

				ResourceManager::instance().initialize(database);

				LoadResourceTask<Texture>* task = ResourceManager::instance().loadResource<Texture>(GlobalObjectID("texture_2d_image"));
				Assert::IsNotNull(task);
				task->wait();
				
				Texture* texture = task->get();

				Assert::IsNotNull(texture);
				Assert::AreEqual(Texture::Type::Texture2D, texture->getType());
				Assert::AreEqual((uint32)308, texture->getWidth());
				Assert::AreEqual((uint32)308, texture->getHeight());
				Assert::AreEqual(Texture::Format::RGBA, texture->getFormat());
				Assert::AreEqual(Texture::InternalFormat::RGBA8, texture->getInternalFormat());
				Assert::AreEqual(Texture::PixelType::UnsignedByte, texture->getPixelType());
				Assert::AreEqual((int64)32, texture->getBitsPerPixel());
				Assert::AreEqual((uint32)4, texture->getComponents());
				Assert::IsFalse(texture->hasMipmap());
			}
		};
	}
}