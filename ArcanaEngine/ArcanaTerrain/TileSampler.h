#ifndef TILE_SAMPLER_H_
#define TILE_SAMPLER_H_

#include "TerrainDefines.h"

#include "Object.h"
#include "TerrainNode.h"
#include "TileProducer.h"
#include "Material.h"

namespace Arcana
{
	class ARCANA_TERRAIN_API TileSampler : public Object
	{
	public:

		class TileFilter : public Object
		{
		public:

			virtual bool storeTile(TerrainQuad* quad) = 0;
		};

		TileSampler(const std::string& name, TileProducer* producer);

		virtual ~TileSampler();

		void update(TerrainNode* root, Material* material);

		TileProducer* getProducer() const;

		void setProducer(TileProducer* producer);

		void setStoreTileFilter(TileFilter* filter);

		void setTile(Material* material, int32 level, int32 tx, int32 ty, int32 childIndex);

	private:

		class Tree 
		{
		public:

			bool newTree;
			bool needTile;
			Tree *parent;
			Tile *t;
			Tree *children[4];
			Tree(Tree *parent);
			virtual ~Tree();
			virtual void recursiveDelete(TileSampler *owner);
		};

	private:

		TileProducer* _producer;

		Tree* _root;

		std::string _name;

		Array<TileFilter*> _storeFilters;

		bool _storeInvisible;
	};
}

#endif // !TILE_SAMPLER_H_

