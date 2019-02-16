#include "CellularAutomata.h"

namespace Arcana
{
	CellularAutomata::CellularAutomata(int32 width, int32 height, const Array<Rule>& ruleSet, CellsArray startingState)
		: _width(width), _height(height), _ruleSet(ruleSet)
	{
		if(startingState.isEmpty())
		{
			_cells = CellsArray(width, height);
		}
	}

	CellularAutomata::~CellularAutomata()
	{

	}

	void CellularAutomata::step()
	{
		std::vector<std::vector<bool>> update;
		update.resize(_width);
		for(uint32 i = 0; i < _width; i++)
			update[i].resize(_height);

		for (uint32 i = 0; i < _cells.size(); i++)
		{
			for (uint32 j = 0; j < _cells.size(i); j++)
			{
				int32 numNeighbors = checkNeighbors(i, j);

				bool updated = false;
				for(auto r = _ruleSet.createConstIterator(); r; r++)
				{
					Rule rule = *r;

					if(rule.check(_cells.getCell(i, j).alive, numNeighbors))
					{
						update[i][j] = rule.getResult();
						updated = true;
					}
				}
				
				if(!updated) 
				{
					update[i][j] = _cells.getCell(i, j).alive;
				}
			}
		}

		for (int32 i = 0; i < _cells.size(); i++)
		 {
			for (int32 j = 0; j < _cells.size(i); j++)
			 {
				_cells.getCell(i, j).alive = update[i][j];
			}
		}
	}

	CellularAutomata::CellsArray& CellularAutomata::getCells()
	{
		return _cells;
	}

	const CellularAutomata::Cell& CellularAutomata::getCell(uint32 i, uint32 j)
	{
		return _cells.getCell(i, j);
	}

	bool CellularAutomata::isCellAlive(uint32 i, uint32 j)
	{
		return _cells.getCell(i, j).alive;
	}

	int32 CellularAutomata::checkNeighbors(int32 i, int32 j)
	{
		int32 num = 0;

		if (i > 0) 
		{
			if (_cells.getCell(i - 1, j).alive)
				num++;
		}

		if (j > 0)
		 {
			if (_cells.getCell(i, j - 1).alive)
				num++;
		}

		if (i < _width - 1) 
		{
			if (_cells.getCell(i + 1, j).alive)
				num++;
		}

		if (j < _height - 1) 
		{
			if (_cells.getCell(i, j + 1).alive)
				num++;
		}

		if (i < _width - 1 && j < _height - 1) 
		{
			if (_cells.getCell(i + 1, j + 1).alive)
				num++;
		}

		if (i > 0 && j > 0) 
		{
			if (_cells.getCell(i - 1, j - 1).alive)
				num++;
		}

		if (i > 0 && j < _height - 1)
		 {
			if (_cells.getCell(i - 1, j + 1).alive)
				num++;
		}

		if (i < _width - 1 && j > 0)
		 {
			if (_cells.getCell(i + 1, j - 1).alive)
				num++;
		}

		return num;
	}

	CellularAutomata::CellsArray::CellsArray()
	{

	}

	CellularAutomata::CellsArray::CellsArray(int32 width, int32 height)
	{
		_state.resize(width);
		for(uint32 i = 0; i < width; i++)
			_state[i].resize(height);
	}

	CellularAutomata::CellsArray::~CellsArray()
	{

	}

	CellularAutomata::Cell& CellularAutomata::CellsArray::getCell(uint32 i, uint32 j)
	{
		if(i < _state.size())
		{
			if(j < _state[i].size())
			{
				return _state[i][j];
			}
		}

		return Cell(0, 0);
	}

	void CellularAutomata::CellsArray::setCell(uint32 i, uint32 j, Cell cell)
	{
		setCell(i, j, cell.alive);
	}

	void CellularAutomata::CellsArray::setCell(uint32 i, uint32 j, bool alive)
	{
		getCell(i, j).alive = alive;
	}

	bool CellularAutomata::CellsArray::isEmpty() const
	{
		return _state.empty();
	}

	uint32 CellularAutomata::CellsArray::size()
	{
		return _state.size();
	}

	uint32 CellularAutomata::CellsArray::size(uint32 i)
	{
		if (i < _state.size())
		{
			return _state[i].size();
		}
		return 0;
	}

	CellularAutomata::Rule::Rule(bool isAlive, int32 numNeighbors, Comparator comparator, bool result)
		: _isAlive(isAlive), _numNeighbors(numNeighbors), _comparator(comparator), _result(result)
	{
		
	}
	
	bool CellularAutomata::Rule::check(bool alive, int32 neighbors)
	{
		bool neighborCompare = false;
		
		switch(_comparator) 
		{
		case Equals:
			neighborCompare = neighbors == _numNeighbors;
			break;
		case NotEqual:
			neighborCompare = neighbors != _numNeighbors;
			break;
		case GreaterThan:
			neighborCompare = neighbors > _numNeighbors;
			break;
		case GreaterThanOrEqualTo:
			neighborCompare = neighbors >= _numNeighbors;
			break;
		case LessThan:
			neighborCompare = neighbors < _numNeighbors;
			break;
		case LessThanOrEqualTo:
			neighborCompare = neighbors <= _numNeighbors;
			break;
		};
		
		return alive == _isAlive && neighborCompare;
	}

	bool CellularAutomata::Rule::getResult() const
	{
		return _result;
	}
}