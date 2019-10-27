#include "DecalRenderProcedure.h"

#include "ResourceManager.h"

namespace Arcana
{

	DecalRenderProcedure::DecalRenderProcedure(const DecalProperties& properties)
		: _mesh(nullptr), _decalMaterial(nullptr), _zTestedDecalMaterial(nullptr), _properties(properties)
	{
		//create mesh/material

		VertexFormat::Attribute attribs[] =
		{
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 3),
		};
		VertexFormat format(1, attribs);
		_mesh = new Mesh(format, Mesh::Triangles);
		_mesh->reference();

		VertexFormat::Attribute instanceAttribs[] =
		{
			VertexFormat::Attribute(VertexFormat::Semantic::Color, 4),
			VertexFormat::Attribute(VertexFormat::Semantic::TexCoord0, 4),
			VertexFormat::Attribute(VertexFormat::Semantic::Binormal, 4),
			VertexFormat::Attribute(VertexFormat::Semantic::Binormal, 4),
			VertexFormat::Attribute(VertexFormat::Semantic::Binormal, 4),
			VertexFormat::Attribute(VertexFormat::Semantic::Binormal, 4)
		};
		_instanceFormat = VertexFormat(6, instanceAttribs, 1);
		_mesh->setInstanceBuffer(_instanceFormat, 0, true);

		float vertices[] = {
			// back face
			-0.5f, -0.5f, -0.5f, // bottom-left
			 0.5f,  0.5f, -0.5f, // top-right
			 0.5f, -0.5f, -0.5f, // bottom-right         
			 0.5f,  0.5f, -0.5f, // top-right
			-0.5f, -0.5f, -0.5f, // bottom-left
			-0.5f,  0.5f, -0.5f, // top-left
			// front face
			-0.5f, -0.5f,  0.5f, // bottom-left
			 0.5f, -0.5f,  0.5f, // bottom-right
			 0.5f,  0.5f,  0.5f, // top-right
			 0.5f,  0.5f,  0.5f, // top-right
			-0.5f,  0.5f,  0.5f, // top-left
			-0.5f, -0.5f,  0.5f, // bottom-left
			// left face
			-0.5f,  0.5f,  0.5f, // top-right
			-0.5f,  0.5f, -0.5f, // top-left
			-0.5f, -0.5f, -0.5f, // bottom-left
			-0.5f, -0.5f, -0.5f, // bottom-left
			-0.5f, -0.5f,  0.5f, // bottom-right
			-0.5f,  0.5f,  0.5f, // top-right
			// right face
			 0.5f,  0.5f,  0.5f, // top-left
			 0.5f, -0.5f, -0.5f, // bottom-right
			 0.5f,  0.5f, -0.5f, // top-right         
			 0.5f, -0.5f, -0.5f, // bottom-right
			 0.5f,  0.5f,  0.5f, // top-left
			 0.5f, -0.5f,  0.5f, // bottom-left     
			// bottom face
			-0.5f, -0.5f, -0.5f, // top-right
			 0.5f, -0.5f, -0.5f, // top-left
			 0.5f, -0.5f,  0.5f, // bottom-left
			 0.5f, -0.5f,  0.5f, // bottom-left
			-0.5f, -0.5f,  0.5f, // bottom-right
			-0.5f, -0.5f, -0.5f, // top-right
			// top face
			-0.5f,  0.5f, -0.5f, // top-left
			 0.5f,  0.5f , 0.5f, // bottom-right
			 0.5f,  0.5f, -0.5f, // top-right     
			 0.5f,  0.5f,  0.5f, // bottom-right
			-0.5f,  0.5f, -0.5f, // top-left
			-0.5f,  0.5f,  0.5f, // bottom-left        
		};

		_mesh->setVertexBuffer(format, 36)->setVertexData(vertices);

		Shader zTestedShader;
		Shader::Defines defines;
		defines.addDefine("DECAL_ZTEST");
		zTestedShader.createProgram(Shader::Vertex, "resources/arcana/shaders/decal_vert.glsl");
		zTestedShader.createProgram(Shader::Fragment, "resources/arcana/shaders/decal_frag.glsl", defines);
		_zTestedDecalMaterial = new Material("zTestedDecalMaterial");
		_zTestedDecalMaterial->reference();
		Technique* zTestedTechnique = new Technique(zTestedShader);
		_zTestedDecalMaterial->addTechnique(zTestedTechnique);

		Shader shader;
		shader.createProgram(Shader::Vertex, "resources/arcana/shaders/decal_vert.glsl");
		shader.createProgram(Shader::Fragment, "resources/arcana/shaders/decal_frag.glsl");
		_decalMaterial = new Material("decalMaterial");
		_decalMaterial->reference();
		Technique* technique = new Technique(shader);
		_decalMaterial->addTechnique(technique);

		Properties.RendererStage = "DeferredDecalStage";
		Properties.LightProperties.CastsDynamicShadow = false;
		Properties.RenderState.setCullEnabled(true);//set renderstate
		Properties.RenderState.setCullFaceSide(RenderState::Back);
		Properties.RenderState.setDepthTestEnabled(false);
		Properties.RenderState.setDepthWriteEnabled(false);
		Properties.RenderState.setBlendEnabled(false);
	}

	DecalRenderProcedure::~DecalRenderProcedure()
	{
		AE_RELEASE(_mesh);
		AE_RELEASE(_decalMaterial);
		AE_RELEASE(_zTestedDecalMaterial);
	}

	void DecalRenderProcedure::render()
	{
		if (NumDecals == 0)
			return;

		//camera test

		bool cameraIntersection = false;

		Material* material = selectMaterial(!cameraIntersection);

		if (!material)
			return;

		Properties.RenderState.bind();

		if (_mesh)
		{
			_mesh->getInstanceProperties()._numInstances = NumDecals;

			std::vector<DecalVertex> instanceData; //change to array (faster)

			for (uint32 i = 0; i < NumDecals; i++)
			{
				Decal d = Decals[i];
				DecalVertex vertex;
				vertex.color = d.getColor().asLinear().toVector4();
				vertex.color.w = d.getOpacity();
				vertex.texCoords = d.getTexCoords();
				Matrix4f transform = d.getTransform().getMatrix().cast<float>();
				vertex.transform0.x = transform.at(0);
				vertex.transform0.y = transform.at(1);
				vertex.transform0.z = transform.at(2);
				vertex.transform0.w = transform.at(3);
				vertex.transform1.x = transform.at(4);
				vertex.transform1.y = transform.at(5);
				vertex.transform1.z = transform.at(6);
				vertex.transform1.w = transform.at(7);
				vertex.transform2.x = transform.at(8);
				vertex.transform2.y = transform.at(9);
				vertex.transform2.z = transform.at(10);
				vertex.transform2.w = transform.at(11);
				vertex.transform3.x = transform.at(12);
				vertex.transform3.y = transform.at(13);
				vertex.transform3.z = transform.at(14);
				vertex.transform3.w = transform.at(15);
				instanceData.push_back(vertex);
			}

			//std::sort(instanceData.begin(), instanceData.end(), [&](DecalVertex a, DecalVertex b)
			//{
			//	return Vector3f::distanceSq(a.position, update.eyePosition.cast<float>()) < Vector3f::distanceSq(b.position, update.eyePosition.cast<float>());
			//});

			_mesh->getInstanceBuffer()->bind();
			if (NumDecals)
			{
				glBufferData(GL_ARRAY_BUFFER, _instanceFormat.getVertexSize() * NumDecals, &instanceData[0].color.x, GL_DYNAMIC_DRAW);
			}
			else
			{
				glMapBuffer(GL_ARRAY_BUFFER, GL_MAP_INVALIDATE_BUFFER_BIT);
				_mesh->getInstanceBuffer()->unbind();
				return;
			}
			_mesh->getInstanceBuffer()->unbind();

			_mesh->getVertexBuffer()->bind();

			Mesh::InstanceProperties instanceProperties = _mesh->getInstanceProperties();

			uint32 componentCount = _mesh->getNumIndexComponents();

			if (componentCount == 0)
			{
				Technique* technique = material->getCurrentTechnique();
				if (technique)
				{
					for (uint32 i = 0; i < technique->getPassCount(); i++)
					{
						Shader* pass = technique->getPass(i);
						if (pass)
						{
							pass->bind();

							//Default Uniforms
							pass->getUniform("u_ProjectionMatrix").setValue(Projection.cast<float>());
							pass->getUniform("u_ViewMatrix").setValue(View.cast<float>());
							pass->getUniform("u_InverseViewMatrix").setValue(View.inverse().cast<float>());
							pass->getUniform("u_InverseProjectionMatrix").setValue(Projection.inverse().cast<float>());
							pass->getUniform("u_CameraPosition").setValue(EyePosition.cast<float>());

							pass->getUniform("u_FarClip").setValue(1000.0f);//temp
							pass->getUniform("u_Resolution").setValue(Vector2f(1920.0f, 1080.0f));//temp

							passDecalAttributes(pass);

							for (uint32 j = 0; j < Uniforms.size(); j++)
							{
								pass->getUniform(Uniforms[j].name).setValue(Uniforms[j].value);
							}

							glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
							if (instanceProperties.isInstanced())
							{
								_mesh->getInstanceBuffer()->bind();
								glDrawArraysInstanced(_mesh->getPrimitive(), 0, _mesh->getNumVertices(), instanceProperties.getNumInstances());
								_mesh->getInstanceBuffer()->unbind();
							}
							else
							{
								glDrawArrays(_mesh->getPrimitive(), 0, _mesh->getNumVertices());
							}

							pass->unbind();
						}
					}
				}
			}

			_mesh->getVertexBuffer()->unbind();
		}

		Properties.RenderState.unbind();
	}
	
	bool DecalRenderProcedure::isValidProcedure()
	{
		return _mesh && _zTestedDecalMaterial && _decalMaterial;
	}

	Material* DecalRenderProcedure::selectMaterial(bool zTest)
	{
		return zTest ? _zTestedDecalMaterial : _decalMaterial;
	}

	void DecalRenderProcedure::passDecalAttributes(Shader* shader)
	{
		if (shader)
		{
			shader->getUniform("u_Decal.angleCutoff").setValue(_properties._angleCutoff);
			shader->getUniform("u_Decal.color").setValue(_properties._color.asLinear().toVector4());

			int32 unit;
			if (_properties._normals)
			{
				unit = _properties._normals->bind();
				shader->getUniform("u_Decal.normals").setValue(unit);
				shader->getUniform("u_Decal.normalsBlendFactor").setValue(_properties._normalsBlend);
			}
			if (_properties._albedo)
			{
				unit = _properties._albedo->bind();
				shader->getUniform("u_Decal.albedo").setValue(unit);
				shader->getUniform("u_Decal.albedoBlendFactor").setValue(_properties._albedoBlend);
			}
			if (_properties._specular)
			{
				unit = _properties._specular->bind();
				shader->getUniform("u_Decal.specular").setValue(unit);
				shader->getUniform("u_Decal.specularBlendFactor").setValue(_properties._specularBlend);
			}
			if (_properties._roughness)
			{
				unit = _properties._roughness->bind();
				shader->getUniform("u_Decal.roughness").setValue(unit);
				shader->getUniform("u_Decal.roughnessBlendFactor").setValue(_properties._roughnessBlend);
			}
			if (_properties._emissive)
			{
				unit = _properties._emissive->bind();
				shader->getUniform("u_Decal.emissive").setValue(unit);
				shader->getUniform("u_Decal.emissiveBlendFactor").setValue(_properties._emissiveBlend);
			}
			if (_properties._metallic)
			{
				unit = _properties._metallic->bind();
				shader->getUniform("u_Decal.metallic").setValue(unit);
				shader->getUniform("u_Decal.metallicBlendFactor").setValue(_properties._metallicBlend);
			}
			if (_properties._ao)
			{
				unit = _properties._ao->bind();
				shader->getUniform("u_Decal.ao").setValue(unit);
				shader->getUniform("u_Decal.aoBlendFactor").setValue(_properties._aoBlend);
			}
		}
	}
}