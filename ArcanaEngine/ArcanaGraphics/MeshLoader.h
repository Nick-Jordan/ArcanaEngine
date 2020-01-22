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

		struct MeshData
		{
			uint32 numMaterials;
			Shader::Defines vertexDefines;
			bool hasNormal;
			bool hasColor;
			bool hasTexCoords0;
			VertexFormat format;
			std::vector<std::vector<uint32>> totalIndexData;
			std::vector<float> vertexData;
			std::vector<KeyValuePair<uint32, uint32>> techniqueMappings;
			std::vector<std::vector<MaterialAttribute>> materialAttributes;
			std::vector<std::vector<KeyValuePair<std::string, KeyValuePair<Vector3i, unsigned char*>>>> materialTextureAttributes;
		};

		MeshLoader();

		~MeshLoader();

		MeshStruct createMesh(const std::string& path, std::string vertex = "", std::string fragment = "");

		bool createMeshLoadData(const std::string& path, MeshData& data);

		MeshStruct createMeshFinal(MeshData& data, std::string vertex, std::string fragment);
	};
}

#endif // !MESH_LOADER_H_

