#ifndef SOUND_ENGINE_H_
#define SOUND_ENGINE_H_

#include "SoundDefines.h"

#include "Object.h"
#include "irrKlang/irrKlang.h"

namespace Arcana
{
	class ARCANA_SOUND_API SoundEngine : public Object
	{
	public:

		SoundEngine();

		~SoundEngine();

		//bool play2DSound(const Sound& sound, );
	
	public:

		irrklang::ISoundEngine* _engine;
	};
}

#endif // !SOUND_ENGINE_H_

