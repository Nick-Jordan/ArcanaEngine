#include "ProceduralTexture.h"

namespace Arcana
{
	ProceduralTexture::ProceduralTexture()
	{
	}
		
	ProceduralTexture::~ProceduralTexture()
	{
		
	}
		
	Texture* ProceduralTexture::buildTexture()
	{
		Texture::Type type;
		const void* pixels = nullptr;
		
		const TextureCacheObjectParameters& parameters = getParameters();
		
		//get type;
		
		switch(type)
		{
		case Texture::Texture1D:
		
			return Texture::create1D(parameters.format, parameters.width, parameters.internalFormat, 
				parameters.pixelType, pixels, parameters.parameters, parameters.generateMipmap);
				
		case Texture::Texture2D:
		
			return Texture::create2D(parameters.format, parameters.width, parameters.height, parameters.internalFormat, 
				parameters.pixelType, pixels, parameters.parameters, parameters.generateMipmap);
				
		case Texture::Texture3D:
		
			return Texture::create3D(parameters.format, parameters.width, parameters.height, parameters.depth, parameters.internalFormat, 
				parameters.pixelType, pixels, parameters.parameters, parameters.generateMipmap);
				
		case Texture::TextureCube:
		
			return nullptr;
			
		case Texture::Texture1DArray:
		
			return Texture::create1DArray(parameters.format, parameters.width, parameters.layers, parameters.internalFormat, 
				parameters.pixelType, pixels, parameters.parameters, parameters.generateMipmap);
				
		case Texture::Texture2DArray:
		
			return Texture::create2DArray(parameters.format, parameters.width, parameters.height, parameters.layers, parameters.internalFormat, 
				parameters.pixelType, pixels, parameters.parameters, parameters.generateMipmap);
				
		case Texture::TextureCubeArray:
		
			return Texture::createCubeArray(parameters.format, parameters.width, parameters.height, parameters.layers, parameters.internalFormat, 
				parameters.pixelType, pixels, parameters.parameters, parameters.generateMipmap);
				
		case Texture::Texture2DMultisample:
		
			return nullptr;
			
		case Texture::Texture2DMultisampleArray:
		
			return nullptr;
			
		case Texture::TextureRectangle:
		
			return Texture::createRectangle(parameters.format, parameters.width, parameters.height, parameters.internalFormat, 
				parameters.pixelType, pixels, parameters.parameters, parameters.generateMipmap);
				
		case Texture::TextureBuffer:
		
			return nullptr;
			
		default:
		
			return nullptr;
		};
	}
		
	ProceduralTexture* ProceduralTexture::create(const Parameters& params, const TextureCacheObjectParameters& textureParams)
	{
		setParameters(textureParams);
		
		return nullptr;
	}
}