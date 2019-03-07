#ifndef MESH_LOADER_H_
#define MESH_LOADER_H_

#include "GraphicsDefines.h"

#include "Mesh.h"

#include "Material.h"

#include <fbxsdk.h>

#include "Vector2.h"
#include "Vector3.h"

#include "Vertex.h"
#include "ArcanaLog.h"

#include "MaterialMap.h"

#include <vector>

namespace Arcana
{
	REGISTER_CATEGORY(MeshLoaderLog, none)

	struct BlendingIndexWeightPair
	{
		uint32 blendingIndex;
		double blendingWeight;

		BlendingIndexWeightPair() :
			blendingIndex(0),
			blendingWeight(0)
		{}
	};

	struct ControlPoint
	{
		Vector3f position;
		std::vector<BlendingIndexWeightPair> blendingInfo;

		ControlPoint()
		{
			blendingInfo.reserve(4);
		}
	};

	struct Triangle
	{
		std::vector<uint32> indices;
		std::string materialName;
		uint32 materialIndex;

		bool operator<(const Triangle& rhs)
		{
			return materialIndex < rhs.materialIndex;
		}
	};

	struct ARCANA_GRAPHICS_API MeshTriangleData
	{
		std::vector<Triangle> triangles;
	};

	struct ARCANA_GRAPHICS_API MeshRootData
	{
		std::vector<ControlPoint> controlPoints;
		std::vector<FBXBlendVertex> vertices;
		std::vector<MeshTriangleData> triangleData;
	};

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

		MeshStruct createMesh(const std::string& path, const Shader& shader);

	private:

		void processNode(FbxNode* node, MeshRootData& rootData, MaterialMap* material, const Shader& shader);

		void processMesh(FbxNode* node, MeshRootData& rootData);

		void processControlPoints(FbxNode* inNode, MeshRootData& rootData);

		void readUV(FbxMesh* mesh, int32 ctrlPointIndex, int32 textureUVIndex, int32 uvLayer, Vector2f& uv);

		void readNormal(FbxMesh* mesh, int32 ctrlPointIndex, int32 vertexCounter, Vector3f& normal);

		void readBinormal(FbxMesh* mesh, int32 ctrlPointIndex, int32 vertexCounter, Vector3f& binormal);

		void readTangent(FbxMesh* mesh, int32 ctrlPointIndex, int32 vertexCounter, Vector3f& tangent);

		void optimize(MeshRootData& rootData);

		int32 findVertex(const FBXBlendVertex& targetVertex, const std::vector<FBXBlendVertex>& uniqueVertices);

		void processMaterials(FbxNode* node, MaterialMap* material, const Shader& shader);

		void associateMaterials(FbxNode* node, MeshTriangleData& data);

		void processMaterialAttribute(FbxSurfaceMaterial* fbxMaterial, uint32 materialIndex, MaterialMap* material);
	};
}

#endif // !MESH_LOADER_H_

