#include "Shader.h"

#include "ResourceManager.h"
#include "ResourceCreator.h"

#include "StringUtils.h"

namespace Arcana
{
	INITIALIZE_CATEGORY(Arcana, ShaderLog)

		GLuint Shader::CurrentProgram = 0;

	Shader::Shader() : Object("Shader"), _initialized(false)
	{
	}

	Shader::Shader(const Shader& shader) : Object("Shader"), _id(shader._id), _initialized(shader._initialized)
	{
		/*_programs.clear();
		for (std::vector<Program>::const_iterator i = shader._programs.begin();
			i != shader._programs.end(); i++)
		{
			_programs.push_back(*i);
		}*/
	}

	Shader::~Shader()
	{
	}

	bool Shader::createProgram(Type type, const std::string& file, Defines defines, bool link)
	{
		const char* source = readSource(file, defines);

		return createProgramFromSource(type, source, defines, link);
	}

	bool Shader::createProgramFromSource(Type type, const char* source, Defines defines, bool link)
	{
		if (!source)
		{
			return false;
		}

		initialize();

		/*Program p;
		p.type = type;
		p.file = file;
		_programs.push_back(p);*/

		GLuint shader = glCreateShader(type);

		GLchar const* source_c[] = { source };
		glShaderSource(shader, 1, source_c, NULL);

		glCompileShader(shader);

		glAttachShader(_id, shader);

		if (link)
		{
			glLinkProgram(_id);
		}


		GLint compileDebug;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compileDebug);
		if (compileDebug == GL_FALSE)
		{
			int length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

			char* log = new char[length];
			glGetShaderInfoLog(shader, length, 0, log);
			LOGF(Error, ShaderLog, "COMPILE Error: %s\n", log);
			AE_DELETE_ARRAY(log);
			return false;
		}

		if (link)
		{
			GLint linkDebug;
			glGetProgramiv(_id, GL_LINK_STATUS, &linkDebug);
			if (linkDebug == GL_FALSE)
			{
				int length;
				glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &length);

				char* log = new char[length];
				glGetProgramInfoLog(_id, length, 0, log);
				LOGF(Error, ShaderLog, "LINK Error: %s\n", log);
				AE_DELETE_ARRAY(log);
				return false;
			}
		}

		glDeleteShader(shader);

		AE_DELETE(source);

		return true;
	}

	GLuint Shader::getId() const
	{
		return _id;
	}

	Uniform Shader::getUniform(const std::string& name)
	{
		GLint location = glGetUniformLocation(_id, name.c_str());

		return Uniform(this, location);
	}

	void Shader::bind()
	{
		if (_id != CurrentProgram)
		{
			glUseProgram(_id);
			CurrentProgram = _id;
		}
	}

	void Shader::unbind()
	{
		if (CurrentProgram)
		{
			glUseProgram(0);
			CurrentProgram = 0;
		}
	}

	Shader& Shader::operator=(const Shader& shader)
	{
		_id = shader._id;
		_initialized = shader._initialized;

		/*_programs.clear();
		for (std::vector<Program>::const_iterator i = shader._programs.begin();
			i != shader._programs.end(); i++)
		{
			_programs.push_back(*i);
		}*/

		return *this;
	}

	Shader::Type Shader::getProgramType(const std::string& string)
	{
		std::string type = StringUtils::toLower(string);

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
		if (_initialized)
			return;

		_id = glCreateProgram();
		_initialized = true;
	}

	char* Shader::readSource(const std::string& file, Defines defines)
	{
		std::string content;
		std::ifstream fileStream(file, std::ios::in);

		if (!fileStream.is_open()) {
			LOGF(Error, ShaderLog, "Could not read file %s. File does not exist.", file.c_str());
			return nullptr;
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

		ShaderResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data)
			: ResourceCreator<Shader>(id, type, data)
		{
			for (auto iter = data.getAdditionalData().begin(); iter != data.getAdditionalData().end(); iter++)
			{
				auto dataPoint = *iter;

				if (dataPoint.key == "program")
				{
					const ResourceData& dataPointResourceData = dataPoint.value;

					Shader::Type programType = Shader::getProgramType(dataPointResourceData.getStringParameter("type"));
					std::string file = dataPointResourceData.getStringParameter("source");

					//defines

					const char* source = readSource(file, Defines());
					
					if (programType != Shader::Unknown && source)
					{
						programs.add(MakePair(programType, source));
					}
				}
			}
		}

		virtual void syncInitialize() override
		{
			for (auto i = programs.createConstIterator(); i; i++)
			{
				Shader::Type programType = (*i).key;
				const char* source = (*i).value;
				createProgramFromSource(programType, source);
			}
		}

	private:

		Array<KeyValuePair<Shader::Type, const char*>> programs;
	};

	Resource::Type<ShaderResource> passResource("pass");
	Resource::Type<ShaderResource> shaderResource("shader");
}