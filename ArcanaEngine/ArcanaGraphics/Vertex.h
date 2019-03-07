#ifndef VERTEX_H_
#define VERTEX_H_

#include "GraphicsDefines.h"

#include "Vector3.h"
#include "Vector4.h"

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

	struct ARCANA_GRAPHICS_API ParticleVertex
	{
		Vector3f position;
		Vector3f size;
		Vector4f color;

		Vector3f rotation0;
		Vector3f rotation1;
		Vector3f rotation2;
	};

	struct VertexBlendingInfo
	{
		uint32 blendingIndex;
		double blendingWeight;

		VertexBlendingInfo() :
			blendingIndex(0),
			blendingWeight(0.0)
		{}

		bool operator < (const VertexBlendingInfo& rhs)
		{
			return (blendingWeight > rhs.blendingWeight);
		}
	};

	struct FBXVertex
	{
		Vector3f position;
		Vector3f normal;
		Vector2f uv;

		bool operator==(const FBXVertex& rhs) const
		{
			bool result1 = position == rhs.position;
			bool result2 = normal == rhs.normal;
			bool result3 = uv == rhs.uv;

			return result1 && result2 && result3;
		}
	};

	struct FBXBlendVertex
	{
		FBXVertex vertex;
		std::vector<VertexBlendingInfo> vertexBlendingInfos;

		void sortBlendingInfoByWeight()
		{
			std::sort(vertexBlendingInfos.begin(), vertexBlendingInfos.end());
		}

		bool operator==(const FBXBlendVertex& rhs) const
		{
			bool sameBlendingInfo = true;

			if (!(vertexBlendingInfos.empty() && rhs.vertexBlendingInfos.empty()))
			{
				for (uint32 i = 0; i < 4; ++i)
				{
					if (vertexBlendingInfos[i].blendingIndex != rhs.vertexBlendingInfos[i].blendingIndex ||
						abs(vertexBlendingInfos[i].blendingWeight - rhs.vertexBlendingInfos[i].blendingWeight) > 0.001)
					{
						sameBlendingInfo = false;
						break;
					}
				}
			}

			

			return vertex == rhs.vertex && sameBlendingInfo;
		}
	};
}

#endif // !VERTEX_H_