#ifndef POST_PROCESS_EFFECT_H_
#define POST_PROCESS_EFFECT_H_

#include "GraphicsDefines.h"

#include "Texture.h"
#include "Shader.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API PostProcessEffect
	{
	public:
		
		PostProcessEffect(const std::string& frag);

		virtual ~PostProcessEffect();

		virtual void apply(Texture* texture);

		virtual void initialize();

		void enable();

		void disable();

		void toggleEffect();

		bool isEnabled() const;

	private:

		Shader* _shader;
		std::string _fragment;

		bool _enabled;

	public://test
		std::vector<Texture*> _extraTextures;
		std::vector<Texture*> _clearedExtraTextures;
	};
}

#endif // !POST_PROCESS_EFFECT_H_
