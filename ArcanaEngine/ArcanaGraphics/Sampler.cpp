#include "Sampler.h"

#include "Vector4.h"
#include "Texture.h"


namespace Arcana
{

	Sampler::Parameters::Parameters() :
		_wrapS(TextureWrap::ClampToEdge), _wrapT(TextureWrap::ClampToEdge), _wrapR(TextureWrap::ClampToEdge),
		_min(TextureFilter::Nearest), _mag(TextureFilter::Linear), _borderType(0),
		_lodMin(-1000.0f), _lodMax(1000.0f), _lodBias(0.0f),
		_maxAnisotropy(1.0f), _compareFunc(RenderState::Always)
	{
		_border.i[0] = 0;
		_border.i[1] = 0;
		_border.i[2] = 0;
		_border.i[3] = 0;
	}

	Sampler::Parameters::~Parameters()
	{
	}

	TextureWrap Sampler::Parameters::getWrapS() const
	{
		return _wrapS;
	}

	TextureWrap Sampler::Parameters::getWrapT() const
	{
		return _wrapT;
	}

	TextureWrap Sampler::Parameters::getWrapR() const
	{
		return _wrapR;
	}

	TextureFilter Sampler::Parameters::getMinFilter() const
	{
		return _min;
	}

	TextureFilter Sampler::Parameters::getMagFilter() const
	{
		return _mag;
	}

	unsigned int Sampler::Parameters::getBorderType() const
	{
		return _borderType;
	}

	const GLint* Sampler::Parameters::getBorderi() const
	{
		return _border.i;
	}

	const GLfloat* Sampler::Parameters::getBorderf() const
	{
		return _border.f;
	}

	const GLint* Sampler::Parameters::getBorderIi() const
	{
		return _border.i;
	}

	const GLuint* Sampler::Parameters::getBorderIui() const
	{
		return _border.ui;
	}

	GLfloat Sampler::Parameters::getLodMin() const
	{
		return _lodMin;
	}

	GLfloat Sampler::Parameters::getLodMax() const
	{
		return _lodMax;
	}

	GLfloat Sampler::Parameters::getLodBias() const
	{
		return _lodBias;
	}

	RenderState::DepthFunction Sampler::Parameters::getCompareFunc() const
	{
		return _compareFunc;
	}

	GLfloat Sampler::Parameters::getMaxAnisotropyEXT() const
	{
		return _maxAnisotropy;
	}

	void Sampler::Parameters::setWrapS(TextureWrap wrapS)
	{
		_wrapS = wrapS;
	}

	void Sampler::Parameters::setWrapT(TextureWrap wrapT)
	{
		_wrapT = wrapT;
	}

	void Sampler::Parameters::setWrapR(TextureWrap wrapR)
	{
		_wrapR = wrapR;
	}

	void Sampler::Parameters::setMinFilter(TextureFilter min)
	{
		_min = min;
	}

	void Sampler::Parameters::setMagFilter(TextureFilter mag)
	{
		_mag = mag;
	}

	void Sampler::Parameters::setBorderi(GLint r, GLint g, GLint b, GLint a)
	{
		_border.i[0] = r;
		_border.i[1] = g;
		_border.i[2] = b;
		_border.i[3] = a;
		_borderType = 0;
	}

	void Sampler::Parameters::setBorderf(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
	{
		_border.f[0] = r;
		_border.f[1] = g;
		_border.f[2] = b;
		_border.f[3] = a;
		_borderType = 1;
	}

	void Sampler::Parameters::setBorderIi(GLint r, GLint g, GLint b, GLint a)
	{
		_border.i[0] = r;
		_border.i[1] = g;
		_border.i[2] = b;
		_border.i[3] = a;
		_borderType = 2;
	}

	void Sampler::Parameters::setBorderIui(GLuint r, GLuint g, GLuint b, GLuint a)
	{
		_border.ui[0] = r;
		_border.ui[1] = g;
		_border.ui[2] = b;
		_border.ui[3] = a;
		_borderType = 3;
	}

	void Sampler::Parameters::setLodMin(GLfloat lodMin)
	{
		_lodMin = lodMin;
	}

	void Sampler::Parameters::setLodMax(GLfloat lodMax)
	{
		_lodMax = lodMax;
	}

	void Sampler::Parameters::setLodBias(GLfloat lodBias)
	{
		_lodBias = lodBias;
	}

	void Sampler::Parameters::setMaxAnisotropyEXT(GLfloat maxAnisotropy)
	{
		_maxAnisotropy = maxAnisotropy;
	}

	void Sampler::Parameters::setCompareFunc(RenderState::DepthFunction compareFunc)
	{
		_compareFunc = compareFunc;
	}

	bool Sampler::Parameters::operator<(const Parameters &v) const
	{
		if (_wrapS < v._wrapS) 
		{
			return true;
		}
		if (_wrapS > v._wrapS) 
		{
			return false;
		}
		if (_wrapT < v._wrapT) 
		{
			return true;
		}
		if (_wrapT > v._wrapT) 
		{
			return false;
		}
		if (_wrapR < v._wrapR) 
		{
			return true;
		}
		if (_wrapR > v._wrapR) 
		{
			return false;
		}
		if (_min < v._min) 
		{
			return true;
		}
		if (_mag > v._mag) 
		{
			return false;
		}
		if (_borderType < v._borderType) 
		{
			return true;
		}
		if (_borderType > v._borderType) 
		{
			return false;
		}
		switch (_borderType) 
		{
		case 0: // i
		case 2: 
		{ // Ii
			Vector4<GLint> ub(_border.i[0], _border.i[1], _border.i[2], _border.i[3]);
			Vector4<GLint> vb(v._border.i[0], v._border.i[1], v._border.i[2], v._border.i[3]);
			if (ub.magnitudeSq() < vb.magnitudeSq()) 
			{
				return true;
			}
			if (vb.magnitudeSq() < ub.magnitudeSq()) 
			{
				return false;
			}
			break;
		}
		case 3: 
		{ // Iui
			Vector4<GLuint> ub(_border.ui[0], _border.ui[1], _border.ui[2], _border.ui[3]);
			Vector4<GLuint> vb(v._border.ui[0], v._border.ui[1], v._border.ui[2], v._border.ui[3]);
			if (ub.magnitudeSq() < vb.magnitudeSq()) 
			{
				return true;
			}
			if (vb.magnitudeSq() < ub.magnitudeSq())
			{
				return false;
			}
			break;
		}
		case 1: 
		{ // f
			Vector4<GLfloat> ub(_border.f[0], _border.f[1], _border.f[2], _border.f[3]);
			Vector4<GLfloat> vb(v._border.f[0], v._border.f[1], v._border.f[2], v._border.f[3]);
			if (ub.magnitudeSq() < vb.magnitudeSq()) 
			{
				return true;
			}
			if (vb.magnitudeSq() < ub.magnitudeSq()) 
			{
				return false;
			}
			break;
		}
		default:
			AE_ASSERT(false);
		}
		if (_lodMin < v._lodMin) 
		{
			return true;
		}
		if (_lodMin > v._lodMin)
		{
			return false;
		}
		if (_lodMax < v._lodMax) 
		{
			return true;
		}
		if (_lodMax > v._lodMax) 
		{
			return false;
		}
		if (_lodBias < v._lodBias) 
		{
			return true;
		}
		if (_lodBias > v._lodBias) 
		{
			return false;
		}
		if (_maxAnisotropy < v._maxAnisotropy) 
		{
			return true;
		}
		if (_maxAnisotropy > v._maxAnisotropy) 
		{
			return false;
		}
		return _compareFunc < v._compareFunc;
	}

	void Sampler::Parameters::set(GLuint samplerId) const
	{
		glSamplerParameteri(samplerId, GL_TEXTURE_WRAP_S, getWrapS());
		glSamplerParameteri(samplerId, GL_TEXTURE_WRAP_T, getWrapT());
		glSamplerParameteri(samplerId, GL_TEXTURE_WRAP_R, getWrapR());
		glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, getMinFilter());
		glSamplerParameteri(samplerId, GL_TEXTURE_MAG_FILTER, getMagFilter());
		switch (getBorderType()) 
		{
		case 0: // i
			glSamplerParameteriv(samplerId, GL_TEXTURE_BORDER_COLOR, getBorderi());
			break;
		case 1: // f
			glSamplerParameterfv(samplerId, GL_TEXTURE_BORDER_COLOR, getBorderf());
			break;
		case 2: // Ii
			glSamplerParameterIiv(samplerId, GL_TEXTURE_BORDER_COLOR, getBorderIi());
			break;
		case 3: // Iui
			glSamplerParameterIuiv(samplerId, GL_TEXTURE_BORDER_COLOR, getBorderIui());
			break;
		default:
			AE_ASSERT(false);
		}
		glSamplerParameterf(samplerId, GL_TEXTURE_MIN_LOD, getLodMin());
		glSamplerParameterf(samplerId, GL_TEXTURE_MAX_LOD, getLodMax());
		glSamplerParameterf(samplerId, GL_TEXTURE_LOD_BIAS, getLodBias());
		if (getCompareFunc() != RenderState::Always) 
		{
			glSamplerParameteri(samplerId, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
			glSamplerParameteri(samplerId, GL_TEXTURE_COMPARE_FUNC, getCompareFunc());
		}
		glSamplerParameterf(samplerId, GL_TEXTURE_MAX_ANISOTROPY_EXT, getMaxAnisotropyEXT());
	}

	Sampler::Sampler(const Parameters &params) : _params(params)
	{
		std::map<Parameters, std::pair<GLuint, uint32> >::iterator i = __instances.find(params);
		if (i == __instances.end())
		{
			glGenSamplers(1, &_samplerId);
			AE_ASSERT(_samplerId > 0);
			params.set(_samplerId);
			__instances[params] = std::make_pair(_samplerId, 1);
		}
		else 
		{
			_samplerId = i->second.first;
			__instances[params] = std::make_pair(_samplerId, i->second.second + 1);
		}
	}

	Sampler::~Sampler()
	{
		//Texture::unbindSampler(this);
		std::map<Parameters, std::pair<GLuint, uint32> >::iterator i = __instances.find(_params);
		AE_ASSERT(i != __instances.end());
		AE_ASSERT(i->second.first == _samplerId);
		AE_ASSERT(i->second.second >= 1);
		if (i->second.second == 1) 
		{
			glDeleteSamplers(1, &_samplerId);
			__instances.erase(i);
		}
		else 
		{
			__instances[_params] = std::make_pair(_samplerId, i->second.second - 1);
		}
	}

	GLuint Sampler::getId() const
	{
		return _samplerId;
	}

	const Sampler::Parameters& Sampler::getParameters()
	{
		_previousParams = _params;

		return _params;
	}

	std::map<Sampler::Parameters, std::pair<GLuint, uint32> > Sampler::__instances;

}
