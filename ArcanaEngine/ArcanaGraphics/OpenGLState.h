#ifndef OPENGL_STATE_H_
#define OPENGL_STATE_H_

#include "GraphicsDefines.h"

#include "../Dependencies/include/opengl/include.h"
#include "Types.h"

namespace Arcana
{

	class ARCANA_GRAPHICS_API OpenGLState
	{
	public:

		enum Blend
		{
			Zero = GL_ZERO,
			One = GL_ONE,
			SrcColor = GL_SRC_COLOR,
			OneMinusSrcColor = GL_ONE_MINUS_SRC_COLOR,
			DstColor = GL_DST_COLOR,
			OneMinusDstColor = GL_ONE_MINUS_DST_COLOR,
			SrcAlpha = GL_SRC_ALPHA,
			OneMinusSrcALpha = GL_ONE_MINUS_SRC_ALPHA,
			DstAlpha = GL_DST_ALPHA,
			OneMinusDstAlpha = GL_ONE_MINUS_DST_ALPHA,
			ConstantAlpha = GL_CONSTANT_ALPHA,
			OneMinusConstantAlpha = GL_ONE_MINUS_CONSTANT_ALPHA,
			SrcAlphaSaturate = GL_SRC_ALPHA_SATURATE,

			NUM_BLEND_FUNCTIONS
		};

		enum DepthFunction
		{
			Never = GL_NEVER,
			Less = GL_LESS,
			Equal = GL_EQUAL,
			LEqual = GL_LEQUAL,
			Greater = GL_GREATER,
			NotEqual = GL_NOTEQUAL,
			GEqual = GL_GEQUAL,
			Always = GL_ALWAYS,

			NUM_DEPTH_FUNCTIONS
		};

		enum CullFaceSide
		{
			Back = GL_BACK,
			Front = GL_FRONT,
			FrontAndBack = GL_FRONT_AND_BACK,

			NUM_CULL_FACE_SIDES
		};

		enum FrontFace
		{
			Clockwise = GL_CW,
			CounterClockwise = GL_CCW,

			NUM_FRONT_FACES
		};

		enum StencilFunction
		{
			StencilNever = GL_NEVER,
			StencilAlways = GL_ALWAYS,
			StencilLess = GL_LESS,
			StencilLEqual = GL_LEQUAL,
			StencilEqual = GL_EQUAL,
			StencilGreater = GL_GREATER,
			StencilGEqual = GL_GEQUAL,
			StencilNotEqual = GL_NOTEQUAL,

			NUM_STENCIL_FUNCTIONS
		};

		enum StencilOperation
		{
			StencilKeep = GL_KEEP,
			StencilZero = GL_ZERO,
			StencilReplace = GL_REPLACE,
			StencilIncr = GL_INCR,
			StencilDecr = GL_DECR,
			StencilInvert = GL_INVERT,
			StencilIncrWrap = GL_INCR_WRAP,
			StencilDecrWrap = GL_DECR_WRAP,
		};

		OpenGLState();
		~OpenGLState();

		void setWireframe(bool wireframe);
		bool isWireframe() const;

		void setCullEnabled(bool enabled);
		bool isCullEnabled() const;

		void setDepthTestEnabled(bool enabled);
		bool isDepthTestEnabled() const;

		void setDepthWriteEnabled(bool enabled);
		bool isDepthWriteEnabled() const;

		void setDepthFunction(DepthFunction func);
		DepthFunction getDepthFunction() const;

		void setBlendEnabled(bool enabled);
		bool isBlendEnabled() const;

		void setBlendSrc(Blend src);
		Blend getBlendSrc() const;

		void setBlendDst(Blend dst);
		Blend getBlendDst() const;

		void setCullFaceSide(CullFaceSide side);
		CullFaceSide getCullFaceSide() const;

		void setFrontFace(FrontFace face);
		FrontFace getFrontFace() const;

		void setStencilTestEnabled(bool enabled);
		bool isStencilTestEnabled() const;

		void setStencilWrite(uint32 stencilWrite);
		uint32 getStencilWrite() const;

		void setStencilFunction(StencilFunction func);
		StencilFunction getStencilFunction() const;

		void setStencilFuncRef(int32 stencilFuncRef);
		int32 getStencilFuncRef() const;

		void setStencilFuncMask(uint32 stencilFuncMask);
		uint32 getStencilFuncMask() const;

		void setStencilOpSFail(StencilOperation stencilOp);
		StencilOperation getStencilOpSFail() const;

		void setStencilOpDpFail(StencilOperation stencilOp);
		StencilOperation getStencilOpDpFail() const;

		void setStencilOpDpPass(StencilOperation stencilOp);
		StencilOperation getStencilOpDpPass() const;


		void bind();

		void unbind();

	private:

		bool _wireframe;
		bool _cullFaceEnabled;
		bool _depthTestEnabled;
		bool _depthWriteEnabled;
		DepthFunction _depthFunction;
		bool _blendEnabled;
		Blend _blendSrc;
		Blend _blendDst;
		CullFaceSide _cullFaceSide;
		FrontFace _frontFace;
		bool _stencilTestEnabled;
		uint32 _stencilWrite;
		StencilFunction _stencilFunction;
		int32 _stencilFunctionRef;
		uint32 _stencilFunctionMask;
		StencilOperation _stencilOpSfail;
		StencilOperation _stencilOpDpfail;
		StencilOperation _stencilOpDppass;
	};

}

#endif // !OPENGL_STATE_H_

