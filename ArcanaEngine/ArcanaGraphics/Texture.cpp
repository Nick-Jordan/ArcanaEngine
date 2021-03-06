#include "Texture.h"

#include "TextureManager.h"
#include "Material.h"

#include "ResourceManager.h"
#include "ResourceCreator.h"

#include "FileInputStream.h"

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
		: _id(0), _type(UnknownType), 
		_format(UnknownFormat), _internalFormat(UnknownInternalFormat), 
		_pixelType(UnknownPixelType), _bitsPerPixel(0), _mipmap(false),
		_width(0), _height(0), _depth(0), _layers(0)
	{
	}

	Texture::Texture(const Texture& copy)
		: _id(copy._id), _type(copy._type),
		_format(copy._format), _internalFormat(copy._internalFormat),
		_pixelType(copy._pixelType), _bitsPerPixel(copy._bitsPerPixel), _mipmap(copy._mipmap),
		_width(copy._width), _height(copy._height), _depth(copy._depth), _layers(copy._layers)
	{
	}

	Texture::~Texture()
	{
		if (_id)
		{
			glDeleteTextures(1, &_id);
		}
	}

	Texture::Type Texture::getType() const
	{
		return _type;
	}

	GLuint Texture::getId() const
	{
		return _id;
	}

	uint32 Texture::getWidth() const
	{
		return _width;
	}

	uint32 Texture::getHeight() const
	{
		return _height;
	}

	uint32 Texture::getDepth() const
	{
		return _depth;
	}

	uint32 Texture::getLayers() const
	{
		return _layers;
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

	void Texture::unbind()
	{
		TextureManager::instance().unbind(this);
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

	void Texture::createId()
	{
		glGenTextures(1, &_id);
		AE_ASSERT(_id > 0);
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
		_id = copy._id;
		_type = copy._type;
		_format = copy._format;
		_internalFormat = copy._internalFormat;
		_pixelType = copy._pixelType;
		_bitsPerPixel = copy._bitsPerPixel;
		_mipmap = copy._mipmap;
		_width = copy._width;
		_height = copy._height;
		_depth = copy._depth;
		_layers = copy._layers;

		return *this;
	}

	class TextureParametersResource : public ResourceCreator<Texture::Parameters>
	{
	public:

		TextureParametersResource(const std::string& name, const std::string& type, const ResourceData& data, Scheduler* dependencyScheduler)
			: ResourceCreator<Texture::Parameters>(name, type, data, dependencyScheduler)
		{
			std::string swizzle = data.getStringParameter("swizzle", "rgba");
			if(swizzle.size() == 4)
				setSwizzle(swizzle[0], swizzle[1], swizzle[2], swizzle[3]);

			int32 minLevel = data.getInt32Parameter("minLevel", 0);
			setMinLevel(minLevel);

			int32 maxLevel = data.getInt32Parameter("maxLevel", 1000);
			setMaxLevel(maxLevel);

			//sampler
			TextureWrap wrapS = Texture::getTextureWrap(data.getStringParameter("wrapS", "clamptoedge"));
			setWrapS(wrapS);
			TextureWrap wrapT = Texture::getTextureWrap(data.getStringParameter("wrapT", "clamptoedge"));
			setWrapS(wrapT);
			TextureWrap wrapR = Texture::getTextureWrap(data.getStringParameter("wrapR", "clamptoedge"));
			setWrapS(wrapR);

			TextureFilter minFilter = Texture::getTextureFilter(data.getStringParameter("minFilter", "nearest"));
			setMinFilter(minFilter);
			TextureFilter magFilter = Texture::getTextureFilter(data.getStringParameter("magFilter", "linear"));
			setMagFilter(magFilter);

			//border stuffs

			float lodMin = data.getFloatParameter("lodMin", -1000.0f);
			setLodMin(lodMin);
			float lodMax = data.getFloatParameter("lodMax", 1000.0f);
			setLodMax(lodMax);
			float lodBias = data.getFloatParameter("lodBias");
			setLodBias(lodBias);

			//compare func

			float maxAnisotropy = data.getFloatParameter("maxAnisotropy", 1.0f);
			setMaxAnisotropyEXT(maxAnisotropy);
		}
	};

	class TextureResource : public ResourceCreator<Texture>
	{
	public:

		TextureResource(const std::string& name, const std::string& type, const ResourceData& data, Scheduler* dependencyScheduler)
			: ResourceCreator<Texture>(name, type, data, dependencyScheduler), type(type), _dependencyScheduler(dependencyScheduler)
		{
			const ResourceDataPoint* dataPoint = data.getDataPoint("data");

			std::string path = dataPoint ? dataPoint->StringData : "";
			bool isImage = dataPoint ? dataPoint->getBoolAttribute("image") : false;

			pixels = nullptr;

			defaultParams(name, data, format, internalFormat, pixelType, parameters, generateMipmap);

			if (type == "texture1D")
			{
				width = data.getUint32Parameter("width");
			}
			else if (type == "texture2D")
			{
				width = data.getUint32Parameter("width");
				height = data.getUint32Parameter("height");
			}
			else if (type == "texture3D")
			{
				width = data.getUint32Parameter("width");
				height = data.getUint32Parameter("height");
				depth = data.getUint32Parameter("depth");
			}
			//cube
			else if (type == "texture1DArray")
			{
				width = data.getUint32Parameter("width");
				layers = data.getUint32Parameter("layers");
			}
			else if (type == "texture2DArray")
			{
				width = data.getUint32Parameter("width");
				height = data.getUint32Parameter("height");
				layers = data.getUint32Parameter("layers");
			}
			else if (type == "textureCubeArray")
			{
				width = data.getUint32Parameter("width");
				height = data.getUint32Parameter("height");
				layers = data.getUint32Parameter("layers");
			}
			else if (type == "texture2DMultisample")
			{
				width = data.getUint32Parameter("width");
				height = data.getUint32Parameter("height");
				samples = data.getUint32Parameter("samples");
				fixedLocations = data.getBoolParameter("fixedLocations");
			}
			else if (type == "texture2DMultisampleArray")
			{
				width = data.getUint32Parameter("width");
				height = data.getUint32Parameter("height");
				layers = data.getUint32Parameter("layers");
				samples = data.getUint32Parameter("samples");
				fixedLocations = data.getBoolParameter("fixedLocations");
			}
			else if (type == "textureRectangle")
			{
				width = data.getUint32Parameter("width");
				height = data.getUint32Parameter("height");
			}
			else if (type == "textureBuffer")
			{

			}

			if (!path.empty() && path != "null")
			{
				if (!isImage)
				{
					FileInputStream stream;
					if (stream.open(path))
					{
						pixels = new uint8[stream.size()];
						stream.read(pixels, stream.size());
					}
				}
				else if (type == "texture2D" || type == "textureRectangle")
				{
					Image<uint8> image;
					image.init(path);
					uint64 size = image.getWidth() * image.getHeight() * (image.getFormat() == ImageFormat::RGBA ? 4 : 3);
					pixels = new uint8[size];
					memcpy(pixels, image.getPixelsPtr(), size);

					width = width ? width : image.getWidth();
					height = height ? height : image.getHeight();
				}
			}
		}

		virtual void syncInitialize() override
		{
			if (type == "texture1D")
			{
				initialize1D(format, width, internalFormat, pixelType, pixels, parameters, generateMipmap);
			}
			else if (type == "texture2D")
			{

				initialize2D(format, width, height, internalFormat, pixelType, pixels, parameters, generateMipmap);
			}
			else if (type == "texture3D")
			{
				initialize3D(format, width, height, depth, internalFormat, pixelType, pixels, parameters, generateMipmap);
			}
			//cube
			else if (type == "texture1DArray")
			{
				initialize1DArray(format, width, layers, internalFormat, pixelType, pixels, parameters, generateMipmap);
			}
			else if (type == "texture2DArray")
			{
				initialize2DArray(format, width, height, layers, internalFormat, pixelType, pixels, parameters, generateMipmap);
			}
			else if (type == "textureCubeArray")
			{
				initializeCubeArray(format, width, height, layers, internalFormat, pixelType, pixels, parameters, generateMipmap);
			}
			else if (type == "texture2DMultisample")
			{
				initialize2DMultisample(format, width, height, samples, internalFormat, pixelType, fixedLocations);
			}
			else if (type == "texture2DMultisampleArray")
			{
				initialize2DMultisampleArray(format, width, height, layers, samples, internalFormat, pixelType, fixedLocations);
			}
			else if (type == "textureRectangle")
			{
				initializeRectangle(format, width, height, internalFormat, pixelType, pixels, parameters, generateMipmap);
			}
			else if (type == "textureBuffer")
			{

			}

			AE_DELETE_ARRAY(pixels);
		}

	private:

		Format format;
		uint32 width;
		uint32 height;
		uint32 depth;
		uint32 layers;
		uint32 samples;
		bool fixedLocations;
		InternalFormat internalFormat;
		PixelType pixelType;
		Parameters parameters;
		void* pixels;
		bool generateMipmap;
		std::string type;

		Scheduler* _dependencyScheduler;

		void defaultParams(const std::string& name, const ResourceData& data, Format& format, InternalFormat& internalFormat, PixelType& pixelType, Texture::Parameters& parameters, bool& generateMipmap)
		{
			format = Texture::getTextureFormat(data.getStringParameter("format"));
			internalFormat = Texture::getTextureInternalFormat(data.getStringParameter("internalFormat"));
			pixelType = Texture::getTexturePixelType(data.getStringParameter("pixelType"));

			const ResourceData* params = data.getAdditionalData("parameters");
			if (!params)
			{
				params = data.getAdditionalData("params");
			}

			if (params)
			{
				LoadResourceTask<Texture::Parameters>* buildTask = ResourceManager::instance().buildResource<Texture::Parameters>(GlobalObjectID(name + "::parameters"), "textureParameters", *params, _dependencyScheduler);
				buildTask->wait();
				parameters = *buildTask->get();
			}

			generateMipmap = data.getBoolParameter("mipmap");
		}

		void initialize1D(Format format, uint32 width, InternalFormat iformat, PixelType pixelType,
			const void* pixels, const Parameters& parameters, bool generateMipmap)
		{
			Texture* texture = Texture::create1D(format, width, iformat, pixelType, pixels, parameters, generateMipmap);
			if (texture)
			{
				(Texture&)*this = *texture;
				texture->release();
			}
		}

		void initialize2D(Format format, uint32 width, uint32 height, InternalFormat iformat, PixelType pixelType,
			const void* pixels, const Parameters& parameters, bool generateMipmap)
		{
			Texture* texture = Texture::create2D(format, width, height, iformat, pixelType, pixels, parameters, generateMipmap);

			if (texture)
			{
				(Texture&)*this = *texture;

				//texture->release();
			}
		}

		void initialize3D(Format format, uint32 width, uint32 height, uint32 depth, InternalFormat iformat, PixelType pixelType,
			const void* pixels, const Parameters& parameters, bool generateMipmap)
		{
			Texture* texture = Texture::create3D(format, width, height, depth, iformat, pixelType, pixels, parameters, generateMipmap);
			if (texture)
			{
				(Texture&)*this = *texture;
				texture->release();
			}
		}

		void initializeCube(Format format, uint32 width, uint32 height, InternalFormat iformat, PixelType pixelType,
			const void* pixels[6], const Parameters& parameters, bool generateMipmap)
		{
			Texture* texture = Texture::createCube(format, width, height, iformat, pixelType, pixels, parameters, generateMipmap);
			if (texture)
			{
				(Texture&)*this = *texture;
				texture->release();
			}
		}

		void initialize1DArray(Format format, uint32 width, uint32 layers, InternalFormat iformat, PixelType pixelType,
			const void* pixels, const Parameters& parameters, bool generateMipmap)
		{
			Texture* texture = Texture::create1DArray(format, width, layers, iformat, pixelType, pixels, parameters, generateMipmap);
			if (texture)
			{
				(Texture&)*this = *texture;
				texture->release();
			}
		}

		void initialize2DArray(Format format, uint32 width, uint32 height, uint32 layers, InternalFormat iformat, PixelType pixelType,
			const void* pixels, const Parameters& parameters, bool generateMipmap)
		{
			Texture* texture = Texture::create2DArray(format, width, height, layers, iformat, pixelType, pixels, parameters, generateMipmap);
			if (texture)
			{
				(Texture&)*this = *texture;
				texture->release();
			}
		}

		void initializeCubeArray(Format format, uint32 width, uint32 height, uint32 layers, InternalFormat iformat, PixelType pixelType,
			const void* pixels, const Parameters& parameters, bool generateMipmap)
		{
			Texture* texture = Texture::createCubeArray(format, width, height, layers, iformat, pixelType, pixels, parameters, generateMipmap);
			if (texture)
			{
				(Texture&)*this = *texture;
				texture->release();
			}
		}

		void initialize2DMultisample(Format format, uint32 width, uint32 height, uint32 samples, InternalFormat iformat, 
			PixelType pixelType, bool fixedLocations)
		{
			Texture* texture = Texture::create2DMultisample(format, width, height, samples, iformat, pixelType, fixedLocations);
			if (texture)
			{
				(Texture&)*this = *texture;
				texture->release();
			}
		}

		void initialize2DMultisampleArray(Format format, uint32 width, uint32 height, uint32 layers, uint32 samples, InternalFormat iformat, 
			PixelType pixelType, bool fixedLocations)
		{
			Texture* texture = Texture::create2DMultisampleArray(format, width, height, layers, samples, iformat, pixelType, fixedLocations);
			if (texture)
			{
				(Texture&)*this = *texture;
				texture->release();
			}
		}

		void initializeRectangle(Format format, uint32 width, uint32 height, InternalFormat iformat, PixelType pixelType,
			const void* pixels, const Parameters& parameters, bool generateMipmap)
		{
			Texture* texture = Texture::createRectangle(format, width, height, iformat, pixelType, pixels, parameters, generateMipmap);
			if (texture)
			{
				(Texture&)*this = *texture;
				texture->release();
			}
		}

		void initializeBuffer()
		{
			Texture* texture = Texture::createBuffer();
			if (texture)
			{
				(Texture&)*this = *texture;
				texture->release();
			}
		}
	};

	Resource::Type<TextureParametersResource> textureParametersResource("textureParameters");
	Resource::Type<TextureResource, true> texture1DResource("texture1D");
	Resource::Type<TextureResource, true> texture2DResource("texture2D");
	Resource::Type<TextureResource, true> texture3DResource("texture3D");
	Resource::Type<TextureResource, true> textureCubeResource("textureCube");
	Resource::Type<TextureResource, true> texture1DArrayResource("texture1DArray");
	Resource::Type<TextureResource, true> texture2DArrayResource("texture2DArray");
	Resource::Type<TextureResource, true> textureCubeArrayResource("textureCubeArray");
	Resource::Type<TextureResource, true> texture2DMultisampleResource("texture2DMultisample");
	Resource::Type<TextureResource, true> texture2DMultisampleArrayResource("texture2DMultisampleArray");
	Resource::Type<TextureResource, true> textureRectangleResource("textureRectangle");
	Resource::Type<TextureResource, true> textureBufferResource("textureBuffer");
}
