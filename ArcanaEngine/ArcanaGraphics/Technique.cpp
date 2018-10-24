#include "Technique.h"

#include "Material.h"

namespace Arcana
{

	Technique::Technique() : _numPasses(0), _passes(nullptr)
	{

	}

	Technique::Technique(uint32 numPasses) : _numPasses(numPasses), _passes(nullptr)
	{
		_passes = new Shader[_numPasses];
	}

	Technique::Technique(const Shader& shader) : _numPasses(1), _passes(nullptr)
	{
		_passes = new Shader[1];
		_passes[0] = shader;
	}

	Technique::Technique(const Technique& technique) : _numPasses(technique._numPasses)
	{
		_passes = new Shader[_numPasses];

		memcpy(_passes, technique._passes, _numPasses * sizeof(Shader));
	}


	Technique::~Technique()
	{
		AE_DELETE_ARRAY(_passes);
	}

	uint32 Technique::getPassCount() const
	{
		return _numPasses;
	}

	Shader* Technique::getPass(uint32 index)
	{
		if (index < _numPasses)
		{
			return &_passes[index];
		}
		return nullptr;
	}

	void Technique::setPass(uint32 index, const Shader& shader)
	{
		if (index < _numPasses)
		{
			_passes[index] = shader;
		}
	}

	bool Technique::operator==(const Technique& t) const
	{
		if (t._numPasses != _numPasses)
			return false;

		for (uint32 i = 0; i < _numPasses; i++)
		{
			if (_passes[i].getId() != t._passes[i].getId())
			{
				return false;
			}
		}

		return true;
	}

	bool Technique::operator != (const Technique& t) const
	{
		return !(*this == t);
	}

	Technique& Technique::operator=(const Technique& technique)
	{
		_numPasses = technique._numPasses;

		_passes = new Shader[_numPasses];

		memcpy(_passes, technique._passes, _numPasses * sizeof(Shader));

		return *this;
	}
}
