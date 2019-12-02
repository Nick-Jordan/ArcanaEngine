#include "AtmosphereRenderProcedure.h"

#include "Profiler.h"
#include "MeshIndexComponent.h"

namespace Arcana
{
	AtmosphereRenderProcedure::AtmosphereRenderProcedure()
		: _mesh(nullptr), _material(nullptr)
	{
		_material = new Material("atmosphere");
		Shader shader;
		shader.createProgram(Shader::Vertex, "resources/terrain/atmosphere/atmosphere_vert.glsl");
		shader.createProgram(Shader::Fragment, "resources/terrain/atmosphere/atmosphere_frag.glsl");
		Technique* technique = new Technique(shader);
		_material->addTechnique(technique);

		Properties.LightProperties.CastsDynamicShadow = false;
		Properties.RendererStage = "TransparentObjectStage";//environment
		Properties.RenderState.setCullEnabled(true);
		Properties.RenderState.setDepthTestEnabled(true);
		Properties.RenderState.setBlendEnabled(true);
		Properties.RenderState.setBlendSrc(RenderState::Blend::SrcAlpha);
		Properties.RenderState.setBlendDst(RenderState::Blend::OneMinusSrcAlpha);

		VertexFormat::Attribute attribs[] =
		{
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 3),
		};
		VertexFormat format(1, attribs);
		_mesh = new Mesh(format, Mesh::TriangleStrip);
		float vertices[] = {
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 0.0f
		};
		_mesh->setVertexBuffer(format, 4)->setVertexData(vertices);
	}

	AtmosphereRenderProcedure::~AtmosphereRenderProcedure()
	{
		if (_mesh)
		{
			AE_DELETE(_mesh);
		}

		if (_material)
		{
			AE_RELEASE(_material);
		}
	}

	void AtmosphereRenderProcedure::render()
	{
		_mesh->getVertexBuffer()->bind();

		Technique* technique = _material->getCurrentTechnique();
		if (technique)
		{
			for (uint32 i = 0; i < technique->getPassCount(); i++)
			{
				Shader* pass = technique->getPass(i);
				if (pass)
				{
					pass->bind();

					pass->getUniform("u_ProjectionMatrix").setValue(Projection.inverse().cast<float>());
					pass->getUniform("u_ViewMatrix").setValue(View.cast<float>());
					pass->getUniform("u_ModelMatrix").setValue(Transform.getMatrix().cast<float>());
					pass->getUniform("u_CameraPosition").setValue(EyePosition.cast<float>());

					int32 unit = Terrain::_inscatter->bind(_material);
					pass->getUniform("inscatterSampler").setValue(unit);
					unit = Terrain::_irradiance->bind(_material);
					pass->getUniform("skyIrradianceSampler").setValue(unit);
					unit = Terrain::_transmittance->bind(_material);
					pass->getUniform("transmittanceSampler").setValue(unit);
					unit = Terrain::_sunglare->bind(_material);
					pass->getUniform("glareSampler").setValue(unit);

					//pass->getUniform("u_StarColor").setValue(Vector3f(66.0, 134.0, 244.0) / 255.0);
					//pass->getUniform("u_StarColor").setValue(Vector3f(237.0, 49.0, 49.0) / 255.0);
					pass->getUniform("u_StarColor").setValue(Vector3f(255.0, 152.0, 17.0) / 255.0);
					//pass->getUniform("u_StarColor").setValue(Vector3f::one());

					pass->getUniform("cameraToWorld").setValue(View.inverse().cast<float>());

					pass->getUniform("u_WorldSunDir").setValue(Vector3f(0, 0, 1));

					pass->getUniform("origin").setValue(Vector3f::zero());

					for (uint32 j = 0; j < Uniforms.size(); j++)
					{
						pass->getUniform(Uniforms[i].name).setValue(Uniforms[i].value);
					}

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					glDrawArrays(_mesh->getPrimitive(), 0, _mesh->getNumVertices());

					pass->unbind();
				}
			}
		}

		_mesh->getVertexBuffer()->unbind();
	}

	bool AtmosphereRenderProcedure::isValidProcedure()
	{
		return _mesh != nullptr && _material != nullptr;
	}
}