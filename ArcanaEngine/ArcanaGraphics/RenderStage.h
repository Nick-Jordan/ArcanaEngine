#ifndef RENDER_STAGE_H_
#define RENDER_STAGE_H_

#include "GraphicsDefines.h"

#include "GlobalObjectID.h"
#include "RenderProcedure.h"
#include "RenderLight.h"
#include "RenderData.h"

namespace Arcana
{
	class RenderStage
	{
	public:

		RenderStage(std::string identifier);

		~RenderStage();

		virtual void initialize() = 0;

		virtual void finalize() = 0;

		virtual void render(const RenderData& data) = 0;

		void addProcedure(RenderProcedure* procedure);

		void addLight(const RenderLight& light);

		void clearProcedures();

		void clearLights();

		const GlobalObjectID& getId() const;

	protected:

		Array<RenderProcedure*> Procedures;

		Array<RenderLight> Lights;

	private:

		GlobalObjectID _identifier;
	};
}

#endif // !RENDER_STAGE_H_