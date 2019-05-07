#include "Renderer.h"

#include "ArcanaLog.h"
#include "ContextType.h"
#include "Application.h"
#include "Profiler.h"

//test
#include "NoDataEvents.h"
#include "ObjectRenderer.h"

namespace Arcana
{
	//threadLocalPtr
	Renderer* Renderer::ActiveRenderer = nullptr;
	
	
	Renderer::Renderer() : _worldRenderer(nullptr)
	{
		_context = RenderContext::create();

		setActive(true);
	}

	Renderer::Renderer(const RenderSettings& settings, Window* owner) : _context(nullptr), _worldRenderer(nullptr)
	{
		_context = RenderContext::create(settings, owner->getWindowContext());

		owner->_renderer = this;

		_timeline.setEventHandler(owner->getParent()->getEventHandler());
		_timeline.setTimelineLengthMode(Timeline::LastKeyFrame);

		setActive(true);
	}

	Renderer::Renderer(const RenderSettings& settings, uint32 width, uint32 height) : _context(nullptr), _worldRenderer(nullptr)
	{
		_context = RenderContext::create(settings, width, height);

		setActive(true);
	}
	
	Renderer::~Renderer()
	{
		setActive(false);

		delete _context;
	}
	
	bool Renderer::setActive(bool active)
	{
		if (_context)
		{
			bool a = _context->setActive(active);

			if (a)
			{
				ActiveRenderer = (active ? this : nullptr);
			}

			if (active)
			{
				_timeline.play();
			}
			else
			{
				_timeline.stop();
			}

			return a;
		}
		
		return false;
	}

	void Renderer::render()
	{
		if (setActive(true))
		{
			if (_context)
			{
				//#ifdef PROFILE_FRAMES
					PROFILE("Renderer Frame");
				//#endif

				double elapsedTime = _timer.reset().toSeconds();
				
				//LOGF(Error, CoreEngine, "Elapsed Time: %f", elapsedTime);
				//LOGF(Error, CoreEngine, "FPS: %f", 1.0/elapsedTime);
				//LOGF(Info, CoreEngine, "Timeline: %f", _timeline.getPlaybackPosition());

				clear(ClearColorDepthStencil, Vector4f::zero(), 1.0, 0);

				if (false)//_splashScreen.renderingSplash)////////////////////////////////SPLASH SCREEN////////////////////////////////
				{
					_splashScreen.splashTime += elapsedTime;

					_splashScreen.render();
				
					if (_splashScreen.splashTime >= _splashScreen.splashLength)
					{
						_splashScreen.renderingSplash = false;
						AE_DELETE(_splashScreen.texture);
					}
				}
				else
				{
					if (_worldRenderer)
					{
						_worldRenderer->renderActors();
					}
				}



				_context->render();
				_timeline.updateTimeline(elapsedTime);
			}
		}
	}

	void Renderer::setVerticalSync(bool enabled)
	{
		if (_context)
		{
			_context->setVerticalSync(enabled);
		}
	}
	
	const RenderSettings& Renderer::getSettings() const
	{
		if (_context)
		{
			return _context->getSettings();
		}

		return RenderSettings();
	}

	Timeline& Renderer::getTimeline()
	{
		return _timeline;
	}

	void Renderer::setWorldRenderer(WorldRenderer* renderer)
	{
		_worldRenderer = renderer;
	}

	WorldRenderer* Renderer::getWorldRenderer() const
	{
		return _worldRenderer;
	}
	
	uint64 Renderer::getActiveContextId()
	{
		return RenderContext::getActiveContextId();
	}

	void Renderer::clear(ClearFlags flags, const Vector4f& clearColor, float clearDepth, int32 clearStencil)
	{
		clear(flags, clearColor.x, clearColor.y, clearColor.z, clearColor.w, clearDepth, clearStencil);
	}

	void Renderer::clear(ClearFlags flags, float red, float green, float blue, float alpha, float clearDepth, int32 clearStencil)
	{
		if (_context)
		{
			_context->clear(flags, red, green, blue, alpha, clearDepth, clearStencil);
		}
	}


	SplashScreen::SplashScreen()
	{
		renderingSplash = true;
		splashLength = 5.0;
		splashTime = 0.0;
		texture = nullptr;
	}

	SplashScreen::~SplashScreen()
	{

	}

	void SplashScreen::render()
	{
		static Shader shader;
		static bool init = false;

		if (!init)
		{
			shader.createProgram(Shader::Vertex, "resources/arcana/shaders/scaled_quad_vert.glsl");
			shader.createProgram(Shader::Fragment, "resources/arcana/shaders/splashscreen_frag.glsl");

			Image<uint8> image;
			image.init("resources/arcana/textures/splash.png");

			Texture::Parameters params;
			params.setMagFilter(TextureFilter::Linear);
			params.setMinFilter(TextureFilter::Linear);

			texture = Texture::create2D(Texture::RGBA, image.getWidth(), image.getHeight(), Texture::RGBA8, Texture::UnsignedByte, image.getPixelsPtr(), params);

			init = true;
		}

		shader.bind();

		shader.getUniform("u_Scale").setValue(Vector2f(0.5f, -0.056730769f) * 1.5f);

		shader.getUniform("u_Texture").setValue(texture->bind());

		shader.getUniform("u_ScreenTime").setValue(Vector2f(splashTime, splashLength));

		ObjectRenderer::drawQuad();
	}
}