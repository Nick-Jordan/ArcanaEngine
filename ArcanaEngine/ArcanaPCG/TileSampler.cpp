#include "TileSampler.h"

#include "TextureTileStorage.h"

namespace Arcana
{

	TileSampler::TileSampler(const std::string& name, TileProducer* producer) : Object("TileSampler"), _name(name), _producer(producer), _root(nullptr)
	{
		_producer->reference();


		_storeInvisible = true;
	}


	TileSampler::~TileSampler()
	{
		if (_root)
		{
			_root->recursiveDelete(this);
		}

		AE_RELEASE(_producer);

		for (auto i = _storeFilters.createIterator(); i; i++)
		{
			AE_RELEASE(*i);
		}
	}

	void TileSampler::update(TerrainNode* root, Material* material)
	{
		_producer->update();

		TextureUpdater::instance().updateTextures();
	}

	TileProducer* TileSampler::getProducer() const
	{
		return _producer;
	}

	void TileSampler::setProducer(TileProducer* producer)
	{
		AE_RELEASE(_producer);

		_producer = producer;
		_producer->reference();
	}

	void TileSampler::setStoreTileFilter(TileFilter* filter)
	{
		if (filter)
		{
			filter->reference();
			_storeFilters.add(filter);
		}
	}

	void TileSampler::setTile(Material* material, int32 level, int32 tx, int32 ty, Vector3d physicalPosition)
	{
		Shader* shader = material->getCurrentTechnique()->getPass(0);

		Tile* tile = _producer->getTile(level, tx, ty);

		TextureTileStorage::TextureSlot *gput = dynamic_cast<TextureTileStorage::TextureSlot*>(tile->getData());
		if (gput)
		{

			int32 unit = gput->texture->bind(material);

			shader->getUniform(_name + ".tilePool")->setValue(unit);
			shader->getUniform(_name + ".tileLayer")->setValue((float)gput->l);
		}
		//shader->getUniform(_name + ".tileSize")->setValue(Vector3f(gput->getWidth(), gput->getHeight(), 1.0f));

		/*Tile* t = nullptr;
		int32 b = _producer->getBorder();
		int32 s = _producer->getCache()->getStorage()->getTileSize();

		float dx = 0;
		float dy = 0;
		float dd = 1;
		float ds0 = (s / 2) * 2.0f - 2.0f * b;
		float ds = ds0;
		while (!_producer->hasTile(level, tx, ty))
		{
			dx += (tx % 2) * dd;
			dy += (ty % 2) * dd;
			dd *= 2;
			ds /= 2;
			level -= 1;
			tx /= 2;
			ty /= 2;
			AE_ASSERT(level >= 0);
		}

		Tree* tt = _root;
		Tree* tc;
		int32 tl = 0;
		while (tl != level && (tc = tt->children[((tx >> (level - tl - 1)) & 1) | ((ty >> (level - tl - 1)) & 1) << 1]) != nullptr)
		{
			tl += 1;
			tt = tc;
		}
		while (level > tl)
		{
			dx += (tx % 2) * dd;
			dy += (ty % 2) * dd;
			dd *= 2;
			ds /= 2;
			level -= 1;
			tx /= 2;
			ty /= 2;
		}
		t = tt->t;

		while (t == nullptr)
		{
			dx += (tx % 2) * dd;
			dy += (ty % 2) * dd;
			dd *= 2;
			ds /= 2;
			level -= 1;
			tx /= 2;
			ty /= 2;
			tt = tt->parent;
			AE_ASSERT(tt != nullptr);
			t = tt->t;
		}

		dx = dx * ((s / 2) * 2 - 2 * b) / dd;
		dy = dy * ((s / 2) * 2 - 2 * b) / dd;

		TextureTileStorage::TextureSlot *gput = dynamic_cast<TextureTileStorage::TextureSlot*>(t->getData());
		AE_ASSERT(gput != nullptr);

		float w = gput->getWidth();
		float h = gput->getHeight();
		AE_ASSERT(w == h);

		Vector4f coords;
		if (s % 2 == 0)
		{
			coords = Vector4f((dx + b) / w, (dy + b) / h, float(gput->l), ds / w);
		}
		else
		{
			coords = Vector4f((dx + b + 0.5f) / w, (dy + b + 0.5f) / h, float(gput->l), ds / w);
		}

		shader->getUniform(_name + ".tilePool")->setValue(gput->texture);
		shader->getUniform(_name + ".tileCoords")->setValue(Vector3f(coords.x, coords.y, coords.z));
		shader->getUniform(_name + ".tileSize")->setValue(Vector3f(coords.w, coords.w, (s / 2) * 2.0f - 2.0f * b));*/
	}

	TileSampler::Tree::Tree(Tree *parent) : newTree(true), needTile(false), parent(parent), t(nullptr)
	{
		children[0] = nullptr;
		children[1] = nullptr;
		children[2] = nullptr;
		children[3] = nullptr;
	}

	TileSampler::Tree::~Tree()
	{
	}

	void TileSampler::Tree::recursiveDelete(TileSampler *owner)
	{
		if (t != nullptr)
		{
			owner->_producer->setTileUnused(t);
			t = nullptr;
		}
		if (children[0] != nullptr)
		{
			children[0]->recursiveDelete(owner);
			children[1]->recursiveDelete(owner);
			children[2]->recursiveDelete(owner);
			children[3]->recursiveDelete(owner);
		}
		delete this;
	}
}
