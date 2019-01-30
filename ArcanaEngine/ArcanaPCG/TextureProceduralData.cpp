#include "TextureProceduralData.h"

#include "TextureAttachment.h"

namespace Arcana
{
	/*REMOVE THIS*/
	void drawQuad(Shader& shader)
	{
		float quadVertices[] = {

			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			1.0f, -1.0f,  1.0f, 0.0f,
			1.0f,  1.0f,  1.0f, 1.0f
		};

		unsigned int quadVAO, quadVBO;
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		shader.bind();

		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	TextureProceduralData::TextureProceduralData(const Shader& shader, Texture::Format format, uint32 width, uint32 height,
		Texture::InternalFormat iformat, Texture::PixelType pixelType, 
		const Texture::Parameters& parameters, bool generateMipmap) 
		: ProceduralData(), _shader(shader)
	{
		_texture = Texture::create2D(format, width, height, iformat, pixelType, nullptr, parameters, generateMipmap);

		_framebuffer = new Framebuffer("texture_procedural_data");
		_framebuffer->addAttachment(new TextureAttachment("texture", _texture));
	}


	TextureProceduralData::~TextureProceduralData()
	{
		AE_DELETE(_framebuffer);
		AE_RELEASE(_texture);
	}

	void TextureProceduralData::generate(const ProceduralParameters& params, const Seed& seed)
	{
		seed.setUniforms(_shader);
		params.setUniforms(_shader);

		GLenum buffers[1] = { Framebuffer::Color0 };
		_framebuffer->setDrawBuffers(buffers, 1);

		Framebuffer* prev = _framebuffer->bind();

		drawQuad(_shader);

		Framebuffer::bind(prev);
	}

	Texture* TextureProceduralData::getTexture()
	{
		return _texture;
	}
}
