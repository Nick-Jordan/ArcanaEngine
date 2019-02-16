#ifndef L_SYSTEM_H_
#define L_SYSTEM_H_

#include <vector>

namespace Arcana
{
	template<typename Element, class Sequence> 
	class LSystem  
	{ 
	public:

		class Rule  
		{
		public:

			Rule(Element conditional, Sequence predicate) : _conditional(conditional), _predicate(predicate) {}  
			  
			bool isMatch(Element conditional) { return conditional == _conditional; }  
			Sequence& getPredicate() { return _predicate; }  

		private:

			Element _conditional;  
			Sequence _predicate; 
		};  
		
		LSystem();

		~LSystem();  
		  
		void setState(Sequence state);

		Sequence& getState(); 
		  
		void addRule(Element conditional, Sequence predicate);  
		  
		Sequence replace(Element& conditional);  
		  
		void step();  

	private:

		Sequence _state;  
		std::vector<Rule> _ruleSet;  
		std::vector<Element> _conditionals;
		std::vector<Sequence> _predicates;
	};  
}

#include "LSystem.inl"

#endif // !L_SYSTEM_H_