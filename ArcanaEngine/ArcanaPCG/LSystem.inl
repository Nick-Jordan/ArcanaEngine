
namespace Arcana
{
	template<typename Element, class Sequence>
	LSystem<Element, Sequence>::LSystem()
	{

	}

	template<typename Element, class Sequence>
	LSystem<Element, Sequence>::~LSystem()
	{

	}  
		
	template<typename Element, class Sequence>  
	void LSystem<Element, Sequence>::setState(Sequence state)
	{
		_state = state;
	}

	template<typename Element, class Sequence>  
	Sequence& LSystem<Element, Sequence>::getState()
	{
		return _state;
	}
		  
	template<typename Element, class Sequence>  
	void LSystem<Element, Sequence>::addRule(Element conditional, Sequence predicate)
	{
		_ruleSet.push_back(Rule(conditional, predicate));
	}  
		  
	template<typename Element, class Sequence>  
	Sequence LSystem<Element, Sequence>::replace(Element& conditional)
	{
		typename std::vector<Rule>::iterator i;
		for(i = _ruleSet.begin(); i != _ruleSet.end(); i++)
		{
			Rule rule = *i;

			if(rule.isMatch(conditional))
			{
				return rule.getPredicate();
			}
		}

		Sequence sequence;
		sequence.push_back(conditional);
		return sequence;
	} 
		  
	template<typename Element, class Sequence>  
	void LSystem<Element, Sequence>::step()
	{
		Sequence newState;

		for(int i = 0; i < _state.size(); i++)
		{
			Sequence replacement = replace(_state.at(i));
			for(int j = 0; j < replacement.size(); j++)
			{
				newState.push_back(replacement[j]);
			}
		}

		_state = newState;
	}  
}