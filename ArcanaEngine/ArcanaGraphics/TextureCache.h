#ifndef TEXTURE_CACHE_H_
#define TEXTURE_CACHE_H_

#include "GraphicsDefines.h"

#include "TextureCacheObject.h"
#include "Texture.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API TextureCache : public Object
	{
	public:
	
		TextureCache();
		
		virtual ~TextureCache();
		
		
		void addUnbuiltTextureObject(std::string textureName, TextureCacheObject* object);
		
		void removeUnbuiltTextureObject(TextureCacheObject* object);

		void removeUnbuiltTextureObject(std::string textureName);
		
		void addTexture(std::string textureName, Texture* texture);
		
		void removeTexture(Texture* texture);

		void removeTexture(std::string name);
		
		void buildUnbuiltObjects();
		
		Texture* get(std::string name);
		
	private:
	
		std::map<std::string, TextureCacheObject*> _unbuiltTextures;
		
		std::map<std::string, Texture*> _cachedTextures;
	};
}

#endif // !TEXTURE_CACHE_H_