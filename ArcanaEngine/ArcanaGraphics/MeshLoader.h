#ifndef MESH_LOADER_H_
#define MESH_LOADER_H_

#include "GraphicsDefines.h"

#include "Mesh.h"

#include "Material.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Vector2.h"
#include "Vector3.h"

#include <vector>

namespace Arcana
{
	struct ARCANA_GRAPHICS_API Vertex
	{
		Vector3f position;
		Vector3f normal;
		Vector2f texCoords;
		Vector3f tangent;
		Vector3f bitangent;
	};

	struct ARCANA_GRAPHICS_API MeshIndexData
	{
		std::vector<uint32> indices;
	};

	struct ARCANA_GRAPHICS_API MeshRootData
	{
		VertexFormat format;
		std::vector<MeshIndexData> indexData;
		std::vector<Vertex> vertices;
	};

	class ARCANA_GRAPHICS_API MeshLoader
	{
	public:
		
		static MeshLoader& instance();

	public:

		MeshLoader();

		~MeshLoader();

		Mesh* createMesh(const std::string& path);

	private:

		void processNode(aiNode *node, const aiScene *scene, MeshRootData& rootData);

		void processMesh(aiMesh *mesh, const aiScene *scene, MeshRootData& rootData, uint32 indexNum);

		uint32 _indexNum;
	};
}

#endif // !MESH_LOADER_H_

