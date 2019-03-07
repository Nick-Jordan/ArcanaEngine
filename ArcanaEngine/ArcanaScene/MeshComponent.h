#ifndef MESH_COMPONENT_H_
#define MESH_COMPONENT_H_

#include "SceneDefines.h"

#include "GeometryComponent.h"

namespace Arcana
{

	class ARCANA_SCENE_API MeshComponent : public GeometryComponent
	{
	public:

		MeshComponent();

		MeshComponent(Mesh* mesh, Material* material, const MeshRenderProperties& properties);

		virtual ~MeshComponent();

		void initialize(Mesh* mesh, Material* material, const MeshRenderProperties& properties);

		virtual void initialize() override;

		virtual bool createRenderProcedure() override;

		virtual void updateRenderData(Matrix4d view, Matrix4d projection, Vector3d eyePosition) override;

	private:

		Mesh* _mesh;
		Material* _material;
		MeshRenderProperties _properties;
	};

}

#endif // !MESH_COMPONENT_H_

