#ifndef POST_PROCESS_EFFECT_H_
#define POST_PROCESS_EFFECT_H_

#include "GraphicsDefines.h"

#include "Texture.h"
#include "Shader.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API PostProcessEffect
	{
		friend class PostProcessQueue;

	public:
		
		PostProcessEffect(const std::string& name, const std::string& frag);

		virtual ~PostProcessEffect();

		void begin(Texture* texture);

		virtual void apply(Texture* texture);

		void end(Texture* texture);

		virtual void initialize();

		bool isEnabled() const;

		const std::string& getName() const;

	private:

		void enable();

		void disable();

		void toggleEffect();

	protected:

		Shader* _shader;

	private:

		std::string _fragment;
		std::string _name;

		bool _enabled;
		bool _defaultEffect;

	public://test
		std::vector<Texture*> _clearedExtraTextures;
	};
}

#endif // !POST_PROCESS_EFFECT_H_
