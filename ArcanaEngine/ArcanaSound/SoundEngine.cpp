#include "SoundEngine.h"

namespace Arcana
{

	SoundEngine::SoundEngine()
	{
		_engine = irrklang::createIrrKlangDevice();
	}


	SoundEngine::~SoundEngine()
	{
	}

}
