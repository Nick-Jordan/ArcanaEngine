#include "GlobalShaders.h"

namespace Arcana
{
	std::map<GlobalShaders::ID, Shader> GlobalShaders::__defaultGlobalShaders;
	std::map<std::string, Shader> GlobalShaders::__userDefinedGlobalShaders;

	void GlobalShaders::registerShader(ID id, const Shader& shader)
	{
		std::map<ID, Shader>::iterator iter = __defaultGlobalShaders.find(id);
		if(iter != __defaultGlobalShaders.end())
		{
			LOGF(Warning, CoreEngine, "Default global shader, \'%s\', replaced!", getDefaultShaderName(id).c_str());
		}
	
		__defaultGlobalShaders[id] = shader;
	}

	void GlobalShaders::registerShader(const std::string& name, const Shader& shader)
	{
		std::map<std::string, Shader>::iterator iter = __userDefinedGlobalShaders.find(name);
		if(iter != __userDefinedGlobalShaders.end())
		{
			LOGF(Warning, CoreEngine, "User-defined global shader, \'%s\', replaced!", name.c_str());
		}
		
		__userDefinedGlobalShaders[name] = shader;
	}

	Shader* GlobalShaders::get(ID id)
	{
		std::map<ID, Shader>::iterator iter = __defaultGlobalShaders.find(id);

		if(iter != __defaultGlobalShaders.end())
		{
			return &(*iter).second;
		}

		return nullptr;
	}

	Shader* GlobalShaders::get(const std::string& name)
	{
		std::map<std::string, Shader>::iterator iter = __userDefinedGlobalShaders.find(name);

		if(iter != __userDefinedGlobalShaders.end())
		{
			return &(*iter).second;
		}

		return nullptr;
	}

	bool GlobalShaders::initialize()
	{
		bool value = true;

		for(uint32 i = Default; i < NUM_DEFAULT_GLOBAL_SHADERS; i++)
		{
			if(!get((ID)i))
			{
				LOGF(Warning, CoreEngine, "Default global shader, \'%s\', not defined; attempting to load from default file...", getDefaultShaderName((ID)i).c_str());
				value &= attemptLoadDefaultShader((ID)i);
			}
		}

		return value;
	}

	std::string GlobalShaders::getDefaultShaderName(ID id)
	{
		static std::string names[NUM_DEFAULT_GLOBAL_SHADERS] =
		{
			"Default",
			"Gaussian Blur",
			"Deferred Lighting",
			"Final HDR",
			"FTL Shadow Depth Mapping",
			"FTL Point Shadow Depth Mapping",
			"Particles",
			"Mesh Particles"
		};

		if(id != NUM_DEFAULT_GLOBAL_SHADERS)
		{
			return names[(uint32)id];
		}

		return "null";
	}

	bool GlobalShaders::attemptLoadDefaultShader(ID id)
	{
		Shader shader;

		//change this?
		std::string directory = "resources/arcana/shaders/";

		if(id == Default)
		{
			//change default resource folder?
			if(!shader.createProgram(Shader::Vertex, directory + "default_vert.glsl"))
				return false;
			if(!shader.createProgram(Shader::Fragment, directory + "default_frag.glsl"))
				return false;
		}
		else if(id == GaussianBlur)
		{
			if(!shader.createProgram(Shader::Vertex, directory + "quad_vert.glsl"))
				return false;
			if(!shader.createProgram(Shader::Fragment, directory + "blur_frag.glsl"))
				return false;
		}
		else if(id == DeferredLighting)
		{
			Shader::Defines defines;
			defines.addDefine("MAX_LIGHTS", "16");

			if(!shader.createProgram(Shader::Vertex, directory + "quad_vert.glsl"))
				return false;
			if(!shader.createProgram(Shader::Fragment, directory + "deferred_stage_frag.glsl", defines))
				return false;
		}
		else if (id == FinalHDR)
		{
			if (!shader.createProgram(Shader::Vertex, directory + "quad_vert.glsl"))
				return false;
			if (!shader.createProgram(Shader::Fragment, directory + "final_hdr_frag.glsl"))
				return false;
		}
		else if (id == FTLShadowDepthMapping)
		{
			if (!shader.createProgram(Shader::Vertex, directory + "ftl/shadow_mapping_depth_vert.glsl"))
				return false;
			if (!shader.createProgram(Shader::Fragment, directory + "ftl/shadow_mapping_depth_frag.glsl"))
				return false;
		}
		else if (id == FTLPointShadowDepthMapping)
		{
			if (!shader.createProgram(Shader::Vertex, directory + "ftl/point_shadow_mapping_depth_vert.glsl"))
				return false;
			if (!shader.createProgram(Shader::Geometry, directory + "ftl/point_shadow_mapping_depth_geom.glsl", Shader::Defines(), false))
				return false;
			if (!shader.createProgram(Shader::Fragment, directory + "ftl/point_shadow_mapping_depth_frag.glsl"))
				return false;
		}
		else if (id == Particles)
		{
			if (!shader.createProgram(Shader::Vertex, directory + "particle_vert.glsl"))
				return false;
			if (!shader.createProgram(Shader::Fragment, directory + "particle_frag.glsl"))
				return false;
		}
		else if (id == MeshParticles)
		{
			if (!shader.createProgram(Shader::Vertex, directory + "mesh_particle_vert.glsl"))
				return false;
			if (!shader.createProgram(Shader::Fragment, directory + "mesh_particle_frag.glsl"))
				return false;
		}

		registerShader(id, shader);

		LOGF(Info, CoreEngine, "Default global shader, \'%s\', loaded successfully!", getDefaultShaderName(id).c_str());

		return true;
	}
}