#ifndef MESH_RENDER_CONTEXT_H_
#define MESH_RENDER_CONTEXT_H_

#include "GraphicsDefines.h"

#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "RenderState.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API MeshRenderContext
	{
	public:

		Mesh* mesh;

		Material* material;

		Transform transform;

		RenderState renderState;

		bool isValid() const
		{
			return mesh && material;
		}
	};
}

#endif // !MESH_RENDER_CONTEXT_H_