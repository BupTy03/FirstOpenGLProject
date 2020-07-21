#include "Shader.h"
#include "GLUtils.h"

#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <vector>


Shader::Shader(std::string_view filename)
    : renderID_{0}
    , filePath_(filename.begin(), filename.end())
    , uniformLocationCache_()
{
    const auto source = ShaderImpl::ParseShader(filePath_);
    renderID_ = ShaderImpl::CreateShader(source.VertexSource(), source.FragmentSource());
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(renderID_));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(renderID_));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

int Shader::GetUniformLocation(const std::string& name) const
{
    auto foundIt = uniformLocationCache_.find(name);
    if (foundIt != uniformLocationCache_.cend()) return foundIt->second;

    GLCall(const auto location = glGetUniformLocation(renderID_, name.c_str()));
    if (location < 0)
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;

    uniformLocationCache_.emplace(name, location);
    return location;
}

void Shader::SetUniform(const std::string& name, int v)
{
    GLCall(glUniform1i(GetUniformLocation(name), v));
}

void Shader::SetUniform(const std::string& name, const std::tuple<float, float, float, float>& v)
{
    GLCall(glUniform4f(GetUniformLocation(name), std::get<0>(v), std::get<1>(v), std::get<2>(v), std::get<3>(v)));
}

void Shader::SetUniform(const std::string& name, const glm::mat4& m)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, std::data(m));
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


    ShaderProgramSource::ShaderProgramSource(std::string_view vertexSource, std::string_view fragmentSource)
        : vertexSource_(vertexSource.begin(), vertexSource.end())
        , fragmentSource_(fragmentSource.begin(), fragmentSource.end())
    {
    }

    const std::string& ShaderProgramSource::VertexSource() const
    {
        return vertexSource_;
    }

    const std::string& ShaderProgramSource::FragmentSource() const
    {
        return fragmentSource_;
    }
}
