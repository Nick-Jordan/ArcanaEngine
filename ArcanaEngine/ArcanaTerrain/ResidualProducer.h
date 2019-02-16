#ifndef RESIDUAL_PRODUCER_H_
#define RESIDUAL_PRODUCER_H_

#include "TerrainDefines.h"

#include "TileProducer.h"
#include "TextureTileStorage.h"

namespace Arcana
{

	class ARCANA_TERRAIN_API ResidualProducer : public TileProducer
	{
	public:

		ResidualProducer(TileCache* cache, std::string name, std::string ext);

		virtual ~ResidualProducer();

		virtual int32 getBorder() const override;

	protected:

		ResidualProducer();

		virtual void runCreateTile(int32 level, int32 tx, int32 ty, TileStorage::Slot *data) override;

	private:

		std::string _name;
		std::string _ext;
	};

}

#endif // !ELEVATION_PRODUCER_H_

