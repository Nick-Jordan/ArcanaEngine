#ifndef SAMPLER_H_
#define SAMPLER_H_

#include "GraphicsDefines.h"

#include "TextureEnums.h"
#include "RenderState.h"
#include "Object.h"
#include "KeyValuePair.h"

#include <map>

namespace Arcana
{
	class Texture;

	class ARCANA_GRAPHICS_API Sampler : public Object
	{
	public:

		class ARCANA_GRAPHICS_API Parameters
		{
		public:

			Parameters();

			virtual ~Parameters();

			TextureWrap getWrapS() const;

			TextureWrap getWrapT() const;

			TextureWrap getWrapR() const;

			TextureFilter getMinFilter() const;

			TextureFilter getMagFilter() const;

			uint32 getBorderType() const;

			const GLint* getBorderi() const;

			const GLfloat* getBorderf() const;

			const GLint* getBorderIi() const;

			const GLuint* getBorderIui() const;

			GLfloat getLodMin() const;

			GLfloat getLodMax() const;

			GLfloat getLodBias() const;

			RenderState::DepthFunction getCompareFunc() const;

			GLfloat getMaxAnisotropyEXT() const;

			virtual void setWrapS(TextureWrap wrapS);

			virtual void setWrapT(TextureWrap wrapT);

			virtual void setWrapR(TextureWrap wrapR);

			virtual void setMinFilter(TextureFilter min);

			virtual void setMagFilter(TextureFilter mag);

			virtual void setBorderi(GLint r = 0, GLint g = 0, GLint b = 0, GLint a = 0);

			virtual void setBorderf(GLfloat r = 0.0f, GLfloat g = 0.0f, GLfloat b = 0.0f, GLfloat a = 0.0f);

			virtual void setBorderIi(GLint r = 0, GLint g = 0, GLint b = 0, GLint a = 0);

			virtual void setBorderIui(GLuint r = 0, GLuint g = 0, GLuint b = 0, GLuint a = 0);

			virtual void setLodMin(GLfloat lodMin);

			virtual void setLodMax(GLfloat lodMax);

			virtual void setLodBias(GLfloat lodBias);

			virtual void setCompareFunc(RenderState::DepthFunction compareFunc);

			virtual void setMaxAnisotropyEXT(GLfloat maxAnisotropy);

			virtual bool operator<(const Parameters &v) const;

			void set(GLuint samplerId) const;

		private:

			TextureWrap _wrapS;

			TextureWrap _wrapT;

			TextureWrap _wrapR;

			TextureFilter _min;

			TextureFilter _mag;

			union {
				GLint i[4];
				GLuint ui[4];
				GLfloat f[4];
			} _border;

			uint32 _borderType;

			GLfloat _lodMin;

			GLfloat _lodMax;

			GLfloat _lodBias;

			GLfloat _maxAnisotropy;

			RenderState::DepthFunction _compareFunc;
		};

		Sampler(const Parameters &value);

		~Sampler();

		GLuint getId() const;

		const Sampler::Parameters& getParameters();

	private:

		GLuint _samplerId;

		const Parameters _params;

		Parameters _previousParams;

		static std::map<Sampler::Parameters, KeyValuePair<GLuint, uint32> > __instances;
	};

}

#endif