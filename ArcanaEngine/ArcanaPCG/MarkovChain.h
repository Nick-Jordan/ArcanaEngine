#ifndef MARKOV_CHAIN_H_
#define MARKOV_CHAIN_H_

#include "Random.h"
#include "Types.h"

#include <vector>
#include <map>

namespace Arcana
{
	template<typename DataType>
	class MarkovChain
	{
	public:

		class State
		{
			friend class MarkovChain;

		public: 

			DataType data;

		private:

			uint32 id;
		};

		MarkovChain();

		~MarkovChain();

		void addState(uint32 id, const State& state);

		void addState(uint32 id, const DataType& data);

		void addTransition(uint32 id0, uint32 id1, double probability);

		void setStartingState(uint32 id);

		uint32 getNumStates() const;

		DataType get();

		std::vector<DataType> get(uint32 num, bool includeCurrent = true);

	private:

		std::map<uint32, State> _states;
		std::map<uint32, std::map<uint32, double>> _probabilities;

		State* _currentState;
	};
}

#include "MarkovChain.inl"

#endif // !MARKOV_CHAIN_H_