#ifndef SHADER_H_
#define SHADER_H_

#include "GraphicsDefines.h"

#include "ArcanaLog.h"
#include "Array.h"
#include "CoreDefines.h"
#include "Types.h"
#include "Uniform.h"
#include "../Dependencies/include/opengl/include.h"
#include <string>

namespace Arcana
{
	
	REGISTER_CATEGORY(ShaderLog, none)
		
	class ARCANA_GRAPHICS_API Shader
	{
	public:
			
		enum Type
		{
			Vertex = GL_VERTEX_SHADER,
			Fragment = GL_FRAGMENT_SHADER,
			Geometry = GL_GEOMETRY_SHADER,
			Compute = GL_COMPUTE_SHADER,
			TessControl = GL_TESS_CONTROL_SHADER,
			TessEvaluation = GL_TESS_EVALUATION_SHADER,
			
			NUM_SHADER_TYPES
		};
		
		class ARCANA_GRAPHICS_API Program
		{
		public:
			
			Type type;
			
			std::string file;
		};
		
		class ARCANA_GRAPHICS_API Defines
		{
			friend class Shader;
			
		public:
			
			void addDefine(std::string name);
			
			void addDefine(std::string name, std::string value);
			
			uint32 getNumDefines() const;
			
		private:
		
			struct Value
			{
				std::string name;
				std::string value;
			};
		
		private:
		
			Array<Value> _values;
		};
		
		
		Shader();
		
		Shader(const Shader& shader);

		~Shader();
		
		
		/*void create(const std::string& vertex);
		
		void create(const std::string& vertex, const std::string& fragment);
		
		void create(const std::string& vertex, const std::string& fragment, const std::string& geometry);
		
		void create(const std::string& vertex, const std::string& fragment, const std::string& tessControl, const std::string& tessEval);
		
		void create(const std::string& vertex, const std::string& fragment, const std::string& geometry, const std::string& tessControl, const std::string& tessEval);

		void createCompute(const std::string& compute);*/
		
		void createProgram(Type type, const std::string& file, Defines defines = Defines());

		GLuint getId() const;

		SmartPtr<Uniform> getUniform(const std::string& name);

		void bind();

		void unbind();


		Shader& operator=(const Shader& shader);
		
	private:

		void initialize();
		
		char* readSource(const std::string& file, Defines defines);
	
	private:
	
		Array<Program> _programs;
		
		GLuint _id;
	};
}

#endif // !SHADER_H_