#ifndef TRANSPARENT_OBJECT_STAGE_H_
#define TRANSPARENT_OBJECT_STAGE_H_

#include "RenderStage.h"

namespace Arcana
{
	class TransparentObjectStage : public RenderStage
	{
	public:

		TransparentObjectStage();

		~TransparentObjectStage();

		virtual void initialize() override;

		virtual void finalize() override;

		virtual void render() override;
	};
}

#endif // !TRANSPARENT_OBJECT_STAGE_H_

