
namespace Arcana
{
	template<typename DataType>
	MarkovChain<DataType>::MarkovChain()
	{

	}

	template<typename DataType>
	MarkovChain<DataType>::~MarkovChain()
	{
		_currentState = nullptr;
		_states.clear();
		_probabilities.clear();
	}

	template<typename DataType>
	void MarkovChain<DataType>::addState(uint32 id, const State& state)
	{
		state.id = id;
		_states.insert(id, state);
	}

	template<typename DataType>
	void MarkovChain<DataType>::addState(uint32 id, const DataType& data)
	{
		State state;
		state.id = id;
		state.data = data;
		_states.emplace(id, state);
	}

	template<typename DataType>
	void MarkovChain<DataType>::addTransition(uint32 id0, uint32 id1, double probability)
	{
		_probabilities[id0][id1] = probability;
	}

	template<typename DataType>
	void MarkovChain<DataType>::setStartingState(uint32 id)
	{
		_currentState = &_states.at(id);
	}

	template<typename DataType>
	uint32 MarkovChain<DataType>::getNumStates() const
	{
		return _states.size();
	}

	template<typename DataType>
	DataType MarkovChain<DataType>::get()
	{
		if(_currentState)
		{
			std::map<uint32, double> probabilities = _probabilities[(*_currentState).id];

			std::map<uint32, double>::iterator iter;
			double last = 0.0;
			for (iter = probabilities.begin(); iter != probabilities.end(); iter++)
			{
				iter->second += last;
				last += iter->second;
			}

			double r = Random<double>::random();

			for(iter = probabilities.begin(); iter != probabilities.end(); iter++)
			{
				if(r < iter->second)
				{
					_currentState = &_states[iter->first];

					return (*_currentState).data;
				}
			}
		}

		return DataType();
	}

	template<typename DataType>
	std::vector<DataType> MarkovChain<DataType>::get(uint32 num, bool includeCurrent)
	{
		std::vector<DataType> data;

		if(_currentState)
		{
			if (includeCurrent)
			{
				data.push_back((*_currentState).data);
			}

			for(uint32 i = 0; i < num; i++)
			{
				data.push_back(get());
			}
		}

		return data;
	}
}