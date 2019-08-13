#include "stdafx.h"
#include "CppUnitTest.h"

#include "ResourceManager.h"
#include "XMLResourceDatabase.h"
#include "Material.h"
#include "OpenGLContext.h"
#include "StringUtils.h"

#include "UnitTestToString.h"

using namespace Arcana;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ResourceUnitTests
{
	/*namespace ResourceCreatorUnitTests
	{
		GLContext* context;
		const std::string databasePath = "C:/Users/ndjor/Documents/Visual Studio 2017/Andromeda/ArcanaEngine/UnitTests/resources/resource_database.xml";
		XMLResourceDatabase* database = nullptr;

		TEST_MODULE_INITIALIZE(Setup)
		{
			RenderSettings settings;
			settings.majorVersion = 4;
			settings.minorVersion = 5;
			settings.attributeFlags = RenderSettings::Core;
			context = OpenGLContext::create();

			database = XMLResourceDatabase::create(databasePath);
			ResourceManager::instance().initialize(database);
		}

		TEST_MODULE_CLEANUP(Cleanup)
		{
			AE_DELETE(context);
			AE_RELEASE(database);
		}

		TEST_CLASS(MaterialResourceCreatorUnitTests)
		{
		public:

			TEST_METHOD(MaterialResourceNotFound)
			{
				LoadResourceTask<Material>* task = ResourceManager::instance().loadResource<Material>(GlobalObjectID("material_not_found"));
				Assert::IsNotNull(task);
				task->wait();

				Material* material = task->get();
				Assert::IsNull(material);
			}
		};

		TEST_CLASS(TextureResourceCreatorUnitTests)
		{
		public:

			TEST_METHOD(TextureResourceNotFound)
			{
				LoadResourceTask<Texture>* task = ResourceManager::instance().loadResource<Texture>(GlobalObjectID("texture_not_found"));
				Assert::IsNotNull(task);
				task->wait();

				Texture* texture = task->get();

				Assert::IsNull(texture);
			}

			TEST_METHOD(TextureResourceCreator1D)
			{
				LoadResourceTask<Texture>* task = ResourceManager::instance().loadResource<Texture>(GlobalObjectID("texture_1d"));
				Assert::IsNotNull(task);
				task->wait();

				Texture* texture = task->get();

				Assert::IsNotNull(texture);
				Assert::AreEqual(Texture::Type::Texture1D, texture->getType());
				Assert::AreEqual((uint32)100, texture->getWidth());
				Assert::AreEqual(Texture::Format::RGBA, texture->getFormat());
				Assert::AreEqual(Texture::InternalFormat::RGBA8, texture->getInternalFormat());
				Assert::AreEqual(Texture::PixelType::UnsignedByte, texture->getPixelType());
				Assert::AreEqual((int64)32, texture->getBitsPerPixel());
				Assert::AreEqual((uint32)4, texture->getComponents());
				Assert::IsFalse(texture->hasMipmap());
			}

			TEST_METHOD(TextureResourceCreator2D)
			{
				LoadResourceTask<Texture>* task = ResourceManager::instance().loadResource<Texture>(GlobalObjectID("texture_2d"));
				Assert::IsNotNull(task);
				task->wait();

				Texture* texture = task->get();

				Assert::IsNotNull(texture);
				Assert::AreEqual(Texture::Type::Texture2D, texture->getType());
				Assert::AreEqual((uint32)100, texture->getWidth());
				Assert::AreEqual((uint32)100, texture->getHeight());
				Assert::AreEqual(Texture::Format::RGBA, texture->getFormat());
				Assert::AreEqual(Texture::InternalFormat::RGBA8, texture->getInternalFormat());
				Assert::AreEqual(Texture::PixelType::UnsignedByte, texture->getPixelType());
				Assert::AreEqual((int64)32, texture->getBitsPerPixel());
				Assert::AreEqual((uint32)4, texture->getComponents());
				Assert::IsFalse(texture->hasMipmap());
			}
			TEST_METHOD(TextureResourceCreator2DImage)
			{
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

			TEST_METHOD(TextureResourceCreator2DBin)
			{
				LoadResourceTask<Texture>* task = ResourceManager::instance().loadResource<Texture>(GlobalObjectID("texture_2d_bin"));
				Assert::IsNotNull(task);
				task->wait();

				Texture* texture = task->get();

				Assert::IsNotNull(texture);
				Assert::AreEqual(Texture::Type::Texture2D, texture->getType());
				Assert::AreEqual((uint32)64, texture->getWidth());
				Assert::AreEqual((uint32)16, texture->getHeight());
				Assert::AreEqual(Texture::Format::RGBA, texture->getFormat());
				Assert::AreEqual(Texture::InternalFormat::RGBA32F, texture->getInternalFormat());
				Assert::AreEqual(Texture::PixelType::Float, texture->getPixelType());
				Assert::AreEqual((int64)32, texture->getBitsPerPixel());
				Assert::AreEqual((uint32)4, texture->getComponents());
				Assert::IsFalse(texture->hasMipmap());
			}

			TEST_METHOD(TextureResourceCreator3D)
			{
				LoadResourceTask<Texture>* task = ResourceManager::instance().loadResource<Texture>(GlobalObjectID("texture_3d"));
				Assert::IsNotNull(task);
				task->wait();

				Texture* texture = task->get();

				Assert::IsNotNull(texture);
				Assert::AreEqual(Texture::Type::Texture3D, texture->getType());
				Assert::AreEqual((uint32)256, texture->getWidth()); 
				Assert::AreEqual((uint32)128, texture->getHeight()); 
				Assert::AreEqual((uint32)32, texture->getDepth());
				Assert::AreEqual(Texture::Format::RGBA, texture->getFormat());
				Assert::AreEqual(Texture::InternalFormat::RGBA32F, texture->getInternalFormat());
				Assert::AreEqual(Texture::PixelType::Float, texture->getPixelType());
				Assert::AreEqual((int64)32, texture->getBitsPerPixel());
				Assert::AreEqual((uint32)4, texture->getComponents());
				Assert::IsFalse(texture->hasMipmap());
			}
		};
	}*/
}