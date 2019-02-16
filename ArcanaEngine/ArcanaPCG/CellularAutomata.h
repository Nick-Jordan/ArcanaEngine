#ifndef CELLULAR_AUTOMATA_H_
#define CELLULAR_AUTOMATA_H_

#include "PCGDefines.h"

#include "Types.h"
#include "Array.h"
#include <vector>

namespace Arcana
{

	class ARCANA_PCG_API CellularAutomata
	{
	public:

		class ARCANA_PCG_API Rule
		{
		public:

			enum ARCANA_PCG_API Comparator
			{
				Equals,
				NotEqual,
				GreaterThan,
				GreaterThanOrEqualTo,
				LessThan,
				LessThanOrEqualTo
			};

			Rule(bool isAlive, int32 numNeighbors, Comparator comparator, bool result);

			bool check(bool alive, int32 neighbors);

			bool getResult() const;

		private:

			bool _isAlive;
			int32 _numNeighbors;
			bool _result;
			Comparator _comparator;
		};

		class ARCANA_PCG_API Cell
		{
		public:

			bool alive;
			int32 x;
			int32 y;

			Cell() : x(0), y(0), alive(false) {};

			Cell(int32 x, int32 y) : x(x), y(y), alive(false) {};
	
			Cell(int32 x, int32 y, bool alive) : x(x), y(y), alive(alive) {};
		};

		class ARCANA_PCG_API CellsArray
		{
		public:

			CellsArray();

			CellsArray(int32 width, int32 height);

			~CellsArray();

			Cell& getCell(uint32 i, uint32 j);

			void setCell(uint32 i, uint32 j, Cell cell);

			void setCell(uint32 i, uint32 j, bool alive);

			bool isEmpty() const;

			uint32 size();

			uint32 size(uint32 i);

		private:

			std::vector<std::vector<Cell>> _state;
		};


		CellularAutomata(int32 width, int32 height, const Array<Rule>& ruleSet, CellsArray startingState = CellsArray());

		~CellularAutomata();

		void step();

		CellsArray& getCells();

		const Cell& getCell(uint32 i, uint32 j);

		bool isCellAlive(uint32 i, uint32 j);

	private:

		int32 checkNeighbors(int32 i, int32 j);

	private:

		int32 _width;
		int32 _height;

		CellsArray _cells;

		Array<Rule> _ruleSet;
	};
}

#endif // !CELLULAR_AUTOMATA_H_