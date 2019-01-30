#ifndef POST_PROCESSING_STAGE_H_
#define POST_PROCESSING_STAGE_H_

#include "RenderStage.h"

namespace Arcana
{
	class PostProcessingStage : public RenderStage
	{
	public:

		PostProcessingStage();

		~PostProcessingStage();

		virtual void initialize() override;

		virtual void finalize() override;

		virtual void render() override;
	};
}

#endif // !POST_PROCESSING_STAGE_H_

