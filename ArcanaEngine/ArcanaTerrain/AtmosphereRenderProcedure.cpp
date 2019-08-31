/*#include "AtmosphereRenderProcedure.h"

#include "Profiler.h"
#include "MeshIndexComponent.h"

namespace Arcana
{
	AtmosphereRenderProcedure::AtmosphereRenderProcedure(const Transform& transform)
		: _data(nullptr), _mesh(nullptr), _material(nullptr), _transform(transform)
	{

	}

	AtmosphereRenderProcedure::~AtmosphereRenderProcedure()
	{
		if (_data)
		{
			AE_DELETE(_data);
		}

		if (_mesh)
		{
			AE_DELETE(_mesh);
		}

		if (_material)
		{
			AE_RELEASE(_material);
		}
	}

	bool AtmosphereRenderProcedure::isDirty() const
	{
		return true;
	}

	void AtmosphereRenderProcedure::markDirty(bool dirty)
	{
		//todo
	}

	void AtmosphereRenderProcedure::createRenderData()
	{
		if (_data)
		{
			AE_DELETE(_data);
		}
		if (_mesh)
		{
			AE_DELETE(_mesh);
		}
		if (_material)
		{
			AE_RELEASE(_material);
		}

		_data = new AtmosphereRenderData();

		_material = new Material("atmosphere");
		Shader shader;
		shader.createProgram(Shader::Vertex, "resources/terrain/atmosphere/atmosphere_vert.glsl");
		shader.createProgram(Shader::Fragment, "resources/terrain/atmosphere/atmosphere_frag.glsl");
		Technique* technique = new Technique(shader);
		_material->addTechnique(technique);

		_renderState.setCullEnabled(true);
		_renderState.setDepthTestEnabled(true);
		_renderState.setBlendEnabled(true);
		_renderState.setBlendSrc(RenderState::Blend::SrcAlpha);
		_renderState.setBlendDst(RenderState::Blend::OneMinusSrcAlpha);

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

		_dummyProjection = Matrix4d::createPerspective(40.0, 1920.0/1080.0, 1.0, 10000);
		_dummyProjection = _dummyProjection.inverse();
	}

	void AtmosphereRenderProcedure::updateRenderData(const RenderDataUpdate& data)
	{

		if (isValidProcedure())
		{
			//_data->_context.material = _material;
			//_data->_context.mesh = _mesh;
			_data->_context.eyePosition = data.eyePosition;
			_data->_context.viewMatrix = data.view;
			_data->_context.projectionMatrix = _dummyProjection;
			_data->_context.renderProperties.renderState = _renderState;

			_data->_context.renderProperties.rendererStage = "TransparentObjectStage";

			if (!_data->_context.callback.isBound())
			{
				_data->_context.callback.bind(this, &AtmosphereRenderProcedure::renderAtmosphere); //keep ?
			}

			_data->_context.transform = _transform;
		}
	}

	RenderData* AtmosphereRenderProcedure::getRenderData() const
	{
		return _data;
	}

	bool AtmosphereRenderProcedure::isValidProcedure()
	{
		return _data != nullptr;
	}

	AtmosphereRenderData::AtmosphereRenderData()
	{
	}

	AtmosphereRenderData::~AtmosphereRenderData()
	{
	}

	void AtmosphereRenderData::render(ObjectRenderer& renderer)
	{
		renderer.addMesh(_context);
	}

	void AtmosphereRenderProcedure::renderAtmosphere()
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

					pass->getUniform("u_ProjectionMatrix").setValue(_data->_context.projectionMatrix.cast<float>());
					pass->getUniform("u_ViewMatrix").setValue(_data->_context.viewMatrix.cast<float>());
					pass->getUniform("u_ModelMatrix").setValue(_data->_context.transform.getMatrix().cast<float>());
					pass->getUniform("u_CameraPosition").setValue(_data->_context.eyePosition.cast<float>());

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

					pass->getUniform("cameraToWorld").setValue(_data->_context.viewMatrix.inverse().cast<float>());

					pass->getUniform("u_WorldSunDir").setValue(Vector3f(0, 0, 1));

					pass->getUniform("origin").setValue(Vector3f::zero());

					for (uint32 j = 0; j < _data->_context.uniforms.size(); j++)
					{
						pass->getUniform(_data->_context.uniforms[i].name).setValue(_data->_context.uniforms[i].value);
					}

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					glDrawArrays(_mesh->getPrimitive(), 0, _mesh->getNumVertices());

					pass->unbind();
				}
			}
		}

		_mesh->getVertexBuffer()->unbind();
	}
}*/