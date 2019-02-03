#include "Texture.h"

#include "TextureInstance.h"
#include "TextureManager.h"
#include "Material.h"

#include "ResourceManager.h"
#include "ResourceCreator.h"

#include <algorithm>

namespace Arcana
{
	Texture::Parameters::Parameters() : Sampler::Parameters(),
		_minLevel(0), _maxLevel(1000)
	{
		_swizzle[0] = 'r';
		_swizzle[1] = 'g';
		_swizzle[2] = 'b';
		_swizzle[3] = 'a';
	}

	Texture::Parameters::~Parameters()
	{
	}

	const char* Texture::Parameters::getSwizzle() const
	{
		return _swizzle;
	}

	GLint Texture::Parameters::getMinLevel() const
	{
		return _minLevel;
	}

	GLint Texture::Parameters::getMaxLevel() const
	{
		return _maxLevel;
	}

	void Texture::Parameters::setSwizzle(char r, char g, char b, char a)
	{
		_swizzle[0] = r;
		_swizzle[1] = g;
		_swizzle[2] = b;
		_swizzle[3] = a;
	}

	void Texture::Parameters::setMinLevel(GLint minLevel)
	{
		_minLevel = minLevel;
	}

	void Texture::Parameters::setMaxLevel(GLint maxLevel)
	{
		_maxLevel = maxLevel;
	}

	void Texture::Parameters::set(Type type) const
	{
		glTexParameteri(type, GL_TEXTURE_WRAP_S, getWrapS());
		glTexParameteri(type, GL_TEXTURE_WRAP_T, getWrapT());
		glTexParameteri(type, GL_TEXTURE_WRAP_R, getWrapR());
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, getMinFilter());
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, getMagFilter());
		switch (getBorderType()) 
		{
		case 0: // i
			glTexParameteriv(type, GL_TEXTURE_BORDER_COLOR, getBorderi());
			break;
		case 1: // f
			glTexParameterfv(type, GL_TEXTURE_BORDER_COLOR, getBorderf());
			break;
		case 2: // Ii
			glTexParameterIiv(type, GL_TEXTURE_BORDER_COLOR, getBorderIi());
			break;
		case 3: // Iui
			glTexParameterIuiv(type, GL_TEXTURE_BORDER_COLOR, getBorderIui());
			break;
		default:
			AE_ASSERT(false);
		}
		if (type != GL_TEXTURE_RECTANGLE) 
		{
			glTexParameterf(type, GL_TEXTURE_MIN_LOD, getLodMin());
			glTexParameterf(type, GL_TEXTURE_MAX_LOD, getLodMax());
		}

		glTexParameterf(type, GL_TEXTURE_LOD_BIAS, getLodBias());
		if (getCompareFunc() != RenderState::Always)
		{
			glTexParameteri(type, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
			glTexParameteri(type, GL_TEXTURE_COMPARE_FUNC, getCompareFunc());
		}
		glTexParameterf(type, GL_TEXTURE_MAX_ANISOTROPY_EXT, getMaxAnisotropyEXT());
		glTexParameteri(type, GL_TEXTURE_SWIZZLE_R, getTextureSwizzle(getSwizzle()[0]));
		glTexParameteri(type, GL_TEXTURE_SWIZZLE_G, getTextureSwizzle(getSwizzle()[1]));
		glTexParameteri(type, GL_TEXTURE_SWIZZLE_B, getTextureSwizzle(getSwizzle()[2]));
		glTexParameteri(type, GL_TEXTURE_SWIZZLE_A, getTextureSwizzle(getSwizzle()[3]));
		if (type != GL_TEXTURE_RECTANGLE) 
		{
			glTexParameteri(type, GL_TEXTURE_BASE_LEVEL, getMinLevel());
			glTexParameteri(type, GL_TEXTURE_MAX_LEVEL, getMaxLevel());
		}
	}


	Texture::Texture() 
		: _instance(nullptr), _type(UnknownType), 
		_format(UnknownFormat), _internalFormat(UnknownInternalFormat), 
		_pixelType(UnknownPixelType), _bitsPerPixel(0), _mipmap(false)
	{
	}

	Texture::Texture(const Texture& copy)
		: _instance(copy._instance), _type(copy._type),
		_format(copy._format), _internalFormat(copy._internalFormat),
		_pixelType(copy._pixelType), _bitsPerPixel(copy._bitsPerPixel), _mipmap(copy._mipmap)
	{
		_instance->reference();
	}

	Texture::~Texture()
	{
		if (_instance)
		{
			AE_RELEASE(_instance);
		}
	}

	Texture::Type Texture::getType() const
	{
		return _type;
	}

	GLuint Texture::getId() const
	{
		if (_instance)
			return _instance->getId();

		return 0;
	}

	uint32 Texture::getWidth() const
	{
		if (_instance)
			return _instance->getWidth();

		return 0;
	}

	uint32 Texture::getHeight() const
	{
		if (_instance)
			return _instance->getHeight();

		return 0;
	}

	uint32 Texture::getDepth() const
	{
		if (_instance)
			return _instance->getDepth();

		return 0;
	}

	uint32 Texture::getLayers() const
	{
		if(_instance)
			return _instance->getLayers();

		return 0;
	}

	bool Texture::isCompressed() const
	{
		return _internalFormat == COMPRESSED_SIGNED_RED_RGTC1
			|| _internalFormat == COMPRESSED_RG_RGTC2
			|| _internalFormat == COMPRESSED_SIGNED_RG_RGTC2
			|| _internalFormat == COMPRESSED_RGBA_BPTC_UNORM_ARB
			|| _internalFormat == COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB
			|| _internalFormat == COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB
			|| _internalFormat == COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB
			|| _internalFormat == COMPRESSED_RGB_S3TC_DXT1_EXT
			|| _internalFormat == COMPRESSED_RGBA_S3TC_DXT1_EXT
			|| _internalFormat == COMPRESSED_RGBA_S3TC_DXT3_EXT;
	}

	Texture::Format Texture::getFormat() const
	{
		return _format;
	}

	Texture::InternalFormat Texture::getInternalFormat() const
	{
		return _internalFormat;
	}

	Texture::PixelType Texture::getPixelType() const
	{
		return _pixelType;
	}

	int64 Texture::getBitsPerPixel() const
	{
		return _bitsPerPixel;
	}

	uint32 Texture::getComponents() const
	{
		switch (_format)
		{
		case Alpha:
		case Depth:
		case Red:
		case Green:
		case Blue:
		case RedInteger:
		case BlueInteger:
		case GreenInteger:
			return 1;
		case RG:
		case RGInteger:
			return 2;
		case RGB:
		case RGB565:
		case BGR:
		case RGBInteger:
		case BGRInteger:
			return 3;
		case RGBA:
		case RGBA4444:
		case RGBA5551:
		case BGRA:
		case RGBAInteger:
		case BGRAInteger:
			return 4;
		case StencilIndex:
			return 0;
		case DepthStencil:
			return 0;
		default:
			return 0;
		}
	}

	bool Texture::hasMipmap() const
	{
		return _mipmap;
	}

	bool Texture::setMipmap(bool mipmap)
	{
		if (mipmap)
		{
			return generateMipmap();
		}
		
		return invalidateMipmap();
	}

	int32 Texture::bind(Material* material, Sampler* sampler)
	{
		GLuint samplerId = sampler == nullptr ? 0 : sampler->getId();
		std::map<uint32, int32>::iterator i = _currentTextureUnits.find(samplerId);

		int32 unit;
		if (i == _currentTextureUnits.end()) 
		{
			unit = TextureManager::instance().getFreeTextureUnit(material);
		}
		else 
		{
			unit = i->second;
		}

		TextureManager::instance().bind(this, sampler, unit);

		return unit;
	}

	int32 Texture::getMaxTextureUnits()
	{
		static int32 maxTextureUnits = 0;

		if (maxTextureUnits == 0) {

			GLint maxVertexTextureImageUnits;
			GLint maxFragmentTextureImageUnits;
			GLint maxCombinedTextureImageUnits;

			glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &maxVertexTextureImageUnits);
			glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxFragmentTextureImageUnits);
			glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxCombinedTextureImageUnits);

			maxTextureUnits = (std::min)(maxCombinedTextureImageUnits, MAX_TEXTURE_UNITS);
		}
		return maxTextureUnits;
	}


	bool Texture::generateMipmap()
	{
		if (!getId() || hasMipmap())
		{
			return false;
		}

		glBindTexture(_type, getId());
		glGenerateMipmap(_type);

		glTexParameteri(_type, GL_TEXTURE_MIN_FILTER, _parameters.getMinFilter() == Linear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR);

		_mipmap = true;

		return true;
	}

	bool Texture::invalidateMipmap()
	{
		if (!_mipmap)
			return false;

		glBindTexture(_type, getId());

		glTexParameteri(_type, GL_TEXTURE_MIN_FILTER, _parameters.getMinFilter());

		_mipmap = false;

		return true;
	}

	void Texture::addCurrentBinding(uint32 samplerId, int32 unit)
	{
		_currentTextureUnits.insert(std::make_pair(samplerId, unit));
	}

	void Texture::removeCurrentBinding(uint32 samplerId)
	{
		std::map<uint32, int32>::iterator i = _currentTextureUnits.find(samplerId);
		if (i != _currentTextureUnits.end());
		{
			_currentTextureUnits.erase(i);
		}
	}

	int64 Texture::getFormatBitsPerPixel(Texture::Format format)
	{
		switch (format)
		{
		case Alpha:
		case Depth:
		case Red:
		case Green:
		case Blue:
		case RedInteger:
		case BlueInteger:
		case GreenInteger:
			return 8;
		case RGB565:
		case RGBA4444:
		case RGBA5551:
		case RG:
		case RGInteger:
			return 16;
		case RGB:
		case BGR:
		case RGBInteger:
		case BGRInteger:
			return 24;
		case RGBA:
		case BGRA:
		case RGBAInteger:
		case BGRAInteger:
			return 32;
		case StencilIndex:
			return -1;
		case DepthStencil:
			return -1;
		defaut:
			return -1;
		}
	}

	GLenum Texture::getTextureSwizzle(char s)
	{
		switch (s) {
		case 'r':
			return GL_RED;
		case 'g':
			return GL_GREEN;
		case 'b':
			return GL_BLUE;
		case 'a':
			return GL_ALPHA;
		case '0':
			return GL_ZERO;
		case '1':
			return GL_ONE;
		default:
			return 0;
		}
	}


	Texture& Texture::operator=(const Texture& copy)
	{
		_instance = copy._instance;
		_type = copy._type;
		_format = copy._format;
		_internalFormat = copy._internalFormat;
		_pixelType = copy._pixelType;
		_bitsPerPixel = copy._bitsPerPixel;
		_mipmap = copy._mipmap;

		_instance->reference();

		return *this;
	}

	class Texture2DResource : public ResourceCreator<Texture>
	{
	public:

		Texture2DResource(const std::string& name, const std::string& type, const ResourceData& data)
			: ResourceCreator<Texture>(name, type, data)
		{
			LOG(Warning, CoreEngine, "Loading texture: " + name);
			Format format = Texture::RGBA;//Texture::getTextureFormat(data.getStringParameter("format"));
			uint32 width = data.getUint32Parameter("width");
			uint32 height = data.getUint32Parameter("height");
			InternalFormat iformat = Texture::RGBA8;//Texture::getTextureInternalFormat(data.getStringParameter("internal_format"));
			PixelType pixelType = Texture::UnsignedByte;//Texture::getTexturePixelType(data.getStringParameter("pixel_type"));
			Texture::Parameters parameters; // READ PARAMETERS
			parameters.setWrapS(TextureWrap::Repeat);
			parameters.setWrapT(TextureWrap::Repeat);
			bool generateMipmap = data.getBoolParameter("mipmap");

			std::string path = data.getStringParameter("data");

			//TEST
			Image<uint8> image;
			image.init(path);

			const void* pixels = image.getPixelsPtr();//nullptr; // READ BYTES

			initialize(format, width ? width : image.getWidth(), height ? height : image.getHeight(), iformat, pixelType, pixels, parameters, generateMipmap);
		}

	private:

		void initialize(Format format, uint32 width, uint32 height, InternalFormat iformat, PixelType pixelType,
			const void* pixels, const Parameters& parameters, bool generateMipmap)
		{
			Texture* texture = Texture::create2D(format, width, height, iformat, pixelType, pixels, parameters, generateMipmap);
			if (texture)
			{
				(Texture&)*this = *texture;
				texture->release();
			}
		}
	};

	Resource::Type<Texture2DResource> texture2DResource("texture2D");
}
