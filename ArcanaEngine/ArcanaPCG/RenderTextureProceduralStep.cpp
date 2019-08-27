#include "RenderTextureProceduralStep.h"

#include "TextureAttachment.h"
#include "DepthStencilAttachment.h"

namespace Arcana
{
	void RenderTextureProceduralStep::perform(
		const TextureProceduralParameters& params,
		ProceduralStep<Texture, TextureProceduralParameters>* previous,
		Texture** object)
	{
		Shader shader;
		shader.createProgram(Shader::Vertex, params.VertexShader.empty() ? "resources/arcana/shaders/quad_vert.glsl" : params.VertexShader);
		shader.createProgram(Shader::Fragment, params.FragmentShader);

		(*object) = Texture::create2D(params.Format, params.Width, params.Height, params.InternalFormat, params.PixelType, nullptr);
		Framebuffer* framebuffer = new Framebuffer("framebuffer");
		framebuffer->addAttachment(new TextureAttachment("texture", (*object)));

		GLenum buffers[1] = { Framebuffer::Color0 };
		framebuffer->setDrawBuffers(buffers, 1);

		Framebuffer* prev = framebuffer->bind();

		float quadVertices[] = 
		{
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
		params.setUniforms(shader);

		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		Framebuffer::bind(prev);

		AE_DELETE(framebuffer);
	}
}