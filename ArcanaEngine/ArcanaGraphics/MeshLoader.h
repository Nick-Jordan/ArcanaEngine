#ifndef MESH_LOADER_H_
#define MESH_LOADER_H_

#include "GraphicsDefines.h"

#include "Mesh.h"

#include "Material.h"

#include "Vector2.h"
#include "Vector3.h"

#include "Vertex.h"
#include "ArcanaLog.h"

#include "MaterialMap.h"

#include <vector>

namespace Arcana
{
	REGISTER_CATEGORY(MeshLoaderLog, none)

	struct ARCANA_GRAPHICS_API MeshStruct //rename???
	{
		Mesh* mesh;

		MaterialMap* materialMap;

		MeshStruct() : mesh(nullptr), materialMap(nullptr) {};

		bool isValid() const
		{
			return mesh && materialMap;
		};
	};

	class ARCANA_GRAPHICS_API MeshLoader
	{
	public:
		
		static MeshLoader& instance();

	public:

		MeshLoader();

		~MeshLoader();

		MeshStruct createMesh(const std::string& path, std::string vertex = "", std::string fragment = "");	
	};
}

#endif // !MESH_LOADER_H_

