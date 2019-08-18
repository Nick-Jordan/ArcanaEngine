#ifndef MESH_COMPONENT_H_
#define MESH_COMPONENT_H_

#include "SceneDefines.h"

#include "Mesh.h"
#include "GeometryComponent.h"

namespace Arcana
{

	class ARCANA_SCENE_API MeshComponent : public GeometryComponent
	{
	public:

		MeshComponent();

		MeshComponent(Mesh* mesh, Material* material, const RenderProcedure::RenderProperties& properties);

		virtual ~MeshComponent();

		void initialize(Mesh* mesh, Material* material, const RenderProcedure::RenderProperties& properties);

		virtual void initialize() override;

		virtual bool createRenderProcedure() override;

	private:

		Mesh* _mesh;
		Material* _material;
		RenderProcedure::RenderProperties _properties;
	};

}

#endif // !MESH_COMPONENT_H_

