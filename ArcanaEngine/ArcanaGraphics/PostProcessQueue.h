#ifndef POST_PROCESS_QUEUE_H_
#define POST_PROCESS_QUEUE_H_

#include "GraphicsDefines.h"

#include "PostProcessEffect.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API PostProcessQueue
	{
		friend class PostProcessor;

	public:

		PostProcessQueue();

		~PostProcessQueue();

		void setBaseEffect(PostProcessEffect* effect, bool enabled = true);

		void setEndEffect(PostProcessEffect* effect, bool enabled = true);

		void addEffect(PostProcessEffect* effect, bool enabled = true);

		//void setEffect(PostProcessEffect* input, PostProcessEffect* effect, bool enabled = true);

		void build();

		void buildCenter();

		void toggleEffect(const std::string& name);

		void enableEffect(const std::string& name);

		void disableEffect(const std::string& name);

	private:

		PostProcessEffect* get(const std::string& name);

		void process(std::vector<PostProcessEffect*>& queue) const;

	private:

		PostProcessEffect* _base;
		PostProcessEffect* _end;

		std::vector<PostProcessEffect*> _queue;
	};
}

#endif // !POST_PROCESS_QUEUE_H_

