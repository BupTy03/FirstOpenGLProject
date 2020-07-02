#pragma once

#include <string>
#include <string_view>
#include <unordered_map>


namespace ShaderImpl
{

    class ShaderProgramSource
    {
    public:
        explicit ShaderProgramSource(
                std::string_view vertexSource,
                std::string_view fragmentSource)
                : vertexSource_(vertexSource.begin(), vertexSource.end())
                , fragmentSource_(fragmentSource.begin(), fragmentSource.end())
        {}

        [[nodiscard]]
        const std::string& VertexSource() const { return vertexSource_; }

        [[nodiscard]]
        const std::string& FragmentSource() const { return fragmentSource_; }

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

    void Bind();
    void Unbind();

    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
    unsigned int GetUniformLocation(const std::string& name);

private:
    unsigned int m_RenderID;
    std::string m_FilePath;
    std::unordered_map<std::string, int> m_UniformLocationCache;
};
