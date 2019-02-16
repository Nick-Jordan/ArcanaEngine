#include "OpenGLState.h"

namespace Arcana
{
	OpenGLState OpenGLState::CurrentState = OpenGLState();

	OpenGLState::OpenGLState()
		: _wireframe(false),
		_cullFaceEnabled(false),
		_depthTestEnabled(false),
		_depthWriteEnabled(false),
		_depthFunction(DepthFunction::Less),
		_blendEnabled(false),
		_blendSrc(Blend::One),
		_blendDst(Blend::Zero),
		_cullFaceSide(CullFaceSide::Back),
		_frontFace(FrontFace::CounterClockwise),
		_stencilTestEnabled(false),
		_stencilWrite(0),
		_stencilFunction(StencilFunction::StencilAlways),
		_stencilFunctionRef(0),
		_stencilFunctionMask(0),
		_stencilOpSfail(StencilOperation::StencilKeep),
		_stencilOpDpfail(StencilOperation::StencilKeep),
		_stencilOpDppass(StencilOperation::StencilKeep)
	{

	}
	OpenGLState::~OpenGLState()
	{

	}

	void OpenGLState::setWireframe(bool wireframe)
	{
		_wireframe = wireframe;
	}
	bool OpenGLState::isWireframe() const
	{
		return _wireframe;
	}

	void OpenGLState::setCullEnabled(bool enabled)
	{
		_cullFaceEnabled = enabled;
	}
	bool OpenGLState::isCullEnabled() const
	{
		return _cullFaceEnabled;
	}

	void OpenGLState::setDepthTestEnabled(bool enabled)
	{
		_depthTestEnabled = enabled;
	}
	bool OpenGLState::isDepthTestEnabled() const
	{
		return _depthTestEnabled;
	}

	void OpenGLState::setDepthWriteEnabled(bool enabled)
	{
		_depthWriteEnabled = enabled;
	}
	bool OpenGLState::isDepthWriteEnabled() const
	{
		return _depthWriteEnabled;
	}

	void OpenGLState::setDepthFunction(DepthFunction func)
	{
		_depthFunction = func;
	}
	OpenGLState::DepthFunction OpenGLState::getDepthFunction() const
	{
		return _depthFunction;
	}

	void OpenGLState::setBlendEnabled(bool enabled)
	{
		_blendEnabled = enabled;
	}
	bool OpenGLState::isBlendEnabled() const
	{
		return _blendEnabled;
	}

	void OpenGLState::setBlendSrc(Blend src)
	{
		_blendSrc = src;
	}
	OpenGLState::Blend OpenGLState::getBlendSrc() const
	{
		return _blendSrc;
	}

	void OpenGLState::setBlendDst(Blend dst)
	{
		_blendDst = dst;
	}
	OpenGLState::Blend OpenGLState::getBlendDst() const
	{
		return _blendDst;
	}

	void OpenGLState::setCullFaceSide(CullFaceSide side)
	{
		_cullFaceSide = side;
	}
	OpenGLState::CullFaceSide OpenGLState::getCullFaceSide() const
	{
		return _cullFaceSide;
	}

	void OpenGLState::setFrontFace(FrontFace face)
	{
		_frontFace = face;
	}
	OpenGLState::FrontFace OpenGLState::getFrontFace() const
	{
		return _frontFace;
	}

	void OpenGLState::setStencilTestEnabled(bool enabled)
	{
		_stencilTestEnabled = enabled;
	}
	bool OpenGLState::isStencilTestEnabled() const
	{
		return _stencilTestEnabled;
	}

	void OpenGLState::setStencilWrite(uint32 stencilWrite)
	{
		_stencilWrite = stencilWrite;
	}
	uint32 OpenGLState::getStencilWrite() const
	{
		return _stencilWrite;
	}

	void OpenGLState::setStencilFunction(StencilFunction func)
	{
		_stencilFunction = func;
	}
	OpenGLState::StencilFunction OpenGLState::getStencilFunction() const
	{
		return _stencilFunction;
	}

	void OpenGLState::setStencilFuncRef(int32 stencilFuncRef)
	{
		_stencilFunctionRef = stencilFuncRef;
	}
	int32 OpenGLState::getStencilFuncRef() const
	{
		return _stencilFunctionRef;
	}

	void OpenGLState::setStencilFuncMask(uint32 stencilFuncMask)
	{
		_stencilFunctionMask = stencilFuncMask;
	}
	uint32 OpenGLState::getStencilFuncMask() const
	{
		return _stencilFunctionMask;
	}

	void OpenGLState::setStencilOpSFail(StencilOperation stencilOp)
	{
		_stencilOpSfail = stencilOp;
	}
	OpenGLState::StencilOperation OpenGLState::getStencilOpSFail() const
	{
		return _stencilOpSfail;
	}

	void OpenGLState::setStencilOpDpFail(StencilOperation stencilOp)
	{
		_stencilOpDpfail = stencilOp;
	}
	OpenGLState::StencilOperation OpenGLState::getStencilOpDpFail() const
	{
		return _stencilOpDpfail;
	}

	void OpenGLState::setStencilOpDpPass(StencilOperation stencilOp)
	{
		_stencilOpDppass = stencilOp;
	}
	OpenGLState::StencilOperation OpenGLState::getStencilOpDpPass() const
	{
		return _stencilOpDppass;
	}


	void OpenGLState::bind()
	{
		bool copy = false;

		if (_wireframe)
		{
			//glPolygonMode?
		}

		//Vertex Winding
		if (CurrentState.getFrontFace() != _frontFace)
		{
			glFrontFace(_frontFace);

			copy = true;
		}

		//Cull Face
		if (!checkCulling(CurrentState))
		{
			if (_cullFaceEnabled)
			{
				glEnable(GL_CULL_FACE);
				glCullFace(_cullFaceSide);
			}
			else
			{
				glDisable(GL_CULL_FACE);
			}

			copy = true;
		}

		//Blend
		if (!checkBlend(CurrentState))
		{
			if (_blendEnabled)
			{
				glEnable(GL_BLEND);
				glBlendFunc(_blendSrc, _blendDst);
			}
			else
			{
				glDisable(GL_BLEND);
			}

			copy = true;
		}

		//Depth
		if (!checkDepth(CurrentState))
		{
			//Depth Test
			if (_depthTestEnabled)
			{
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(_depthFunction);
			}
			else
			{
				glDisable(GL_DEPTH_TEST);
			}

			//Depth Write
			if (_depthWriteEnabled)
			{
				glDepthMask(_depthWriteEnabled ? GL_TRUE : GL_FALSE);
			}

			copy = true;
		}

		//Stencil Test
		if (!checkStencil(CurrentState))
		{
			if (_stencilTestEnabled)
			{
				glEnable(GL_STENCIL_TEST);
				glStencilMask(_stencilWrite);
				glStencilFunc(_stencilFunction, _stencilFunctionRef, _stencilFunctionMask);
				glStencilOp(_stencilOpSfail, _stencilOpDpfail, _stencilOpDppass);
			}
			else
			{
				glDisable(GL_STENCIL_TEST);
			}

			copy = true;
		}

		if(copy)
			CurrentState.set(*this);
	}

	void OpenGLState::unbind()
	{

	}

	void OpenGLState::set(const OpenGLState& state)
	{
		setWireframe(state.isWireframe());

		setDepthTestEnabled(state.isDepthTestEnabled());
		setDepthWriteEnabled(state.isDepthWriteEnabled());
		setDepthFunction(state.getDepthFunction());

		setBlendEnabled(state.isBlendEnabled());
		setBlendSrc(state.getBlendSrc());
		setBlendDst(state.getBlendDst());

		setCullEnabled(state.isCullEnabled());
		setCullFaceSide(state.getCullFaceSide());

		setFrontFace(state.getFrontFace());

		setStencilTestEnabled(state.isStencilTestEnabled());
		setStencilWrite(state.getStencilWrite());
		setStencilFunction(state.getStencilFunction());
		setStencilFuncRef(state.getStencilFuncRef());
		setStencilFuncMask(state.getStencilFuncMask());
		setStencilOpSFail(state.getStencilOpSFail());
		setStencilOpDpFail(state.getStencilOpDpFail());
		setStencilOpDpPass(state.getStencilOpDpPass());
	}

	bool OpenGLState::checkDepth(const OpenGLState& state)
	{
		return state.isDepthTestEnabled() == isDepthTestEnabled()
			&& state.isDepthWriteEnabled() == isDepthWriteEnabled()
			&& state.getDepthFunction() == getDepthFunction();
	}

	bool OpenGLState::checkBlend(const OpenGLState& state)
	{
		return state.isBlendEnabled() == isBlendEnabled()
			&& state.getBlendSrc() == getBlendSrc()
			&& state.getBlendDst() == getBlendDst();
	}

	bool OpenGLState::checkCulling(const OpenGLState& state)
	{
		return state.isCullEnabled() == isCullEnabled()
			&& state.getCullFaceSide() == getCullFaceSide();
	}

	bool OpenGLState::checkStencil(const OpenGLState& state)
	{
		return state.isStencilTestEnabled() == isStencilTestEnabled()
			&& state.getStencilWrite() == getStencilWrite()
			&& state.getStencilFunction() == getStencilFunction()
			&& state.getStencilFuncRef() == getStencilFuncRef()
			&& state.getStencilFuncMask() == getStencilFuncMask()
			&& state.getStencilOpSFail() == getStencilOpSFail()
			&& state.getStencilOpDpFail() == getStencilOpDpFail()
			&& state.getStencilOpDpPass() == getStencilOpDpPass();
	}
}