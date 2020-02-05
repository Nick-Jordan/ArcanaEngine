#ifndef RENDER_PROCEDURE_H_
#define RENDER_PROCEDURE_H_

#include "GraphicsDefines.h"

#include "Object.h"
#include "FTLResult.h"
#include "LightProperties.h"
#include "Uniform.h"

//test
#include "Transform.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API RenderProcedure : public Object
	{
	public:

		struct UniformParameter
		{
			UniformParameter() {};
			~UniformParameter() {};

			UniformParameter& operator=(const UniformParameter& param)
			{
				name = param.name;
				value = param.value;
				return *this;
			};

			std::string name;

			Uniform::Value value;
		};

		struct RenderProperties
		{
			RenderState RenderState;
			std::string RendererStage;
			LightProperties LightProperties;
		};

		Matrix4d View;
		Matrix4d Projection;
		Vector3d EyePosition;
		Transform Transform;
		FTLResult FTLResult;
		RenderProperties Properties;
		std::vector<UniformParameter> Uniforms;

		virtual ~RenderProcedure() {};

		virtual void render() = 0;

		virtual void renderWithShader(const Shader& shader, bool bindRenderState) {};

		virtual bool isValidProcedure() = 0;
	};

}

#endif // !RENDER_PROCEDURE_H_

