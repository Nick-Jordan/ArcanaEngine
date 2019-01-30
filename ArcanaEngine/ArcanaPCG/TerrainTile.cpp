#include "TerrainTile.h"

namespace Arcana
{
	TerrainTileProceduralParameters::TerrainTileProceduralParameters(TerrainQuad* quad, Matrix4d transformation, double radius)
		: _quad(quad), _transformation(transformation), _radius(radius)
	{

	}

	void TerrainTileProceduralParameters::setUniforms(Shader& shader) const
	{
		shader.getUniform("u_QuadOffset")->setValue(Vector4f(_quad->getPhysicalXCoordinate(), _quad->getPhysicalYCoordinate(), _quad->getPhysicalLevel(), _quad->getLevel()));

		shader.getUniform("u_Transformation")->setValue(_transformation.cast<float>());

		if (_radius > 0.0)
		{
			shader.getUniform("u_Radius")->setValue((float)_radius);
		}
	}



	TerrainTile::TerrainTile() 
		: ProceduralObject("TerrainTile", "terrain_tile")
	{
		//test
		Shader shader;
		shader.createProgram(Shader::Vertex, "resources/height_shader_vert.glsl");
		shader.createProgram(Shader::Fragment, "resources/height_shader_frag.glsl");

		_heightData = new TextureProceduralData(shader, Texture::Red, 101, 101, Texture::R32F, Texture::Float);
		_heightData->reference();
		registerData(_heightData);
	}

	TerrainTile::~TerrainTile()
	{

	}

	Texture* TerrainTile::getHeightTexture()
	{
		if (_heightData)
		{
			return _heightData->getTexture();
		}
		return nullptr;
	}
}

