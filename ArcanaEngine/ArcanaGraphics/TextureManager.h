#ifndef TEXTURE_MANAGER_H_
#define TEXTURE_MANAGER_H_

#include "GraphicsDefines.h"

#include "Material.h"

namespace Arcana
{

	class ARCANA_GRAPHICS_API TextureManager
	{
	public:

		class ARCANA_GRAPHICS_API Unit
		{
		public:

			Unit(GLuint unit);

			const Texture* getBoundTexture() const;

			const Sampler* getBoundSampler() const;

			bool isFree() const;

			uint32 getBindingTime() const;

			void bind(Texture* texture, const Sampler* sampler, uint32 time);

		private:

			GLuint _unit;

			Texture* _boundTexture;

			const Sampler* _boundSampler;

			uint32 _time;
		};

		static TextureManager& instance();

		void bind(Texture* texture, const Sampler* sampler, uint32 unit);

		void unbind(const Texture* texture);

		void unbind(const Sampler* sampler);

		void clearUnits();

		int32 getFreeTextureUnit(Material* material);

	private:

		TextureManager();

		~TextureManager();

	private:

		Unit* _units[MAX_TEXTURE_UNITS];

		uint32 _currentTime;
	};

}

#endif // !TEXTURE_MANAGER_H_

