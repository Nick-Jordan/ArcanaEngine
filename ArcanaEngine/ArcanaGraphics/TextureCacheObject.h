#ifndef TEXTURE_CACHE_OBJECT_H_
#define TEXTURE_CACHE_OBJECT_H_

#include "GraphicsDefines.h"

#include "Object.h"

#include "Texture.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API TextureCacheObjectParameters;
	
	class ARCANA_GRAPHICS_API TextureCacheObject : public Object
	{
	public:
	
		TextureCacheObject();
		
		virtual ~TextureCacheObject();
		
		virtual Texture* buildTexture() = 0;
		
		const TextureCacheObjectParameters& getParameters() const;
		
		void setParameters(const TextureCacheObjectParameters& parameters);
		
	private:
	
		TextureCacheObjectParameters& _parameters;
	};
}

#endif // !TEXTURE_CACHE_OBJECT_H_