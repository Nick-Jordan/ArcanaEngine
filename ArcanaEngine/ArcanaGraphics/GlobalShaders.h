#ifndef GLOBAL_SHADERS_H_
#define GLOBAL_SHADERS_H_

#include "GraphicsDefines.h"  //should be in graphics?

#include "Shader.h"

#include <map>

namespace Arcana
{

	class ARCANA_GRAPHICS_API GlobalShaders
	{
	public:

		enum ID
		{
			//add global shader ids
			Default = 0,
			GaussianBlur,
			DeferredLighting,
			FinalHDR,
			FTLShadowDepthMapping,
			FTLPointShadowDepthMapping,
			Particles,
			MeshParticles,
			NUM_DEFAULT_GLOBAL_SHADERS
		};

		static void registerShader(ID id, const Shader& shader);  //const shader?

		static void registerShader(const std::string& name, const Shader& shader);

		static Shader* get(ID id);

		static Shader* get(const std::string& name);

		static bool initialize();

		static std::string getDefaultShaderName(ID id);

	private:

		static bool attemptLoadDefaultShader(ID id);

	private:

		static std::map<ID, Shader> __defaultGlobalShaders;
		static std::map<std::string, Shader> __userDefinedGlobalShaders;
	};
}

#endif // !GLOBAL_SHADERS_H_