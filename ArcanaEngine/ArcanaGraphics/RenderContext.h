#ifndef RENDER_CONTEXT_H_
#define RENDER_CONTEXT_H_

//test
#define ARCANA_OPENGL

#ifdef ARCANA_OPENGL
#include "OpenGLContext.h"
#endif

namespace Arcana
{
	
	#ifdef ARCANA_OPENGL
	typedef OpenGLContext RenderContext;
	#endif
}

#endif // !RENDER_CONTEXT_H_