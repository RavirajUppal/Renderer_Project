#include "ShaderClass.h"

std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return contents;
	}
	throw std::runtime_error("Failed to open file: " + std::string(filename));
}

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;

	std::string defineStr = "\n#define USE_GEOMETRY\n";

    try
    {
        vertexCode = get_file_contents(vertexPath);
        fragmentCode = get_file_contents(fragmentPath);
		if (geometryPath != nullptr)
		{
        	geometryCode = get_file_contents(geometryPath);
			size_t pos = vertexCode.find('\n');
			vertexCode.insert(pos + 1, defineStr);
		}
    }
    catch (const std::exception& e)
    {
        std::cerr << "Shader file loading error: " << e.what() << std::endl;
    }
	
	GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexCode.c_str());
	compileErrors(vertexShader, "VERTEX");
	
	GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());
	compileErrors(fragmentShader, "FRAGMENT");

	GLuint geometryShader = 0;
	if (!geometryCode.empty())
	{
		geometryShader = CompileShader(GL_GEOMETRY_SHADER, geometryCode.c_str());
		compileErrors(geometryShader, "GEOMETRY");
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	if (geometryShader != 0)
		glAttachShader(ID, geometryShader);
	glLinkProgram(ID);
	compileErrors(ID, "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (geometryShader != 0)
		glDeleteShader(geometryShader);
}

GLuint Shader::CompileShader(GLuint type, const char* source)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	return shader;
}

void Shader::Activate()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}

void Shader::compileErrors(unsigned int shader, const char *type)
{
	// Stores status of compilation
	GLint hasCompiled;
	// Character array to store error message in
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n"
					  << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n"
					  << infoLog << std::endl;
		}
	}
}

void Shader::SetVec3(const std::string &name, const glm::vec3 &value) const
{
	GLint loc = glGetUniformLocation(ID, name.c_str());
	if (loc != -1)
	{
		glUniform3fv(loc, 1, &value[0]);
	}
	else
	{
		std::cerr << "Warning: uniform '" << name << "' not found.\n";
	}
}

// Overload for individual floats
void Shader::SetFloat4(const std::string &name, float x, float y, float z, float w) const
{
	GLint loc = glGetUniformLocation(ID, name.c_str());
	if (loc != -1)
	{
		glUniform4f(loc, x, y, z, w);
	}
	else
	{
		std::cerr << "Warning: uniform '" << name << "' not found.\n";
	}
}

void Shader::SetFloat3(const std::string &name, float x, float y, float z) const
{
	GLint loc = glGetUniformLocation(ID, name.c_str());
	if (loc != -1)
	{
		glUniform3f(loc, x, y, z);
	}
	else
	{
		std::cerr << "Warning: uniform '" << name << "' not found.\n";
	}
}

void Shader::SetFloat1(const std::string &name, float x) const
{
	GLint loc = glGetUniformLocation(ID, name.c_str());
	if (loc != -1)
	{
		glUniform1f(loc, x);
	}
	else
	{
		std::cerr << "Warning: uniform '" << name << "' not found.\n";
	}
}

void Shader::SetInt1(const std::string &name, int x) const
{
	GLint loc = glGetUniformLocation(ID, name.c_str());
	if (loc != -1)
	{
		glUniform1i(loc, x);
	}
	else
	{
		std::cerr << "Warning: uniform '" << name << "' not found.\n";
	}
}

void Shader::SetMat4(const std::string &name, const float *matrix) const
{
    GLint loc = glGetUniformLocation(ID, name.c_str());
    if (loc != -1)
    {
        // Upload 4x4 matrix, no transpose (OpenGL expects column-major order)
        glUniformMatrix4fv(loc, 1, GL_FALSE, matrix);
    }
    else
    {
        std::cerr << "Warning: uniform '" << name << "' not found.\n";
    }
}

void Shader::SetVecOfMat4(const std::string &name, const std::vector<glm::mat4>& matrixVec) const
{
    GLint loc = glGetUniformLocation(ID, name.c_str());
    if (loc != -1)
    {
        // Upload 4x4 matrix, no transpose (OpenGL expects column-major order)
        glUniformMatrix4fv(loc, matrixVec.size(), GL_FALSE, &matrixVec[0][0][0]);
    }
    else
    {
        std::cerr << "Warning: uniform '" << name << "' not found.\n";
    }
}

void Shader::PrintActiveUniforms() const
{
    GLint count;
    glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &count);

    std::cout << "Active uniforms in shader " << ID << ":\n";
    for (GLint i = 0; i < count; ++i)
    {
        char name[256];
        GLsizei length;
        GLint size;
        GLenum type;

        glGetActiveUniform(ID, (GLuint)i, sizeof(name), &length, &size, &type, name);

        GLint location = glGetUniformLocation(ID, name);

        std::cout << "  #" << i
                  << " name=" << name
                  << " location=" << location
                  << " type=" << type
                  << " size=" << size
                  << "\n";
    }
}