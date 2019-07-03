#include "PostProcessEffect.h"

#include "ObjectRenderer.h"
#include "StringUtils.h"
#include "PostProcessor.h"
#include "ArcanaTemplate.h"

namespace Arcana
{
	PostProcessEffect::PostProcessEffect(const std::string& name, const std::string& frag)
		: _name(name), _fragment(frag), _shader(nullptr), _enabled(false)
	{
		
	}

	PostProcessEffect::~PostProcessEffect()
	{
		AE_DELETE(_shader);
	}

	void PostProcessEffect::begin(Texture* texture)
	{
		if (!texture)
			return;

		if (!_enabled && !_defaultEffect)
			return;

		_shader->bind();

		int32 unit = texture->bind();
		_shader->getUniform("u_Texture").setValue(unit);
	}

	void PostProcessEffect::apply(Texture* texture)
	{
	
	}

	void PostProcessEffect::end(Texture* texture)
	{
		if (!texture)
			return;

		if (!_enabled && !_defaultEffect)
			return;

		uint32 index = 0;
		for (auto i = _clearedExtraTextures.begin(); i != _clearedExtraTextures.end(); i++)
		{
			Texture* t = *i;
			int32 unit = t->bind();
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
		_enabled = true;
	}

	void PostProcessEffect::disable()
	{
		_enabled = false;
	}

	void PostProcessEffect::toggleEffect()
	{
		_enabled = !_enabled;
	}

	bool PostProcessEffect::isEnabled() const
	{
		return _enabled;
	}

	const std::string& PostProcessEffect::getName() const
	{
		return _name;
	}
}