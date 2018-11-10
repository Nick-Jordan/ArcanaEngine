#include "TextureManager.h"

namespace Arcana
{

	TextureManager& TextureManager::instance()
	{
		static TextureManager managerInstance;

		return managerInstance;
	}

	void TextureManager::bind(Texture* texture, const Sampler* sampler, uint32 unit)
	{
		if (unit < Texture::getMaxTextureUnits())
		{
			_units[unit]->bind(texture, sampler, _currentTime++);
		}
	}

	void TextureManager::unbind(const Texture* texture)
	{
		for (uint32 i = 0; i < Texture::getMaxTextureUnits(); i++) 
		{
			if (_units[i]->getBoundTexture() == texture) 
			{
				_units[i]->bind(nullptr, nullptr, _currentTime++);
			}
		}
	}

	void TextureManager::unbind(const Sampler *sampler)
	{
		for (uint32 i = 0; i < Texture::getMaxTextureUnits(); i++)
		{
			if (_units[i]->getBoundSampler() == sampler)
			{
				_units[i]->bind(nullptr, nullptr, _currentTime++);
			}
		}
	}

	void TextureManager::clearUnits()
	{
		for (uint32 i = 0; i < Texture::getMaxTextureUnits(); i++)
		{
			_units[i]->bind(nullptr, nullptr, _currentTime++);
		}
	}

	int32 TextureManager::getFreeTextureUnit(Material* material)
	{
		for (uint32 i = 0; i < Texture::getMaxTextureUnits(); i++) 
		{
			if (_units[i]->isFree()) 
			{
				return i;
			}
		}

		int32 unit = -1;
		uint32 oldest = _currentTime;

		for (GLuint i = 0; i < Texture::getMaxTextureUnits(); i++) 
		{
			const Texture *t = _units[i]->getBoundTexture();

			bool usesTexture = false;

			if (material)
			{
				usesTexture = material->usesTexture(t);
			}

			if (!usesTexture)
			{
				uint32 bindingTime = _units[i]->getBindingTime();
				if (unit == -1 || bindingTime < oldest)
				{
					unit = i;
					oldest = bindingTime;
				}
			}
		}

		return unit;
	}

	TextureManager::TextureManager()
	{
		for (uint32 i = 0; i < Texture::getMaxTextureUnits(); i++)
		{
			_units[i] = new Unit(i);
		}

		_currentTime = 0;
	}


	TextureManager::~TextureManager()
	{
		for (uint32 i = 0; i < Texture::getMaxTextureUnits(); i++)
		{
			AE_DELETE(_units[i]);
		}
	}


	TextureManager::Unit::Unit(GLuint unit) : _unit(unit), _boundTexture(nullptr), _boundSampler(nullptr), _time(0)
	{

	}

	const Texture* TextureManager::Unit::getBoundTexture() const
	{
		return _boundTexture;
	}

	const Sampler* TextureManager::Unit::getBoundSampler() const
	{
		return _boundSampler;
	}

	bool TextureManager::Unit::isFree() const
	{
		return _boundTexture == nullptr;
	}

	uint32 TextureManager::Unit::getBindingTime() const
	{
		return _time;
	}

	void TextureManager::Unit::bind(Texture* texture, const Sampler* sampler, uint32 time)
	{
		_time = time;

		const Sampler* currentSampler = _boundSampler;

		glActiveTexture(GL_TEXTURE0 + _unit);

		if (sampler != _boundSampler) 
		{
			GLuint samplerId = sampler ? sampler->getId() : 0;
			glBindSampler(_unit, samplerId);
			_boundSampler = sampler;
		}

		if (texture != _boundTexture) 
		{
			if (_boundTexture != nullptr)
			{
				_boundTexture->removeCurrentBinding(currentSampler);

				if (texture == nullptr || _boundTexture->getType() != texture->getType())
				{
					LOG(Warning, CoreEngine, "Binding Texture 0");
					glBindTexture(_boundTexture->getType(), 0);
				}
			}
			if (texture != nullptr)
			{
				LOG(Warning, CoreEngine, "Binding Texture");
				texture->addCurrentBinding(sampler, _unit);
				glBindTexture(texture->getType(), texture->getId());
			}
			_boundTexture = texture;
		}
	}
}