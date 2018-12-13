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

		struct UniformParameter
		{
			UniformParameter() {};
			~UniformParameter() {};

			std::string name;

			Uniform::Value value;
		};

		Mesh* mesh;

		Material* material;

		Transform transform;

		Matrix4d viewMatrix;

		Matrix4d projectionMatrix;

		Vector3d eyePosition;

		RenderState renderState;

		std::vector<UniformParameter> uniforms;

		bool isValid() const
		{
			return mesh && material;
		}
	};
}

#endif // !MESH_RENDER_CONTEXT_H_