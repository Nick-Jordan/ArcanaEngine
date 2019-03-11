#include "MeshLoader.h"

#include "MeshIndexComponent.h"
#include "FileInputStream.h"

namespace Arcana
{
	INITIALIZE_CATEGORY(Arcana, MeshLoaderLog)


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

	int32 getSize(bool hasPosition, bool hasNormal, bool hasColor, bool hasTangent, bool hasBinormal, bool* hasTexCoords)
	{
		int32 size = 3 * (hasPosition + hasNormal + hasTangent + hasBinormal);

		if (hasColor)
		{
			size += 4;
		}

		for (uint32 i = 0; i < 8; i++)
		{
			if (hasTexCoords[i])
			{
				size += 2;
			}
		}

		return size;
	}

	MeshStruct MeshLoader::createMesh(const std::string& path, const Shader& shader)
	{
		FileInputStream file;
		if (!file.open(path))
		{
			return MeshStruct();
		}

		int32 numMeshes;
		file.read(&numMeshes, sizeof(int32));

		std::vector<float> vertexData;
		std::vector<std::vector<uint32>> totalIndexData;
		int32 actualVertexCount = 0;
		bool hasPosition;
		bool hasNormal;
		bool hasColor;
		bool hasTangent;
		bool hasBinormal;
		bool hasTexCoords[8];//change to MAX_TEXCOORDS or something

		for (uint32 i = 0; i < numMeshes; i++)
		{
			LOGF(Info, CoreEngine, "reading mesh: %d", i);

			int32 numVertices;
			file.read(&numVertices, sizeof(int32));
			int32 numIndices;
			file.read(&numIndices, sizeof(int32));

			file.read(&hasPosition, sizeof(bool));
			file.read(&hasNormal, sizeof(bool));
			file.read(&hasColor, sizeof(bool));
			file.read(&hasTangent, sizeof(bool));
			file.read(&hasBinormal, sizeof(bool));
			for (uint32 j = 0; j < 8; j++)//change to MAX_TEXCOORDS or something
			{
				file.read(&hasTexCoords[j], sizeof(bool));
			}

			std::vector<uint32> indexData;

			for (uint32 j = 0; j < numVertices; j++)
			{
				float v;
				file.read(&v, sizeof(float));
				vertexData.push_back(v);
			}
			
			for (uint32 j = 0; j < numIndices; j++)
			{
				uint32 index;
				file.read(&index, sizeof(uint32));
				indexData.push_back(index + actualVertexCount);
			}

			actualVertexCount += numVertices / getSize(hasPosition, hasNormal, hasColor, hasTangent, hasBinormal, hasTexCoords);

			totalIndexData.push_back(indexData);
		}

		std::vector<VertexFormat::Attribute> attribs;
		if (hasPosition)
		{
			attribs.push_back(VertexFormat::Attribute(VertexFormat::Semantic::Position, 3));
		}
		if (hasNormal)
		{
			attribs.push_back(VertexFormat::Attribute(VertexFormat::Semantic::Position, 3));
		}
		if (hasColor)
		{
			attribs.push_back(VertexFormat::Attribute(VertexFormat::Semantic::Color, 4));
		}
		if (hasTangent)
		{
			attribs.push_back(VertexFormat::Attribute(VertexFormat::Semantic::Tangent, 3));
		}
		if (hasBinormal)
		{
			attribs.push_back(VertexFormat::Attribute(VertexFormat::Semantic::Binormal, 3));
		}

		for (uint32 i = 0; i < 8; i++)//change to MAX_TEXCOORDS or something
		{
			if (hasTexCoords[i])
			{
				attribs.push_back(VertexFormat::Attribute((VertexFormat::Semantic)(VertexFormat::Semantic::TexCoord0 + i), 2));
			}
		}

		VertexFormat format(attribs.size(), &attribs[0]);

		Mesh* mesh = new Mesh(format, Mesh::Triangles);

		for (uint32 td = 0; td < totalIndexData.size(); td++)
		{
			mesh->addIndexComponent(Mesh::Triangles)->setIndexBuffer(IndexBuffer::Index32, totalIndexData[td].size(), false, &totalIndexData[td][0]);
		}

		mesh->setVertexBuffer(format, vertexData.size())->setVertexData(&vertexData[0]);

		MeshStruct meshStruct;
		meshStruct.mesh = mesh;
		meshStruct.materialMap = nullptr;

		return meshStruct;
	}
}
