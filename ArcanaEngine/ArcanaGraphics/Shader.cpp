#include "Shader.h"

#include "ResourceManager.h"
#include "ResourceCreator.h"

//temporary tolower
#include <algorithm>

namespace Arcana
{
	INITIALIZE_CATEGORY(Arcana, ShaderLog)

		Shader::Shader() : Object("Shader")
	{
		initialize();
	}

	Shader::Shader(const Shader& shader) : Object("Shader"), _id(shader._id)
	{
		_programs.reset();
		for (auto i = shader._programs.createConstIterator(); i; i++)
		{
			_programs.add(*i);
		}
	}

	Shader::~Shader()
	{
	}

	void Shader::createProgram(Type type, const std::string& file, Defines defines)
	{
		Program p;
		p.type = type;
		p.file = file;
		_programs.add(p);

		GLuint shader = glCreateShader(type);

		const char* source = readSource(file, defines);

		GLchar const* source_c[] = { source };
		glShaderSource(shader, 1, source_c, NULL);

		glCompileShader(shader);

		glAttachShader(_id, shader);

		glLinkProgram(_id);


		GLint compileDebug;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compileDebug);
		if (compileDebug == GL_FALSE)
		{
			int length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

			char* log = new char[length];
			glGetShaderInfoLog(shader, length, 0, log);
			LOGF(Error, ShaderLog, "COMPILE Error: %s\n", log);
			AE_DELETE(log);
		}

		GLint linkDebug;
		glGetProgramiv(_id, GL_LINK_STATUS, &linkDebug);
		if (linkDebug == GL_FALSE)
		{
			int length;
			glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &length);

			char* log = new char[length];
			glGetProgramInfoLog(_id, length, 0, log);
			LOGF(Error, ShaderLog, "LINK Error: %s\n", log);
			AE_DELETE(log);
		}

		glDeleteShader(shader);

		AE_DELETE(source);
	}

	GLuint Shader::getId() const
	{
		return _id;
	}

	SmartPtr<Uniform> Shader::getUniform(const std::string& name)
	{
		return Uniform::get(this, name);
	}

	void Shader::bind()
	{
		glUseProgram(_id);
	}

	void Shader::unbind()
	{
		glUseProgram(0);
	}

	Shader& Shader::operator=(const Shader& shader)
	{
		_id = shader._id;

		_programs.reset();
		for (auto i = shader._programs.createConstIterator(); i; i++)
		{
			_programs.add(*i);
		}

		return *this;
	}

	Shader::Type Shader::getProgramType(const std::string& string)
	{
		std::string type = string;

		//temporary tolower
		std::transform(type.begin(), type.end(), type.begin(), ::tolower);

		if (type == "vertex")
		{
			return Vertex;
		}
		else if (type == "fragment")
		{
			return Fragment;
		}
		else if (type == "geometry")
		{
			return Geometry;
		}
		else if (type == "compute")
		{
			return Compute;
		}
		else if (type == "tessellationcontrol" || type == "tesscontrol")
		{
			return TessControl;
		}
		else if (type == "tessellationevaluation" || type == "tessevaluation")
		{
			return TessEvaluation;
		}
		
		return Unknown;
	}


	void Shader::initialize()
	{
		_id = glCreateProgram();
	}

	char* Shader::readSource(const std::string& file, Defines defines)
	{
		std::string content;
		std::ifstream fileStream(file, std::ios::in);

		if (!fileStream.is_open()) {
			LOGF(Error, ShaderLog, "Could not read file %s. File does not exist.", file.c_str());
			return "";
		}

		bool firstFinished = false;
		bool definesDone = false;

		std::string line = "";
		while (!fileStream.eof()) 
		{
			if (firstFinished && !definesDone)
			{
				definesDone = true;

				if (defines.getNumDefines() > 0)
				{
					//for (auto i = defines._values.createConstIterator(); i; i++)
					std::vector<Shader::Defines::Value>::iterator i;
					for (i = defines._values.begin(); i != defines._values.end(); i++)
					{
						content.append("#define " + (*i).name + " " + (*i).value + "\n");
					}
				}
			}

			std::getline(fileStream, line);

			if (line.find("#include") != std::string::npos && line.find("//") == std::string::npos)
			{
				unsigned first = line.find("\"");
				unsigned last = line.find_last_of("\"");
				std::string include = line.substr(first + 1, last - first - 1);
				LOGF(Info, CoreEngine, "include: %s", include.c_str());
				const char* source = readSource(include, Defines());
				content.append(std::string(source) + "\n");
				AE_DELETE(source);
			}
			else
			{
				content.append(line + "\n");
			}

			firstFinished = true;
		}

		fileStream.close();

		char* r = new char[content.length() + 1];
		strcpy(r, content.c_str());

		return r;
	}


	//Defines

	void Shader::Defines::addDefine(std::string name)
	{
		Value v;
		v.name = name;
		v.value = "";
		_values.push_back(v);
	}

	void Shader::Defines::addDefine(std::string name, std::string value)
	{
		Value v;
		v.name = name;
		v.value = value;
		_values.push_back(v);
	}

	uint32 Shader::Defines::getNumDefines() const
	{
		return _values.size();
	}


	class ShaderResource : public ResourceCreator<Shader>
	{
	public:

		ShaderResource(const std::string& name, const std::string& type, const ResourceData& data)
			: ResourceCreator<Shader>(name, type, data)
		{
			std::vector<ResourceDataPoint>::const_iterator iter;
			for (iter = data.getDataPoints().begin(); iter != data.getDataPoints().end(); iter++)
			{
				const ResourceDataPoint& dataPoint = *iter;

				if (dataPoint.hasResourceData)
				{
					if (dataPoint.name == "program")
					{
						const ResourceData& dataPointResourceData = dataPoint.resourceData;

						Shader::Type programType = Shader::getProgramType(dataPointResourceData.getStringParameter("type"));
						std::string source = dataPointResourceData.getStringParameter("source");

						if (programType != Shader::Unknown && source.size() > 0)
						{
							createProgram(programType, source);
						}
					}
				}
			}
		}
	};

	Resource::Type<ShaderResource> passResource("pass");
	Resource::Type<ShaderResource> shaderResource("shader");
}