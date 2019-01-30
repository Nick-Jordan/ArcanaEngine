#ifndef OPAQUE_OBJECT_STAGE_H_
#define OPAQUE_OBJECT_STAGE_H_

#include "RenderStage.h"

namespace Arcana
{
	class OpaqueObjectStage : public RenderStage
	{
	public:

		OpaqueObjectStage();

		~OpaqueObjectStage();

		virtual void initialize() override;

		virtual void finalize() override;

		virtual void render() override;
	};
}

#endif // !OPAQUE_OBJECT_STAGE_H_

