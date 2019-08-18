#include "DecalRenderProcedure.h"

#include "ResourceManager.h"

namespace Arcana
{

	DecalRenderProcedure::DecalRenderProcedure(const DecalProperties& properties)
		: _data(nullptr), _mesh(nullptr), _material(nullptr), _properties(properties)
	{
	}

	DecalRenderProcedure::~DecalRenderProcedure()
	{
		if (_data)
		{
			AE_DELETE(_data);
		}

		AE_RELEASE(_mesh);
		AE_RELEASE(_material);
	}

	bool DecalRenderProcedure::isDirty() const
	{
		return true;
	}

	void DecalRenderProcedure::markDirty(bool dirty)
	{
		//nothing
	}

	void DecalRenderProcedure::createRenderData()
	{
		//create mesh/material

		VertexFormat::Attribute attribs[] =
		{
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 3),
			VertexFormat::Attribute(VertexFormat::Semantic::Normal, 3),
			VertexFormat::Attribute(VertexFormat::Semantic::TexCoord0, 2),
		};
		VertexFormat format(3, attribs);
		_mesh = new Mesh(format, Mesh::Triangles);
		_mesh->reference();

		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};

		_mesh->setVertexBuffer(format, 36)->setVertexData(vertices);

		Shader shader;
		shader.createProgram(Shader::Vertex, "resources/arcana/shaders/deferred_decal_vert.glsl");
		shader.createProgram(Shader::Fragment, "resources/arcana/shaders/deferred_decal_frag.glsl");

		_material = new Material("decal");
		_material->reference();
		Technique* technique = new Technique(shader);
		_material->addTechnique(technique);

		_renderProperties.rendererStage = "DeferredDecalStage";
		_renderProperties.lightProperties.CastsDynamicShadow = false;
		_renderProperties.renderState.setCullEnabled(true);//set renderstate
		_renderProperties.renderState.setCullFaceSide(RenderState::Back);
		_renderProperties.renderState.setDepthTestEnabled(true);
		_renderProperties.renderState.setBlendEnabled(false);

		if (_data)
		{
			AE_DELETE(_data);
		}

		_data = new MeshRenderData();

		_data->context.mesh = _mesh;
		_data->context.material = _material;
		_data->context.renderProperties = _renderProperties;
		_data->context.transform.setIdentity();
	}

	void DecalRenderProcedure::updateRenderData(const RenderDataUpdate& data)
	{
		if (isValidProcedure())
		{
			_data->context.projectionMatrix = data.projection;
			_data->context.viewMatrix = data.view;
			_data->context.eyePosition = data.eyePosition;
			_data->context.transform.set(data.transform);

			_data->context.uniforms.clear();

			int32 unit;
			if (_properties._normals)
			{
				unit = _properties._normals->bind();
				MeshRenderContext::UniformParameter param;
				param.name = "u_Decal.normals";
				param.value.type = Uniform::Value::Int32;
				param.value.i = unit;
				_data->context.uniforms.push_back(param);

				MeshRenderContext::UniformParameter paramBlend;
				paramBlend.name = "u_Decal.normalsBlendFactor";
				paramBlend.value.type = Uniform::Value::Float;
				paramBlend.value.f = _properties._normalsBlend;
				_data->context.uniforms.push_back(paramBlend);
			}
			if (_properties._albedo)
			{
				unit = _properties._albedo->bind();
				MeshRenderContext::UniformParameter param;
				param.name = "u_Decal.albedo";
				param.value.type = Uniform::Value::Int32;
				param.value.i = unit;
				_data->context.uniforms.push_back(param);

				MeshRenderContext::UniformParameter paramBlend;
				paramBlend.name = "u_Decal.albedoBlendFactor";
				paramBlend.value.type = Uniform::Value::Float;
				paramBlend.value.f = _properties._albedoBlend;
				_data->context.uniforms.push_back(paramBlend);
			}
			if (_properties._specular)
			{
				unit = _properties._specular->bind();
				MeshRenderContext::UniformParameter param;
				param.name = "u_Decal.specular";
				param.value.type = Uniform::Value::Int32;
				param.value.i = unit;
				_data->context.uniforms.push_back(param);

				MeshRenderContext::UniformParameter paramBlend;
				paramBlend.name = "u_Decal.specularBlendFactor";
				paramBlend.value.type = Uniform::Value::Float;
				paramBlend.value.f = _properties._specularBlend;
				_data->context.uniforms.push_back(paramBlend);
			}
			if (_properties._roughness)
			{
				unit = _properties._roughness->bind();
				MeshRenderContext::UniformParameter param;
				param.name = "u_Decal.roughness";
				param.value.type = Uniform::Value::Int32;
				param.value.i = unit;
				_data->context.uniforms.push_back(param);

				MeshRenderContext::UniformParameter paramBlend;
				paramBlend.name = "u_Decal.roughnessBlendFactor";
				paramBlend.value.type = Uniform::Value::Float;
				paramBlend.value.f = _properties._roughnessBlend;
				_data->context.uniforms.push_back(paramBlend);
			}
			if (_properties._emissive)
			{
				unit = _properties._emissive->bind();
				MeshRenderContext::UniformParameter param;
				param.name = "u_Decal.emissive";
				param.value.type = Uniform::Value::Int32;
				param.value.i = unit;
				_data->context.uniforms.push_back(param);

				MeshRenderContext::UniformParameter paramBlend;
				paramBlend.name = "u_Decal.emissive";
				paramBlend.value.type = Uniform::Value::Float;
				paramBlend.value.f = _properties._emissiveBlend;
				_data->context.uniforms.push_back(paramBlend);
			}
			if (_properties._metallic)
			{
				unit = _properties._metallic->bind();
				MeshRenderContext::UniformParameter param;
				param.name = "u_Decal.metallic";
				param.value.type = Uniform::Value::Int32;
				param.value.i = unit;
				_data->context.uniforms.push_back(param);

				MeshRenderContext::UniformParameter paramBlend;
				paramBlend.name = "u_Decal.metallicBlendFactor";
				paramBlend.value.type = Uniform::Value::Float;
				paramBlend.value.f = _properties._metallicBlend;
				_data->context.uniforms.push_back(paramBlend);
			}
			if (_properties._ao)
			{
				unit = _properties._ao->bind();
				MeshRenderContext::UniformParameter param;
				param.name = "u_Decal.ao";
				param.value.type = Uniform::Value::Int32;
				param.value.i = unit;
				_data->context.uniforms.push_back(param);

				MeshRenderContext::UniformParameter paramBlend;
				paramBlend.name = "u_Decal.aoBlendFactor";
				paramBlend.value.type = Uniform::Value::Float;
				paramBlend.value.f = _properties._aoBlend;
				_data->context.uniforms.push_back(paramBlend);
			}
		}
	}

	RenderData* DecalRenderProcedure::getRenderData() const
	{
		return _data;
	}

	bool DecalRenderProcedure::isValidProcedure()
	{
		return _data != nullptr;
	}

	void MeshRenderData::render(ObjectRenderer& renderer)
	{
		renderer.addMesh(context);
	}

	const MeshRenderContext& MeshRenderData::getContext() const
	{
		return context;
	}
}