#include "PostProcessEffect.h"

#include "ObjectRenderer.h"
#include "StringUtils.h"
#include "PostProcessor.h"
#include "ArcanaTemplate.h"

namespace Arcana
{
	PostProcessEffect::PostProcessEffect(const std::string& frag)
		: _fragment(frag), _shader(nullptr), _enabled(false)
	{
	}

	PostProcessEffect::~PostProcessEffect()
	{
		AE_DELETE(_shader);
	}

	void PostProcessEffect::apply(Texture* texture)
	{
		if (!_enabled || !texture)
			return;

		_shader->bind();

		int32 unit = texture->bind();
		_shader->getUniform("u_Texture").setValue(unit);

		uint32 index = 0;
		for (auto i = _extraTextures.begin(); i != _extraTextures.end(); i++)
		{
			Texture* t = *i;
			unit = t->bind();
			_shader->getUniform("u_ExtraTextures[" + StringUtils::convertUint32ToString(index++) + "]").setValue(unit);
		}
		for (auto i = _clearedExtraTextures.begin(); i != _clearedExtraTextures.end(); i++)
		{
			Texture* t = *i;
			unit = t->bind();
			_shader->getUniform("u_ExtraTextures[" + StringUtils::convertUint32ToString(index++) + "]").setValue(unit);
		}

		ObjectRenderer::drawQuad();

		_shader->unbind();

		_clearedExtraTextures.clear();
	}

	void PostProcessEffect::initialize()
	{
		if (!_shader)
		{
			_shader = new Shader();
			_shader->createProgram(Shader::Vertex, "resources/arcana/shaders/quad_vert.glsl");
			_shader->createProgram(Shader::Fragment, _fragment);
		}
	}

	void PostProcessEffect::enable()
	{
		if (_enabled)
			return;

		_enabled = true;

		PostProcessor::EffectQueue.push_back(this);
	}

	void PostProcessEffect::disable()
	{
		if (!_enabled)
			return;

		_enabled = false;

		PostProcessor::EffectQueue.erase(std::remove(PostProcessor::EffectQueue.begin(), PostProcessor::EffectQueue.end(), this), PostProcessor::EffectQueue.end());
	}

	void PostProcessEffect::toggleEffect()
	{
		if (_enabled)
		{
			disable();
		}
		else
		{
			enable();
		}
	}

	bool PostProcessEffect::isEnabled() const
	{
		return _enabled;
	}
}