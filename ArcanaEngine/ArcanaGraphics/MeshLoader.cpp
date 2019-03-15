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
		int32 numMaterials;
		file.read(&numMaterials, sizeof(int32));
		bool hasMaterials = numMaterials > 0;
		LOGF(Info, CoreEngine, "numMaterials: %d", numMaterials);

		std::vector<float> vertexData;
		std::vector<std::vector<uint32>> totalIndexData;
		int32 actualVertexCount = 0;
		bool hasPosition;
		bool hasNormal;
		bool hasColor;
		bool hasTangent;
		bool hasBinormal;
		bool hasTexCoords[8];//change to MAX_TEXCOORDS or something

		MaterialMap* materialMap = new MaterialMap("material");

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

			if (hasMaterials)
			{
				int32 materialIndex;
				file.read(&materialIndex, sizeof(int32));
				LOGF(Info, CoreEngine, "materialIndex: %d", materialIndex);

				materialMap->addTechniqueMapping(i, materialIndex, false);
			}
		}
		//  for each attribute
	//		name length
	//		name
	//		type
	//		data

		for (uint32 i = 0; i < numMaterials; i++)
		{
			LOGF(Info, CoreEngine, "reading material: %d", i);

			int32 numAttributes;
			file.read(&numAttributes, sizeof(int32));

			LOGF(Info, CoreEngine, "material num attributes: %d", numAttributes);

			Technique* technique = new Technique(shader);
			materialMap->addTechnique(technique);
			for (uint32 j = 0; j < numAttributes; j++)
			{
				std::string name;
				size_t size;
				file.read(&size, sizeof(size));
				LOGF(Info, CoreEngine, "material attribute name length: %d", size);
				name.resize(size);
				file.read(&name[0], size);	
				LOGF(Info, CoreEngine, "material attribute name: %s", name.c_str());


				int32 type;
				file.read(&type, sizeof(int32));
				LOGF(Info, CoreEngine, "material attribute type: %d", type);
				if (type == 0)
				{
					float x;
					file.read(&x, sizeof(x));
					technique->addAttribute(name, x);
				}
				else if (type == 1)
				{
					float x;
					file.read(&x, sizeof(x));
					float y;
					file.read(&y, sizeof(y));
					float z;
					file.read(&z, sizeof(z));
					LOGF(Info, CoreEngine, "x, y, z: %f, %f, %f", x, y, z);
					technique->addAttribute(name, Vector3f(x, y, z));
				}
				else if (type == 2)
				{
					float x;
					file.read(&x, sizeof(x));
					float y;
					file.read(&y, sizeof(y));
					float z;
					file.read(&z, sizeof(z));
					float w;
					file.read(&z, sizeof(w));
					technique->addAttribute(name, Vector4f(x, y, z, w));
				}
				else if (type == 3)
				{
					uint32 length;
					file.read(&length, sizeof(length));
					unsigned char* data = new unsigned char[length];

					int32 x;
					file.read(&x, sizeof(x));
					int32 y;
					file.read(&y, sizeof(y));
					int32 z;
					file.read(&z, sizeof(z));

					file.read(data, length);

					Texture::Parameters params;
					params.setMinFilter(TextureFilter::LinearMipmapLinear);
					params.setMagFilter(TextureFilter::Linear);

					Texture* texture = Texture::create2D(
						z == 4 ? Texture::RGBA : Texture::RGB, 
						x, y, 
						z == 4 ? Texture::RGBA8 : Texture::RGB8,
						Texture::UnsignedByte, data, params, true);

					technique->addAttribute(name, texture);

					AE_DELETE_ARRAY(data);
				}
			}
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
		meshStruct.materialMap = materialMap;

		return meshStruct;
	}
}
