#ifndef ACTOR_ATTACHMENT_H_
#define ACTOR_ATTACHMENT_H_

#include "SceneDefines.h"

namespace Arcana
{
	enum AttachmentRule
	{
		KeepRelative,
		KeepWorld,
		SnapTo,
	};

	struct ARCANA_SCENE_API TransformAttachmentRules
	{
		AttachmentRule positionRule;
		AttachmentRule rotationRule;
		AttachmentRule scaleRule;

		TransformAttachmentRules(AttachmentRule position = AttachmentRule::KeepRelative,
			AttachmentRule rotation = AttachmentRule::KeepRelative, 
			AttachmentRule scale = AttachmentRule::KeepWorld)
			: positionRule(position), rotationRule(rotation), scaleRule(scale) { }
	};
}

#endif // !ACTOR_ATTACHMENT_H_