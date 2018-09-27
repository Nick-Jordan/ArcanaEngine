#ifndef EQUATION_H_
#define EQUATION_H_

#include "ArcanaMath.h"

#include <cmath>
#include <string>
#include <vector>

//MOVE TO ANOTHER CLASS
#include <algorithm> 
#include <cctype>
#include <locale>

namespace Arcana
{
	class ARCANA_MATH_API Operation
	{
	public:

		enum Type
		{
			Unknown = -1,

			Addition,
			Subtraction,
			Multiplication,
			Division,
			Exponentiation
		};

		Operation();
		Operation(Type type);
		~Operation();

		double evaluate(double a, double b);

		Type getType() const;

	private:

		Type _type;
	};

	class ARCANA_MATH_API Expression
	{
	public:

		Expression();
		Expression(Operation op, Expression* left, Expression* right);
		Expression(double value);
		Expression(std::string variable);
		~Expression();

		double evaluate();

	private:

		Operation _operation;
		Expression* _left;
		Expression* _right;

		bool _isValue;
		double _value;
	};

	class ARCANA_MATH_API Equation
	{
	public:

		Equation();
		Equation(std::vector<std::string> inputVariables, std::string equation);
		~Equation();

		double evaluate(double vars, ...);

		static Operation parseOperation(const std::string& operation);

	private:

		void parseEquation(const std::string& string);
		void parseExpression(const std::string& string);


		///MOVE TO ANOTHER CLASS
		inline void ltrim(std::string &s) {
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
				return !std::isspace(ch);
			}));
		}

		inline void rtrim(std::string &s) {
			s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
				return !std::isspace(ch);
			}).base(), s.end());
		}

		inline void trim(std::string &s) {
			ltrim(s);
			rtrim(s);
		}

		inline bool isCharacter(std::string s) {
			return s.size() == 1 && isCharacter(s.at(0));
		}
		inline bool isCharacter(char s) {
			return (s >= 65 && s <= 90) || (s >= 97 && s <= 122);
		}


		std::vector<std::string> _inputVariables;
		
	};
}

#endif // !EQUATION_H_