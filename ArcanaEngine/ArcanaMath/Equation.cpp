#include "Equation.h"

#include <iostream>

namespace Arcana
{
	//Operation

	Operation::Operation() : _type(Unknown)
	{

	}
	Operation::Operation(Type type) : _type(type)
	{

	}
	Operation::~Operation()
	{

	}

	double Operation::evaluate(double a, double b)
	{
		switch (_type)
		{
		case Addition:
			return a + b;
		case Subtraction:
			return a - b;
		case Multiplication:
			return a * b;
		case Division:
			return a / b;
		case Exponentiation:
			return pow(a, b);
		};

		return a;
	}

	Operation::Type Operation::getType() const
	{
		return _type;
	}


	//Expression

	Expression::Expression() : 
		_operation(Operation::Unknown), _left(nullptr), _right(nullptr), _isValue(false), _value(0.0)
	{

	}
	Expression::Expression(Operation op, Expression* left, Expression* right) : 
		_operation(op), _left(left), _right(right), _isValue(false), _value(0.0)
	{

	}
	Expression::Expression(double value) :
		_operation(Operation::Unknown), _left(nullptr), _right(nullptr), _isValue(true), _value(value)
	{

	}
	Expression::~Expression()
	{

	}

	double Expression::evaluate()
	{
		if (_isValue)
		{
			return _value;
		}

		if (!_left || !_right)
		{
			return 0.0;
		}

		return _operation.evaluate(_left->evaluate(), _right->evaluate());
	}


	//Equation

	Equation::Equation()
	{

	}
	Equation::Equation(std::vector<std::string> inputVariables, std::string equation) : _inputVariables(inputVariables)
	{
		parseEquation(equation);
	}
	Equation::~Equation()
	{

	}

	double Equation::evaluate(double vars, ...)
	{
		return 0;
	}

	Operation Equation::parseOperation(const std::string& operation)
	{
		if (operation.compare("+") == 0)
		{
			return Operation::Addition;
		}
		else if (operation.compare("-") == 0)
		{
			return Operation::Subtraction;
		}
		if (operation.compare("*") == 0)
		{
			return Operation::Multiplication;
		}
		if (operation.compare("/") == 0)
		{
			return Operation::Division;
		}
		if (operation.compare("^") == 0)
		{
			return Operation::Exponentiation;
		}

		return Operation::Unknown;
	}

	void Equation::parseEquation(const std::string& string)
	{
		size_t eq = string.find("=");

		if (eq == std::string::npos)
		{
			return;
		}

		std::string left = string.substr(0, eq);
		std::string right = string.substr(eq + 1, string.size());

		trim(left);
		trim(right);

		if (!isCharacter(left) && !isCharacter(right))
		{
			return;
		}

		parseExpression(isCharacter(left) ? right : left);
	}

	void Equation::parseExpression(const std::string& string)
	{
		std::vector<std::string> vars;
		for (int i = 0; i < string.size(); i++)
		{
			if (isCharacter(string.at(i)))
			{
				vars.push_back(std::string(1, string.at(i)));
			}
		}

		if (vars.size() != _inputVariables.size())
		{
			return;
		}

		for (int i = 0; i < vars.size(); i++)
		{
			if (std::find(_inputVariables.begin(), _inputVariables.end(), vars[i]) == _inputVariables.end())
			{
				return;
			}
		}


	}
}