#ifndef TRANSPARENT_OBJECT_STAGE_H_
#define TRANSPARENT_OBJECT_STAGE_H_

#include "GraphicsDefines.h"

#include "RenderStage.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API TransparentObjectStage : public RenderStage
	{
	public:

		TransparentObjectStage();

		~TransparentObjectStage();

		virtual void initialize() override;

		virtual void finalize() override;

		virtual void render() override;

	private:

		RenderState _renderState;
	};
}

#endif // !TRANSPARENT_OBJECT_STAGE_H_

