#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <array>
#include <vector>
#include <string_view>
#include <algorithm>
#include <fstream>
#include <string>
#include <cassert>

#define GLCall(x) \
    GLClearAllErrors(); \
    x; \
    assert(GLCheckErrors())

static void GLClearAllErrors() { while(glGetError() != GL_NO_ERROR){}; }
static bool GLCheckErrors()
{
    GLenum error{GL_NO_ERROR};
    while((error = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "[OpenGL Error] (" << error << ")\n";
        return false;
    }

    std::cout.flush();
    return true;
}

static bool contains(const std::string& str, const std::string_view fragment)
{
    return str.find(fragment) != std::string::npos;
}

class ShaderProgramSource
{
public:
    explicit ShaderProgramSource(
            std::string vertexSource,
            std::string fragmentSource)
            : vertexSource_(std::move(vertexSource))
            , fragmentSource_(std::move(fragmentSource))
    {}

    [[nodiscard]]
    const std::string& VertexSource() const { return vertexSource_; }

    [[nodiscard]]
    const std::string& FragmentSource() const { return fragmentSource_; }

private:
    std::string vertexSource_;
    std::string fragmentSource_;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream file(filepath);

    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::string strs[2];
    ShaderType type = ShaderType::NONE;
    while(std::getline(file, line))
    {
        if(contains(line, "#shader"))
        {
            if(contains(line, "vertex"))
                type = ShaderType::VERTEX;
            else if(contains(line, "fragment"))
                type = ShaderType::FRAGMENT;
            else
                throw std::runtime_error("unknown shader type");
        }
        else
        {
            auto& str = strs[static_cast<int>(type)];
            str += line;
            str += '\n';
        }
    }

    return ShaderProgramSource(
            std::move(strs[static_cast<int>(ShaderType::VERTEX)]),
            std::move(strs[static_cast<int>(ShaderType::FRAGMENT)]));
}

static unsigned int CompileShader(unsigned int type, const std::string_view source)
{
    const auto id = glCreateShader(type);
    const char* src = std::data(source);
    const auto sourceLength = static_cast<int>(std::size(source));
    glShaderSource(id, 1, &src, &sourceLength);
    glCompileShader(id);

    int result{GL_FALSE};
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE)
    {
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
static unsigned int CreateShader(const std::string_view vertexShader, const std::string_view fragmentShader)
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


int main()
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if(glewInit() != GLEW_OK)
    {
        std::cout << "Error!" << std::endl;
        return -2;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    constexpr std::array<float, 8> positions = {
            -0.5f, -0.5f, // 0
             0.5f, -0.5f, // 1
             0.5f,  0.5f, // 2
            -0.5f,  0.5f  // 3
    };

    constexpr std::array<unsigned int, 6> indices = {
            0, 1, 2,
            2, 3, 0
    };

    unsigned int buffer{0};
    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER,
            std::size(positions) * sizeof(decltype(positions)::value_type),
            std::data(positions),
            GL_STATIC_DRAW));

    unsigned int indexBufferObj{0};
    GLCall(glGenBuffers(1, &indexBufferObj));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObj));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            std::size(indices) * sizeof(decltype(indices)::value_type),
            std::data(indices),
            GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr));

    const auto source = ParseShader("../resources/shaders/Basic.shader");
    const unsigned int shader = CreateShader(source.VertexSource(), source.FragmentSource());
    GLCall(glUseProgram(shader));

    GLCall(const auto location = glGetUniformLocation(shader, "u_Color"));
    assert(location != -1);
    GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));

    constexpr float delta{0.005f};
    float r{};
    float increment{delta};

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
        GLCall(glDrawElements(GL_TRIANGLES, std::size(indices), GL_UNSIGNED_INT, nullptr));

        if(r > 1.0f) increment = -delta;
        else if(r < 0.0f) increment = delta;

        r += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}
