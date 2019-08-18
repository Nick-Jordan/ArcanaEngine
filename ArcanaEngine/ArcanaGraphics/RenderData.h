#ifndef RENDER_DATA_H_
#define RENDER_DATA_H_

#include "Transform.h"

namespace Arcana
{
	struct RenderData
	{
		Matrix4d View;
		Matrix4d Projection;
		Vector3d EyePosition;
	};
}

#endif // !RENDER_DATA_H_