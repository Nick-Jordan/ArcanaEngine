#include "TextureCache.h"

namespace Arcana
{
	TextureCache::TextureCache()
	{
	}
		
	TextureCache::~TextureCache()
	{
		std::map<std::string, TextureCacheObject*>::iterator iter;
		for (iter = _unbuiltTextures.begin(); iter != _unbuiltTextures.end(); iter++)
		{
			AE_RELEASE(iter->second);
		}
		
		std::map<std::string, Texture*>::iterator iter2;
		for (iter2 = _cachedTextures.begin(); iter2 != _cachedTextures.end(); iter2++)
		{
			AE_RELEASE(iter->second);
		}
		
		_unbuiltTextures.clear();
		_cachedTextures.clear();
	}
				
	void TextureCache::addUnbuiltTextureObject(std::string textureName, TextureCacheObject* object)
	{
		_unbuiltTextures.insert(std::make_pair(textureName, object));
		object->reference();
	}
		
	void TextureCache::removeUnbuiltTextureObject(TextureCacheObject* object)
	{
		std::map<std::string, TextureCacheObject*>::iterator iter;
		for(iter = _unbuiltTextures.begin(); iter != _unbuiltTextures.end(); iter++)
		{
			if (iter->second == object)
				break;
		}

		if (iter != _unbuiltTextures.end())
		{
			_unbuiltTextures.erase(iter);
			AE_RELEASE(object);
		}
	}

	void TextureCache::removeUnbuiltTextureObject(std::string textureName)
	{
		std::map<std::string, TextureCacheObject*>::iterator iter = _unbuiltTextures.find(textureName);

		if (iter != _unbuiltTextures.end())
		{
			TextureCacheObject* object = iter->second;
			_unbuiltTextures.erase(iter);
			AE_RELEASE(object);
		}
	}
		
	void TextureCache::addTexture(std::string name, Texture* texture)
	{
		_cachedTextures.insert(std::make_pair(name, texture));
		texture->reference();
	}
		
	void TextureCache::removeTexture(Texture* texture)
	{
		std::map<std::string, Texture*>::iterator iter;
		for (iter = _cachedTextures.begin(); iter != _cachedTextures.end(); iter++)
		{
			if (iter->second == texture)
				break;
		}

		if (iter != _cachedTextures.end())
		{
			_cachedTextures.erase(iter);
			AE_RELEASE(texture);
		}
	}

	void TextureCache::removeTexture(std::string textureName)
	{
		std::map<std::string, Texture*>::iterator iter = _cachedTextures.find(textureName);

		if (iter != _cachedTextures.end())
		{
			Texture* texture = iter->second;
			_cachedTextures.erase(iter);
			AE_RELEASE(texture);
		}
	}
	
	void TextureCache::buildUnbuiltObjects()
	{
		std::map<std::string, TextureCacheObject*>::iterator iter;
		for (iter = _unbuiltTextures.begin(); iter != _unbuiltTextures.end(); iter++)
		{
			TextureCacheObject* object = iter->second;

			_cachedTextures.insert(std::make_pair(iter->first, object->buildTexture()));
		}

		for (iter = _unbuiltTextures.begin(); iter != _unbuiltTextures.end(); iter++)
		{
			AE_RELEASE(iter->second);
		}
		
		_unbuiltTextures.clear();
	}

	Texture* TextureCache::get(std::string name)
	{
		std::map<std::string, Texture*>::iterator iter = _cachedTextures.find(name);

		if (iter != _cachedTextures.end())
		{
			iter->second->reference();
			return iter->second;
		}

		return nullptr;
	}
}