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
	REGISTER_CALLBACK(MeshRenderCallback);

	struct ARCANA_GRAPHICS_API MeshRenderProperties
	{
		RenderState renderState;
		std::string rendererStage;
		LightProperties lightProperties;
	};

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

		MeshRenderContext() : mesh(nullptr), material(nullptr) {};

		Mesh* mesh;

		Material* material;

		Transform transform;

		Matrix4d viewMatrix;

		Matrix4d projectionMatrix;

		Vector3d eyePosition;

		std::vector<UniformParameter> uniforms;

		MeshRenderCallback callback;

		MeshRenderProperties renderProperties;

		//keep this?
		//FTLResult ftlResult;

		bool isValid() const
		{
			return (mesh && material) || callback.isBound();
		}

		bool hasMesh() const
		{
			return (mesh && material);
		}
	};
}

#endif // !MESH_RENDER_CONTEXT_H_