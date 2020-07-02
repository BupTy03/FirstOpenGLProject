#include "Shader.h"
#include "Renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <vector>


Shader::Shader(std::string_view filename)
    : m_RenderID{0}
    , m_FilePath(filename.begin(), filename.end())
{
    const auto source = ShaderImpl::ParseShader(m_FilePath);
    m_RenderID = ShaderImpl::CreateShader(source.VertexSource(), source.FragmentSource());
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RenderID));
}

void Shader::Bind()
{
    GLCall(glUseProgram(m_RenderID));
}

void Shader::Unbind()
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
    auto foundIt = m_UniformLocationCache.find(name);
    if(foundIt != m_UniformLocationCache.cend()) return foundIt->second;

    GLCall(const auto location = glGetUniformLocation(m_RenderID, name.c_str()));
    if(location < 0)
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;

    m_UniformLocationCache.emplace(name, location);
    return location;
}


namespace ShaderImpl {

    ShaderProgramSource ParseShader(const std::string &filepath)
    {
        std::ifstream file(filepath);

        enum class ShaderType {
            NONE = -1,
            VERTEX = 0,
            FRAGMENT = 1
        };

        std::string line;
        std::string strs[2];
        ShaderType type = ShaderType::NONE;
        while (std::getline(file, line)) {
            if (contains(line, "#shader")) {
                if (contains(line, "vertex"))
                    type = ShaderType::VERTEX;
                else if (contains(line, "fragment"))
                    type = ShaderType::FRAGMENT;
                else
                    throw std::runtime_error("unknown shader type");
            } else {
                auto &str = strs[static_cast<int>(type)];
                str += line;
                str += '\n';
            }
        }

        return ShaderProgramSource(
                std::move(strs[static_cast<int>(ShaderType::VERTEX)]),
                std::move(strs[static_cast<int>(ShaderType::FRAGMENT)]));
    }

    unsigned int CompileShader(unsigned int type, const std::string &source)
    {
        const auto id = glCreateShader(type);
        const char *src = source.c_str();
        const auto sourceLength = static_cast<int>(std::size(source));
        glShaderSource(id, 1, &src, &sourceLength);
        glCompileShader(id);

        int result{GL_FALSE};
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            int length{0};
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

            std::vector<char> message(length);
            glGetShaderInfoLog(id, length, &length, std::data(message));
            std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << '\n';
            std::copy(std::cbegin(message), std::cend(message), std::ostreambuf_iterator<char>(std::cout));
            std::cout << std::endl;
            glDeleteShader(id);
            return 0;
        }

        return id;
    }

    unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
    {
        const auto program = glCreateProgram();
        const auto vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
        const auto fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
    }

}
