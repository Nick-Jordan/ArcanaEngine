#ifndef ELEVATION_PRODUCER_H_
#define ELEVATION_PRODUCER_H_

#include "PCGDefines.h"

#include "TileProducer.h"

namespace Arcana
{

	class ARCANA_PCG_API ElevationProducer : public TileProducer
	{
	public:

		ElevationProducer(TileCache* cache, int32 gridMeshSize, bool flipDiagonals = false);

		virtual ~ElevationProducer();

		virtual int32 getBorder() const override;

	protected:

		ElevationProducer();

		virtual void runCreateTile(int32 level, int32 tx, int32 ty, TileStorage::Slot *data) override;

	private:

		int32 _face;
		int32 _gridMeshSize;
		bool _flipDiagonals;
	};

}

#endif // !ELEVATION_PRODUCER_H_

