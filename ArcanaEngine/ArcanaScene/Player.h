#ifndef PLAYER_H_
#define PLAYER_H_

#include "SceneDefines.h"

#include "Object.h"
#include "PlayerController.h"
#include "CommandExecutor.h"

namespace Arcana
{
	class ARCANA_SCENE_API Player : public Object, public CommandExecutor
	{
	public:

		Player();

		virtual ~Player();

		virtual bool execute(World* world, const std::string& command) override;

		PlayerController* getController(const World* world) const;

		virtual void setController(PlayerController* controller);

	private:

		PlayerController* _controller;
	};
}

#endif // !PLAYER_H_

