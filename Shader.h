#pragma once

#include "glm/glm.hpp"

#include <tuple>
#include <string>
#include <string_view>
#include <unordered_map>


namespace ShaderImpl
{

    class ShaderProgramSource
    {
    public:
        explicit ShaderProgramSource(std::string_view vertexSource, std::string_view fragmentSource);

        [[nodiscard]] const std::string& VertexSource() const;
        [[nodiscard]] const std::string& FragmentSource() const;

    private:
        std::string vertexSource_;
        std::string fragmentSource_;
    };

    ShaderProgramSource ParseShader(const std::string& filepath);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

}

class Shader
{
public:
    explicit Shader(std::string_view filename);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetUniform(const std::string& name, int v);
    void SetUniform(const std::string& name, const std::tuple<float, float, float, float>& v);
    void SetUniform(const std::string& name, const glm::mat4& m);

private:
    int GetUniformLocation(const std::string& name) const;

private:
    unsigned int renderID_;
    std::string filePath_;
    mutable std::unordered_map<std::string, int> uniformLocationCache_;
};
