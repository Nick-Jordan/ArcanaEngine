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

	MeshStruct MeshLoader::createMesh(const std::string& path, std::string vertex, std::string fragment)
	{
		/*FileInputStream file;
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

		MaterialMap* materialMap = nullptr;
		if (hasMaterials)
		{
			materialMap = new MaterialMap("material");
		}

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
				LOGF(Info, CoreEngine, "%d --> materialIndex: %d", i, materialIndex);

				materialMap->addTechniqueMapping(i, materialIndex, false);
			}
		}

		Shader::Defines vertexDefines;

		std::vector<VertexFormat::Attribute> attribs;
		int vertexAttribIndex = 0;
		if (hasPosition)
		{
			LOG(Info, CoreEngine, "position");
			attribs.push_back(VertexFormat::Attribute(VertexFormat::Semantic::Position, 3));
			vertexDefines.addDefine("ATTRIB_POSITION", "0");
			vertexAttribIndex++;
		}
		if (hasNormal)
		{
			LOG(Info, CoreEngine, "normal");
			attribs.push_back(VertexFormat::Attribute(VertexFormat::Semantic::Normal, 3));
			vertexDefines.addDefine("ATTRIB_NORMALS", std::to_string(vertexAttribIndex));
			vertexAttribIndex++;
		}
		if (hasColor)
		{
			LOG(Info, CoreEngine, "color");
			attribs.push_back(VertexFormat::Attribute(VertexFormat::Semantic::Color, 4));
			vertexDefines.addDefine("ATTRIB_COLOR", std::to_string(vertexAttribIndex));
			vertexAttribIndex++;
		}
		if (hasTangent)
		{
			LOG(Info, CoreEngine, "tangent");
			attribs.push_back(VertexFormat::Attribute(VertexFormat::Semantic::Tangent, 3));
			vertexDefines.addDefine("ATTRIB_TANGENT", std::to_string(vertexAttribIndex));
			vertexAttribIndex++;
		}
		if (hasBinormal)
		{
			LOG(Info, CoreEngine, "binormal");
			attribs.push_back(VertexFormat::Attribute(VertexFormat::Semantic::Binormal, 3));
			vertexDefines.addDefine("ATTRIB_BINORMAL", std::to_string(vertexAttribIndex));
			vertexAttribIndex++;
		}

		for (uint32 i = 0; i < 8; i++)//change to MAX_TEXCOORDS or something
		{
			if (hasTexCoords[i])
			{
				LOGF(Info, CoreEngine, "has tex coords");
				attribs.push_back(VertexFormat::Attribute((VertexFormat::Semantic)(VertexFormat::Semantic::TexCoord0 + i), 2));
				vertexDefines.addDefine("ATTRIB_TEXCOORD" + std::to_string(i), std::to_string(vertexAttribIndex));
				vertexAttribIndex++;
			}
		}

		VertexFormat format(attribs.size(), &attribs[0]);

		std::vector<Shader> shaders(numMaterials);
		std::vector<std::vector<MaterialAttribute>> attributes(numMaterials);

		for (uint32 i = 0; i < numMaterials; i++)
		{
			LOGF(Info, CoreEngine, "reading material: %d", i);

			int32 numAttributes;
			file.read(&numAttributes, sizeof(int32));

			LOGF(Info, CoreEngine, "material num attributes: %d", numAttributes);

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
					attributes[i].push_back(MaterialAttribute(name, x));
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
					attributes[i].push_back(MaterialAttribute(name, Vector3f(x, y, z)));
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
					attributes[i].push_back(MaterialAttribute(name, Vector4f(x, y, z, w)));
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
					params.setWrapS(TextureWrap::Repeat);
					params.setWrapT(TextureWrap::Repeat);

					Texture* texture = Texture::create2D(
						z == 4 ? Texture::RGBA : Texture::RGB,
						x, y,
						z == 4 ? Texture::RGBA8 : Texture::RGB8,
						Texture::UnsignedByte, data, params, true);

					attributes[i].push_back(MaterialAttribute(name, texture));

					AE_DELETE_ARRAY(data);
				}
			}
		}

		for (uint32 i = 0; i < numMaterials; i++)
		{
			if (vertex.empty())
			{
				shaders[i].createProgram(Shader::Vertex, "resources/arcana/shaders/mesh_default_vert.glsl", vertexDefines);
			}
			else
			{
				shaders[i].createProgram(Shader::Vertex, vertex);
			}
			if (fragment.empty())
			{
				std::string s = "#version 400\n"
					"layout(location = 0) out vec4 fs_PositionAO;\n"
					"layout(location = 1) out vec4 fs_NormalRoughness;\n"
					"layout(location = 2) out vec4 fs_AlbedoSpecular;\n"
					"layout(location = 3) out vec4 fs_EmissiveMetallic;\n"
					"layout(location = 4) out vec4 fs_LightData;\n"
					"in vec3 fs_Position;\n";

				if (hasNormal)
				{
					s += "in vec3 fs_Normal;\n";
				}
				if (hasColor)
				{
					s += "in vec3 fs_Color;\n";
				}
				if (hasTexCoords[0])
				{
					s += "in vec2 fs_TexCoord0;\n";
				}

				for (uint32 j = 0; j < attributes[i].size(); j++)
				{
					std::string typeString = "vec3 ";
					if (attributes[i][j].getType() == MaterialAttribute::Texture)
					{
						typeString = "sampler2D ";
					}
					if (attributes[i][j].getType() == MaterialAttribute::Number)
					{
						typeString = "float ";
					}
					s += "uniform " + typeString + attributes[i][j].getName() + ";\n";
				}

				s += "void main(){\n"
					"fs_PositionAO = vec4(fs_Position, 1.0);\n"
					"fs_NormalRoughness = vec4(fs_Normal, 1.0);\n"
					"fs_AlbedoSpecular = vec4(vec3(0.0), 1.0);\n"
					"fs_EmissiveMetallic = vec4(vec3(0.0), 0.0);\n"
					"fs_LightData = vec4(0);\n";

				for (uint32 j = 0; j < attributes[i].size(); j++)
				{
					if (attributes[i][j].getName() == "baseColor")
					{
						if (attributes[i][j].getType() == MaterialAttribute::Vector3)
						{
							s += "fs_AlbedoSpecular.xyz = baseColor;\n";
						}
						else if (attributes[i][j].getType() == MaterialAttribute::Texture)
						{
							s += "fs_AlbedoSpecular.xyz = texture(baseColor, fs_TexCoord0).rgb;\n";
						}
						else if (attributes[i][j].getType() == MaterialAttribute::Number)
						{
							s += "fs_AlbedoSpecular.xyz = vec3(baseColor);\n";
						}
					}
					else if (attributes[i][j].getName() == "specular")
					{
						if (attributes[i][j].getType() == MaterialAttribute::Texture)
						{
							s += "fs_AlbedoSpecular.w = texture(specular, fs_TexCoord0).r;\n";
						}
						else if (attributes[i][j].getType() == MaterialAttribute::Number)
						{
							s += "fs_AlbedoSpecular.w = specular;\n";
						}
					}
					else if (attributes[i][j].getName() == "ao")
					{
						if (attributes[i][j].getType() == MaterialAttribute::Texture)
						{
							s += "fs_PositionAO.w = texture(ao, fs_TexCoord0).r;\n";
						}
						else if (attributes[i][j].getType() == MaterialAttribute::Number)
						{
							s += "fs_PositionAO.w = ao;\n";
						}
					}
					else if (attributes[i][j].getName() == "roughness")
					{
						if (attributes[i][j].getType() == MaterialAttribute::Texture)
						{
							s += "fs_NormalRoughness.w = texture(roughness, fs_TexCoord0).r;\n";
						}
						else if (attributes[i][j].getType() == MaterialAttribute::Number)
						{
							s += "fs_NormalRoughness.w = roughness;\n";
						}
					}
					else if (attributes[i][j].getName() == "normals")
					{
						if (attributes[i][j].getType() == MaterialAttribute::Texture)
						{
							s += "fs_NormalRoughness.xyz = getNormalMap();\n";
						}
					}
					else if (attributes[i][j].getName() == "metallic")
					{
						if (attributes[i][j].getType() == MaterialAttribute::Texture)
						{
							s += "fs_EmissiveMetallic.w = texture(metallic, fs_TexCoord0).r;\n";
						}
						else if (attributes[i][j].getType() == MaterialAttribute::Number)
						{
							s += "fs_EmissiveMetallic.w = metallic;\n";
						}
					}
					else if (attributes[i][j].getName() == "emissive")
					{
						if (attributes[i][j].getType() == MaterialAttribute::Vector3)
						{
							s += "fs_EmissiveMetallic.xyz = emissive;\n";
						}
						else if (attributes[i][j].getType() == MaterialAttribute::Texture)
						{
							s += "fs_EmissiveMetallic.xyz = texture(emissive, fs_TexCoord0).rgb;\n";
						}
						else if (attributes[i][j].getType() == MaterialAttribute::Number)
						{
							s += "fs_EmissiveMetallic.xyz = vec3(emissive);\n";
						}
					}
				}

				s += "}";

				char* str = new char[s.length() + 1];
				strcpy(str, s.c_str());
				shaders[i].createProgramFromSource(Shader::Fragment, str, vertexDefines);
			}
			else
			{
				shaders[i].createProgram(Shader::Fragment, fragment);
			}
		}

		for (uint32 i = 0; i < numMaterials; i++)
		{
			Technique* technique = new Technique(shaders[i]);
			materialMap->addTechnique(technique);
			for (uint32 j = 0; j < attributes[i].size(); j++)
			{
				technique->addAttribute(attributes[i][j]);
			}
		}

		Mesh* mesh = new Mesh(format, Mesh::Triangles);

		for (uint32 td = 0; td < totalIndexData.size(); td++)
		{
			mesh->addIndexComponent(Mesh::Triangles)->setIndexBuffer(IndexBuffer::Index32, totalIndexData[td].size(), false, &totalIndexData[td][0]);
		}

		LOGF(Info, CoreEngine, "vertexData: %d, vertexSize: %d, vertexCount: %d", vertexData.size(), (format.getVertexSize() / sizeof(float)), vertexData.size() / (format.getVertexSize() / sizeof(float)));
		mesh->setVertexBuffer(format, vertexData.size() / (format.getVertexSize() / sizeof(float)))->setVertexData(&vertexData[0]);

		MeshStruct meshStruct;
		meshStruct.mesh = mesh;
		meshStruct.materialMap = materialMap;

		return meshStruct;*/

		uint32 numMaterials;
		FileInputStream file;
		Shader::Defines vertexDefines;
		bool hasNormal;
		bool hasColor; 
		bool hasTexCoords0;
		VertexFormat format;
		std::vector<std::vector<uint32>> totalIndexData;
		std::vector<float> vertexData;
		MaterialMap* materialMap;

		createMeshLoadData(path, numMaterials, file, vertexDefines, hasNormal, hasColor, hasTexCoords0, format, totalIndexData, vertexData, &materialMap);

		if (file.size() == -1)
			return MeshStruct();

		return createMeshFinal(numMaterials, file, vertexDefines, hasNormal, hasColor, hasTexCoords0, format, totalIndexData, vertexData, &materialMap, vertex, fragment);
	}

	void MeshLoader::createMeshLoadData(const std::string& path, uint32& numMaterials, FileInputStream& file,
		Shader::Defines& vertexDefines, bool& hasNormal, bool& hasColor, bool& hasTexCoords0, VertexFormat& format,
		std::vector<std::vector<uint32>>& totalIndexData, std::vector<float>& vertexData, MaterialMap** materialMap)
	{
		if (!file.open(path))
		{
			return;
		}

		int32 numMeshes;
		file.read(&numMeshes, sizeof(int32));
		file.read(&numMaterials, sizeof(int32));
		bool hasMaterials = numMaterials > 0;
		LOGF(Info, CoreEngine, "numMaterials: %d", numMaterials);

		int32 actualVertexCount = 0;
		bool hasPosition;
		bool hasTangent;
		bool hasBinormal;
		bool hasTexCoords[8];//change to MAX_TEXCOORDS or something

		*materialMap = nullptr;
		if (hasMaterials)
		{
			*materialMap = new MaterialMap("material");
		}

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
				LOGF(Info, CoreEngine, "%d --> materialIndex: %d", i, materialIndex);

				(*materialMap)->addTechniqueMapping(i, materialIndex, false);
			}
		}

		std::vector<VertexFormat::Attribute> attribs;
		int vertexAttribIndex = 0;
		if (hasPosition)
		{
			LOG(Info, CoreEngine, "position");
			attribs.push_back(VertexFormat::Attribute(VertexFormat::Semantic::Position, 3));
			vertexDefines.addDefine("ATTRIB_POSITION", "0");
			vertexAttribIndex++;
		}
		if (hasNormal)
		{
			LOG(Info, CoreEngine, "normal");
			attribs.push_back(VertexFormat::Attribute(VertexFormat::Semantic::Normal, 3));
			vertexDefines.addDefine("ATTRIB_NORMALS", std::to_string(vertexAttribIndex));
			vertexAttribIndex++;
		}
		if (hasColor)
		{
			LOG(Info, CoreEngine, "color");
			attribs.push_back(VertexFormat::Attribute(VertexFormat::Semantic::Color, 4));
			vertexDefines.addDefine("ATTRIB_COLOR", std::to_string(vertexAttribIndex));
			vertexAttribIndex++;
		}
		if (hasTangent)
		{
			LOG(Info, CoreEngine, "tangent");
			attribs.push_back(VertexFormat::Attribute(VertexFormat::Semantic::Tangent, 3));
			vertexDefines.addDefine("ATTRIB_TANGENT", std::to_string(vertexAttribIndex));
			vertexAttribIndex++;
		}
		if (hasBinormal)
		{
			LOG(Info, CoreEngine, "binormal");
			attribs.push_back(VertexFormat::Attribute(VertexFormat::Semantic::Binormal, 3));
			vertexDefines.addDefine("ATTRIB_BINORMAL", std::to_string(vertexAttribIndex));
			vertexAttribIndex++;
		}

		for (uint32 i = 0; i < 8; i++)//change to MAX_TEXCOORDS or something
		{
			if (hasTexCoords[i])
			{
				LOGF(Info, CoreEngine, "has tex coords");
				attribs.push_back(VertexFormat::Attribute((VertexFormat::Semantic)(VertexFormat::Semantic::TexCoord0 + i), 2));
				vertexDefines.addDefine("ATTRIB_TEXCOORD" + std::to_string(i), std::to_string(vertexAttribIndex));
				vertexAttribIndex++;
			}
		}

		hasTexCoords0 = hasTexCoords[0];

		format = VertexFormat(attribs.size(), &attribs[0]);
	}

	MeshStruct MeshLoader::createMeshFinal(uint32 numMaterials, FileInputStream& file, 
		const Shader::Defines& vertexDefines, bool hasNormal, bool hasColor, bool hasTexCoords0, const VertexFormat& format, 
		std::vector<std::vector<uint32>>& totalIndexData, const std::vector<float>& vertexData, MaterialMap** materialMap,
		std::string vertex, std::string fragment)
	{
		//maybe move shader creation to load data

		std::vector<Shader> shaders(numMaterials);
		std::vector<std::vector<MaterialAttribute>> attributes(numMaterials);

		for (uint32 i = 0; i < numMaterials; i++)
		{
			LOGF(Info, CoreEngine, "reading material: %d", i);

			int32 numAttributes;
			file.read(&numAttributes, sizeof(int32));

			LOGF(Info, CoreEngine, "material num attributes: %d", numAttributes);

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
					attributes[i].push_back(MaterialAttribute(name, x));
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
					attributes[i].push_back(MaterialAttribute(name, Vector3f(x, y, z)));
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
					attributes[i].push_back(MaterialAttribute(name, Vector4f(x, y, z, w)));
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
					params.setWrapS(TextureWrap::Repeat);
					params.setWrapT(TextureWrap::Repeat);

					Texture* texture = Texture::create2D(
						z == 4 ? Texture::RGBA : Texture::RGB,
						x, y,
						z == 4 ? Texture::RGBA8 : Texture::RGB8,
						Texture::UnsignedByte, data, params, true);

					attributes[i].push_back(MaterialAttribute(name, texture));

					AE_DELETE_ARRAY(data);
				}
			}
		}

		for (uint32 i = 0; i < numMaterials; i++)
		{
			if (vertex.empty())
			{
				shaders[i].createProgram(Shader::Vertex, "resources/arcana/shaders/mesh_default_vert.glsl", vertexDefines);
			}
			else
			{
				shaders[i].createProgram(Shader::Vertex, vertex);
			}
			if (fragment.empty())
			{
				std::string s = "#version 400\n"
					"layout(location = 0) out vec4 fs_PositionAO;\n"
					"layout(location = 1) out vec4 fs_NormalRoughness;\n"
					"layout(location = 2) out vec4 fs_AlbedoSpecular;\n"
					"layout(location = 3) out vec4 fs_EmissiveMetallic;\n"
					"layout(location = 4) out vec4 fs_LightData;\n"
					"in vec3 fs_Position;\n";

				if (hasNormal)
				{
					s += "in vec3 fs_Normal;\n";
				}
				if (hasColor)
				{
					s += "in vec3 fs_Color;\n";
				}
				if (hasTexCoords0)
				{
					s += "in vec2 fs_TexCoord0;\n";
				}

				for (uint32 j = 0; j < attributes[i].size(); j++)
				{
					std::string typeString = "vec3 ";
					if (attributes[i][j].getType() == MaterialAttribute::Texture)
					{
						typeString = "sampler2D ";
					}
					if (attributes[i][j].getType() == MaterialAttribute::Number)
					{
						typeString = "float ";
					}
					s += "uniform " + typeString + attributes[i][j].getName() + ";\n";
				}

				s += "void main(){\n"
					"fs_PositionAO = vec4(fs_Position, 1.0);\n"
					"fs_NormalRoughness = vec4(fs_Normal, 1.0);\n"
					"fs_AlbedoSpecular = vec4(vec3(0.0), 1.0);\n"
					"fs_EmissiveMetallic = vec4(vec3(0.0), 0.0);\n"
					"fs_LightData = vec4(0);\n";

				for (uint32 j = 0; j < attributes[i].size(); j++)
				{
					if (attributes[i][j].getName() == "baseColor")
					{
						if (attributes[i][j].getType() == MaterialAttribute::Vector3)
						{
							s += "fs_AlbedoSpecular.xyz = baseColor;\n";
						}
						else if (attributes[i][j].getType() == MaterialAttribute::Texture)
						{
							s += "fs_AlbedoSpecular.xyz = texture(baseColor, fs_TexCoord0).rgb;\n";
						}
						else if (attributes[i][j].getType() == MaterialAttribute::Number)
						{
							s += "fs_AlbedoSpecular.xyz = vec3(baseColor);\n";
						}
					}
					else if (attributes[i][j].getName() == "specular")
					{
						if (attributes[i][j].getType() == MaterialAttribute::Texture)
						{
							s += "fs_AlbedoSpecular.w = texture(specular, fs_TexCoord0).r;\n";
						}
						else if (attributes[i][j].getType() == MaterialAttribute::Number)
						{
							s += "fs_AlbedoSpecular.w = specular;\n";
						}
					}
					else if (attributes[i][j].getName() == "ao")
					{
						if (attributes[i][j].getType() == MaterialAttribute::Texture)
						{
							s += "fs_PositionAO.w = texture(ao, fs_TexCoord0).r;\n";
						}
						else if (attributes[i][j].getType() == MaterialAttribute::Number)
						{
							s += "fs_PositionAO.w = ao;\n";
						}
					}
					else if (attributes[i][j].getName() == "roughness")
					{
						if (attributes[i][j].getType() == MaterialAttribute::Texture)
						{
							s += "fs_NormalRoughness.w = texture(roughness, fs_TexCoord0).r;\n";
						}
						else if (attributes[i][j].getType() == MaterialAttribute::Number)
						{
							s += "fs_NormalRoughness.w = roughness;\n";
						}
					}
					else if (attributes[i][j].getName() == "normals")
					{
						if (attributes[i][j].getType() == MaterialAttribute::Texture)
						{
							s += "fs_NormalRoughness.xyz = getNormalMap();\n";
						}
					}
					else if (attributes[i][j].getName() == "metallic")
					{
						if (attributes[i][j].getType() == MaterialAttribute::Texture)
						{
							s += "fs_EmissiveMetallic.w = texture(metallic, fs_TexCoord0).r;\n";
						}
						else if (attributes[i][j].getType() == MaterialAttribute::Number)
						{
							s += "fs_EmissiveMetallic.w = metallic;\n";
						}
					}
					else if (attributes[i][j].getName() == "emissive")
					{
						if (attributes[i][j].getType() == MaterialAttribute::Vector3)
						{
							s += "fs_EmissiveMetallic.xyz = emissive;\n";
						}
						else if (attributes[i][j].getType() == MaterialAttribute::Texture)
						{
							s += "fs_EmissiveMetallic.xyz = texture(emissive, fs_TexCoord0).rgb;\n";
						}
						else if (attributes[i][j].getType() == MaterialAttribute::Number)
						{
							s += "fs_EmissiveMetallic.xyz = vec3(emissive);\n";
						}
					}
				}

				s += "}";

				char* str = new char[s.length() + 1];
				strcpy(str, s.c_str());
				shaders[i].createProgramFromSource(Shader::Fragment, str, vertexDefines);
			}
			else
			{
				shaders[i].createProgram(Shader::Fragment, fragment);
			}
		}

		for (uint32 i = 0; i < numMaterials; i++)
		{
			Technique* technique = new Technique(shaders[i]);
			(*materialMap)->addTechnique(technique);
			for (uint32 j = 0; j < attributes[i].size(); j++)
			{
				technique->addAttribute(attributes[i][j]);
			}
		}

		Mesh* mesh = new Mesh(format, Mesh::Triangles);

		for (uint32 td = 0; td < totalIndexData.size(); td++)
		{
			mesh->addIndexComponent(Mesh::Triangles)->setIndexBuffer(IndexBuffer::Index32, totalIndexData[td].size(), false, &totalIndexData[td][0]);
		}

		LOGF(Info, CoreEngine, "vertexData: %d, vertexSize: %d, vertexCount: %d", vertexData.size(), (format.getVertexSize() / sizeof(float)), vertexData.size() / (format.getVertexSize() / sizeof(float)));
		mesh->setVertexBuffer(format, vertexData.size() / (format.getVertexSize() / sizeof(float)))->setVertexData(&vertexData[0]);

		MeshStruct meshStruct;
		meshStruct.mesh = mesh;
		meshStruct.materialMap = *materialMap;

		return meshStruct;
	}
}
