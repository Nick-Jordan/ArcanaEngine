#ifndef TEXTURE_INSTANCE_H_
#define TEXTURE_INSTANCE_H_

#include "GraphicsDefines.h"

#include "Object.h"
#include "Texture.h"

#include "opengl/include.h"

namespace Arcana
{

	class ARCANA_GRAPHICS_API TextureInstance : public Object
	{
	public:

		TextureInstance();

		virtual ~TextureInstance();

		GLuint getId() const;

		virtual uint32 getWidth() const;

		virtual uint32 getHeight() const;

		virtual uint32 getDepth() const;

		virtual uint32 getLayers() const;

	protected:

		void initialize(Texture::Type type, Texture::InternalFormat iformat, const Texture::Parameters &params);

	protected:

		GLuint _id;
	};

}

#endif // !TEXTURE_INSTANCE_H_

