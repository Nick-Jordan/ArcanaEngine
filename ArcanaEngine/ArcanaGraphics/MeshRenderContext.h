#ifndef MESH_RENDER_CONTEXT_H_
#define MESH_RENDER_CONTEXT_H_

#include "GraphicsDefines.h"

#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "FTLResult.h"
#include "RenderState.h"
#include "LightProperties.h"

#include "Callback.h"

namespace Arcana
{
	REGISTER_CALLBACK(MeshRenderCallback)

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

		std::string rendererStage;

		std::vector<UniformParameter> uniforms;

		LightProperties lightProperties;

		MeshRenderCallback callback;

		//keep this?
		//FTLResult ftlResult;

		bool isValid() const
		{
			return (mesh && material) || callback.isBound();
		}
	};
}

#endif // !MESH_RENDER_CONTEXT_H_