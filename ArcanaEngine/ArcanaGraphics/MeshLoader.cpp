#include "MeshLoader.h"

#include "MeshIndexComponent.h"

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

	//test///////////////////////////////////////
	FbxString getAttributeTypeName(FbxNodeAttribute::EType type) {
		switch (type) {
		case FbxNodeAttribute::eUnknown: return "unidentified";
		case FbxNodeAttribute::eNull: return "null";
		case FbxNodeAttribute::eMarker: return "marker";
		case FbxNodeAttribute::eSkeleton: return "skeleton";
		case FbxNodeAttribute::eMesh: return "mesh";
		case FbxNodeAttribute::eNurbs: return "nurbs";
		case FbxNodeAttribute::ePatch: return "patch";
		case FbxNodeAttribute::eCamera: return "camera";
		case FbxNodeAttribute::eCameraStereo: return "stereo";
		case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
		case FbxNodeAttribute::eLight: return "light";
		case FbxNodeAttribute::eOpticalReference: return "optical reference";
		case FbxNodeAttribute::eOpticalMarker: return "marker";
		case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
		case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
		case FbxNodeAttribute::eBoundary: return "boundary";
		case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
		case FbxNodeAttribute::eShape: return "shape";
		case FbxNodeAttribute::eLODGroup: return "lodgroup";
		case FbxNodeAttribute::eSubDiv: return "subdiv";
		default: return "unknown";
		}
	}

	void printAttribute(FbxNodeAttribute* pAttribute) {
		if (!pAttribute) return;

		FbxString typeName = getAttributeTypeName(pAttribute->GetAttributeType());
		FbxString attrName = pAttribute->GetName();
		LOGF(Info, CoreEngine, "attribute:  %s, %s", typeName.Buffer(), attrName.Buffer());
	}
	void testPrintNode(FbxNode* pNode)
	{
		const char* nodeName = pNode->GetName();
		FbxDouble3 translation = pNode->LclTranslation.Get();
		FbxDouble3 rotation = pNode->LclRotation.Get();
		FbxDouble3 scaling = pNode->LclScaling.Get();

		// Print the contents of the node.
		LOGF(Info, CoreEngine, "node: %s, %f, %f, %f, %f, %f, %f, %f, %f, %f", nodeName, 
			translation[0], translation[1], translation[2],
			rotation[0], rotation[1], rotation[2],
			scaling[0], scaling[1], scaling[2]);

		// Print the node's attributes.
		for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
			printAttribute(pNode->GetNodeAttributeByIndex(i));

		// Recursively print the children.
		for (int j = 0; j < pNode->GetChildCount(); j++)
			testPrintNode(pNode->GetChild(j));
	}
	//test///////////////////////////////////////
	
	MeshStruct MeshLoader::createMesh(const std::string& path, const Shader& shader)
	{
		const char* filename = path.c_str();

		FbxManager* sdkManager = FbxManager::Create();

		FbxIOSettings *ios = FbxIOSettings::Create(sdkManager, IOSROOT);
		sdkManager->SetIOSettings(ios);

		FbxImporter* importer = FbxImporter::Create(sdkManager, "");

		if (!importer->Initialize(filename, -1, sdkManager->GetIOSettings()))
		{
			LOG(Error, CoreEngine, "Failed to initialize mesh importer!");
			LOGF(Error, CoreEngine, "Error returned: %s\n\n", importer->GetStatus().GetErrorString());

			return MeshStruct();
		}

		FbxScene* scene = FbxScene::Create(sdkManager, filename);

		importer->Import(scene);

		importer->Destroy();

		MeshRootData rootData;

		FbxNode* rootNode = scene->GetRootNode();
		
		MaterialMap* material = new MaterialMap("material");
		processNode(rootNode, rootData, material, shader);
		optimize(rootData);

		VertexFormat::Attribute attribs[]
		{
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 3),
			VertexFormat::Attribute(VertexFormat::Semantic::Normal, 3),
			//VertexFormat::Attribute(VertexFormat::Semantic::Tangent, 3),
			//VertexFormat::Attribute(VertexFormat::Semantic::Binormal, 3),
			VertexFormat::Attribute(VertexFormat::Semantic::TexCoord0, 2),
		};

		VertexFormat format(3, attribs);

		Mesh* mesh = new Mesh(format, Mesh::Triangles);

		std::vector<FBXVertex> vertices;
		for (uint32 i = 0; i < rootData.vertices.size(); i++)
		{
			vertices.push_back(rootData.vertices[i].vertex);
		}

		for (uint32 td = 0; td < rootData.triangleData.size(); td++)
		{
			std::vector<uint32> indices;
			for (uint32 i = 0; i < rootData.triangleData[td].triangles.size(); i++)
			{
				indices.insert(indices.begin(), rootData.triangleData[td].triangles[i].indices.begin(), rootData.triangleData[td].triangles[i].indices.end());
			}
			mesh->addIndexComponent(Mesh::Triangles)->setIndexBuffer(IndexBuffer::Index32, indices.size(), false, &indices[0]);
		}

		mesh->setVertexBuffer(format, vertices.size())->setVertexData(&vertices[0].position.x);

		sdkManager->Destroy();

		MeshStruct meshStruct;
		meshStruct.mesh = mesh;
		meshStruct.materialMap = material;

		return meshStruct;
	}

	void MeshLoader::processNode(FbxNode* node, MeshRootData& rootData, MaterialMap* material, const Shader& shader)
	{
		if (node->GetNodeAttribute())
		{
			switch (node->GetNodeAttribute()->GetAttributeType())
			{
			case FbxNodeAttribute::eMesh:

				processControlPoints(node, rootData);

				//if (mHasAnimation)
				//{
				//	ProcessJointsAndAnimations(inNode);
				//}
				processMesh(node, rootData);
				//associateMaterials(node, rootData.triangleData.back());
				processMaterials(node, material, shader);
				break;
			}
		}

		for (uint32 i = 0; i < node->GetChildCount(); i++)
		{
			processNode(node->GetChild(i), rootData, material, shader);
		}
	}

	void MeshLoader::processControlPoints(FbxNode* inNode, MeshRootData& rootData)
	{
		FbxMesh* currMesh = inNode->GetMesh();
		uint32 ctrlPointCount = currMesh->GetControlPointsCount();
		for (uint32 i = 0; i < ctrlPointCount; ++i)
		{
			ControlPoint point;
			point.position.x = static_cast<float>(currMesh->GetControlPointAt(i).mData[0]);
			point.position.y = static_cast<float>(currMesh->GetControlPointAt(i).mData[1]);
			point.position.z = static_cast<float>(currMesh->GetControlPointAt(i).mData[2]);
			rootData.controlPoints.push_back(point);
		}
	}

	void MeshLoader::processMesh(FbxNode* inNode, MeshRootData& rootData)
	{
		MeshTriangleData triangleData;

		FbxMesh* currMesh = inNode->GetMesh();

		uint32 triangleCount = currMesh->GetPolygonCount();
		int32 vertexCounter = 0;
		triangleData.triangles.reserve(triangleCount);

		for (uint32 i = 0; i < triangleCount; ++i)
		{
			Vector3f normal[3];
			Vector3f tangent[3];
			Vector3f binormal[3];
			Vector2f UV[3][2];
			Triangle triangle;
			triangleData.triangles.push_back(triangle);

			for (uint32 j = 0; j < 3; ++j)
			{
				int32 ctrlPointIndex = currMesh->GetPolygonVertex(i, j);
				ControlPoint point = rootData.controlPoints[ctrlPointIndex];

				readNormal(currMesh, ctrlPointIndex, vertexCounter, normal[j]);

				for (int32 k = 0; k < 1; ++k)
				{
					UV[j][k] = Vector2f::zero();
					//readUV(currMesh, ctrlPointIndex, currMesh->GetTextureUVIndex(i, j), k, UV[j][k]);
				}


				FBXBlendVertex temp;
				temp.vertex.position = Vector3f(point.position.x, point.position.y, point.position.z);
				temp.vertex.normal = normal[j];
				temp.vertex.uv = UV[j][0];

				for (uint32 i = 0; i < point.blendingInfo.size(); ++i)
				{
					VertexBlendingInfo currBlendingInfo;
					currBlendingInfo.blendingIndex = point.blendingInfo[i].blendingIndex;
					currBlendingInfo.blendingWeight = point.blendingInfo[i].blendingWeight;
					temp.vertexBlendingInfos.push_back(currBlendingInfo);
				}

				temp.sortBlendingInfoByWeight();

				rootData.vertices.push_back(temp);
				triangleData.triangles.back().indices.push_back(vertexCounter);
				vertexCounter++;
			}
		}

		rootData.controlPoints.clear();

		rootData.triangleData.push_back(triangleData);
	}

	void MeshLoader::readUV(FbxMesh* mesh, int32 ctrlPointIndex, int32 textureUVIndex, int32 uvLayer, Vector2f& uv)
	{
		if (uvLayer >= 2 || mesh->GetElementUVCount() <= uvLayer)
		{
			LOG(Error, MeshLoaderLog, "Invalid UV Layer Number");
		}
		FbxGeometryElementUV* vertexUV = mesh->GetElementUV(uvLayer);

		switch (vertexUV->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
			switch (vertexUV->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				uv.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(ctrlPointIndex).mData[0]);
				uv.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(ctrlPointIndex).mData[1]);
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
			{
				int32 index = vertexUV->GetIndexArray().GetAt(ctrlPointIndex);
				uv.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
				uv.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
			}
			break;

			default:
				LOG(Error, MeshLoaderLog, "Invalid Reference");
			}
			break;

		case FbxGeometryElement::eByPolygonVertex:
			switch (vertexUV->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			case FbxGeometryElement::eIndexToDirect:
			{
				uv.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(textureUVIndex).mData[0]);
				uv.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(textureUVIndex).mData[1]);
			}
			break;

			default:
				LOG(Error, MeshLoaderLog, "Invalid Reference");
			}
			break;
		}
	}

	void MeshLoader::readNormal(FbxMesh* mesh, int32 ctrlPointIndex, int32 vertexCounter, Vector3f& normal)
	{
		if (mesh->GetElementNormalCount() < 1)
		{
			LOG(Error, MeshLoaderLog, "Invalid Normal Number");
		}

		FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
		switch (vertexNormal->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
			switch (vertexNormal->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				normal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(ctrlPointIndex).mData[0]);
				normal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(ctrlPointIndex).mData[1]);
				normal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(ctrlPointIndex).mData[2]);
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
			{
				int32 index = vertexNormal->GetIndexArray().GetAt(ctrlPointIndex);
				normal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
				normal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
				normal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
			}
			break;

			default:
				LOG(Error, MeshLoaderLog, "Invalid Reference");
			}
			break;

		case FbxGeometryElement::eByPolygonVertex:
			switch (vertexNormal->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				normal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[0]);
				normal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[1]);
				normal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[2]);
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
			{
				int32 index = vertexNormal->GetIndexArray().GetAt(vertexCounter);
				normal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
				normal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
				normal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
			}
			break;

			default:
				LOG(Error, MeshLoaderLog, "Invalid Reference");
			}
			break;
		}
	}

	void MeshLoader::readBinormal(FbxMesh* mesh, int32 ctrlPointIndex, int32 vertexCounter, Vector3f& binormal)
	{
		if (mesh->GetElementBinormalCount() < 1)
		{
			LOG(Error, MeshLoaderLog, "Invalid Binormal Number");
		}

		FbxGeometryElementBinormal* vertexBinormal = mesh->GetElementBinormal(0);
		switch (vertexBinormal->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
			switch (vertexBinormal->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				binormal.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(ctrlPointIndex).mData[0]);
				binormal.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(ctrlPointIndex).mData[1]);
				binormal.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(ctrlPointIndex).mData[2]);
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
			{
				int32 index = vertexBinormal->GetIndexArray().GetAt(ctrlPointIndex);
				binormal.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
				binormal.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
				binormal.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);
			}
			break;

			default:
				LOG(Error, MeshLoaderLog, "Invalid Reference");
			}
			break;

		case FbxGeometryElement::eByPolygonVertex:
			switch (vertexBinormal->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				binormal.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCounter).mData[0]);
				binormal.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCounter).mData[1]);
				binormal.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCounter).mData[2]);
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
			{
				int32 index = vertexBinormal->GetIndexArray().GetAt(vertexCounter);
				binormal.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
				binormal.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
				binormal.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);
			}
			break;

			default:
				LOG(Error, MeshLoaderLog, "Invalid Reference");
			}
			break;
		}
	}

	void MeshLoader::readTangent(FbxMesh* mesh, int32 ctrlPointIndex, int32 vertexCounter, Vector3f& tangent)
	{
		if (mesh->GetElementTangentCount() < 1)
		{
			LOG(Error, MeshLoaderLog, "Invalid Tangent Number");
		}

		FbxGeometryElementTangent* vertexTangent = mesh->GetElementTangent(0);
		switch (vertexTangent->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
			switch (vertexTangent->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				tangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(ctrlPointIndex).mData[0]);
				tangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(ctrlPointIndex).mData[1]);
				tangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(ctrlPointIndex).mData[2]);
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
			{
				int32 index = vertexTangent->GetIndexArray().GetAt(ctrlPointIndex);
				tangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
				tangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
				tangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
			}
			break;

			default:
				LOG(Error, MeshLoaderLog, "Invalid Reference");
			}
			break;

		case FbxGeometryElement::eByPolygonVertex:
			switch (vertexTangent->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				tangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertexCounter).mData[0]);
				tangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertexCounter).mData[1]);
				tangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertexCounter).mData[2]);
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
			{
				int32 index = vertexTangent->GetIndexArray().GetAt(vertexCounter);
				tangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
				tangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
				tangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
			}
			break;

			default:
				LOG(Error, MeshLoaderLog, "Invalid Reference");
			}
			break;
		}
	}

	void MeshLoader::optimize(MeshRootData& rootData)
	{
		std::vector<FBXBlendVertex> uniqueVertices;
		for (uint32 td = 0; td < rootData.triangleData.size(); td++)
		{
			for (uint32 i = 0; i < rootData.triangleData[td].triangles.size(); i++)
			{
				for (uint32 j = 0; j < 3; j++)
				{
					if (findVertex(rootData.vertices[i * 3 + j], uniqueVertices) == -1)
					{
						uniqueVertices.push_back(rootData.vertices[i * 3 + j]);
					}
				}
			}
		}

		for (uint32 td = 0; td < rootData.triangleData.size(); td++)
		{
			for (uint32 i = 0; i < rootData.triangleData[td].triangles.size(); i++)
			{
				for (uint32 j = 0; j < 3; j++)
				{
					rootData.triangleData[td].triangles[i].indices[j] = findVertex(rootData.vertices[i * 3 + j], uniqueVertices);
				}
			}
		}

		rootData.vertices.clear();
		rootData.vertices = uniqueVertices;
		uniqueVertices.clear();

		for (uint32 td = 0; td < rootData.triangleData.size(); td++)
		{
			std::sort(rootData.triangleData[td].triangles.begin(), rootData.triangleData[td].triangles.end());
		}
	}

	int32 MeshLoader::findVertex(const FBXBlendVertex& targetVertex, const std::vector<FBXBlendVertex>& uniqueVertices)
	{
		for (unsigned int i = 0; i < uniqueVertices.size(); ++i)
		{
			if (targetVertex == uniqueVertices[i])
			{
				return i;
			}
		}

		return -1;
	}

	void MeshLoader::processMaterials(FbxNode* node, MaterialMap* material, const Shader& shader)
	{
		uint32 materialCount = node->GetMaterialCount();

		for (uint32 i = 0; i < materialCount; ++i)
		{
			Technique* technique = new Technique(shader);
			material->addTechnique(technique);

			FbxSurfaceMaterial* surfaceMaterial = node->GetMaterial(i);
			processMaterialAttribute(surfaceMaterial, i, material);
			//processMaterialTexture(surfaceMaterial, mMaterialLookUp[i]);
		}
	}

	void MeshLoader::associateMaterials(FbxNode* node, MeshTriangleData& data)
	{
		FbxLayerElementArrayTemplate<int32>* materialIndices;
		FbxGeometryElement::EMappingMode materialMappingMode = FbxGeometryElement::eNone;
		FbxMesh* currMesh = node->GetMesh();

		if (currMesh->GetElementMaterial())
		{
			materialIndices = &(currMesh->GetElementMaterial()->GetIndexArray());
			materialMappingMode = currMesh->GetElementMaterial()->GetMappingMode();

			if (materialIndices)
			{
				switch (materialMappingMode)
				{
				case FbxGeometryElement::eByPolygon:
				{
					if (materialIndices->GetCount() == data.triangles.size())
					{
						for (uint32 i = 0; i < data.triangles.size(); i++)
						{
							uint32 materialIndex = materialIndices->GetAt(i);
							data.triangles[i].materialIndex = materialIndex;
						}
					}
				}
				break;

				case FbxGeometryElement::eAllSame:
				{
					uint32 materialIndex = materialIndices->GetAt(0);
					for (uint32 i = 0; i < data.triangles.size(); ++i)
					{
						data.triangles[i].materialIndex = materialIndex;
					}
				}
				break;

				default:
					LOG(Error, MeshLoaderLog, "Invalid mapping mode for material.");
				}
			}
		}
	}

	void MeshLoader::processMaterialAttribute(FbxSurfaceMaterial* fbxMaterial, uint32 materialIndex, MaterialMap* material)
	{
		FbxDouble3 vec3;
		FbxDouble scalar;

		FbxSurfaceLambert* lambert = reinterpret_cast<FbxSurfaceLambert*>(fbxMaterial);
		
		if (lambert)
		{
			vec3 = lambert->Diffuse;
			LOGF(Info, CoreEngine, "lambert diffuse: %f, %f, %f", static_cast<float>(vec3.mData[0]), static_cast<float>(vec3.mData[1]), static_cast<float>(vec3.mData[2]));
			material->addAttribute("baseColor",
				Vector3f(static_cast<float>(vec3.mData[0]), static_cast<float>(vec3.mData[1]), static_cast<float>(vec3.mData[2])),
				material->getTechniqueCount() - 1);
		}
	}
}
