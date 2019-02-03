#include "MeshLoader.h"

#include "MeshIndexComponent.h"

namespace Arcana
{

	MeshLoader& MeshLoader::instance()
	{
		static MeshLoader meshLoader;
		return meshLoader;
	}

	MeshLoader::MeshLoader()
	{

	}

	MeshLoader::~MeshLoader()
	{

	}

	Mesh* MeshLoader::createMesh(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			LOGF(Error, CoreEngine, "ASSIMP-Error: %s", importer.GetErrorString());
			return nullptr;
		}

		MeshRootData rootData;

		_indexNum = 0;

		processNode(scene->mRootNode, scene, rootData);

		Mesh* mesh = new Mesh(rootData.format, Mesh::Triangles);

		mesh->setVertexBuffer(rootData.format, rootData.vertices.size())->setVertexData(&rootData.vertices[0].position.x);

		for (uint32 i = 0; i < rootData.indexData.size(); i++)
		{
			mesh->addIndexComponent(Mesh::Triangles)->setIndexBuffer(IndexBuffer::Index32, rootData.indexData[i].indices.size(), false, &rootData.indexData[i].indices[0]);
		}

		return mesh;
	}

	void MeshLoader::processNode(aiNode *node, const aiScene *scene, MeshRootData& rootData)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			processMesh(mesh, scene, rootData, _indexNum);
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene, rootData);
		}
	}

	void MeshLoader::processMesh(aiMesh *mesh, const aiScene *scene, MeshRootData& rootData, uint32 indexNum)
	{
		MeshIndexData data;

		std::vector<VertexFormat::Attribute> attribs;
		
		attribs.push_back(VertexFormat::Attribute(VertexFormat::Semantic::Position, 3));
		attribs.push_back(VertexFormat::Attribute(VertexFormat::Semantic::Normal, 3));
		
		bool addTexCoordAttrib = true;

		for (uint32 i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			Vector3f vector;
			// positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.position = vector;
			// normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
			// texture coordinates
			if (mesh->mTextureCoords[0])
			{
				if (addTexCoordAttrib)
				{
					attribs.push_back(VertexFormat::Attribute(VertexFormat::Semantic::TexCoord0, 2));
					addTexCoordAttrib = false;
				}
				Vector2f vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.texCoords = vec;
			}
			else
				vertex.texCoords = Vector2f::zero();
			// tangent
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.tangent = vector;
			// bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.bitangent = vector;
			rootData.vertices.push_back(vertex);
		}

		attribs.push_back(VertexFormat::Attribute(VertexFormat::Semantic::Tangent, 3));
		attribs.push_back(VertexFormat::Attribute(VertexFormat::Semantic::Binormal, 3));

		for (uint32 i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			for (uint32 j = 0; j < face.mNumIndices; j++)
			{
				data.indices.push_back(face.mIndices[j] + indexNum);
			}
		}

		_indexNum += data.indices.size();

		rootData.format = VertexFormat(attribs.size(), &attribs[0]);

		rootData.indexData.push_back(data);
	}
}
