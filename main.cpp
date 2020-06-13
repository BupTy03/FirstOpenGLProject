#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <array>
#include <vector>
#include <string_view>
#include <algorithm>
#include <fstream>
#include <string>


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

    if(glewInit() != GLEW_OK)
    {
        std::cout << "Error!" << std::endl;
        return -2;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    constexpr std::array positions = {
            -0.5f, -0.5f,
            0.0f, 0.5f,
            0.5f, -0.5f
    };

    unsigned int buffer{0};
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, std::size(positions) * sizeof(float), std::data(positions), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);

    const auto source = ParseShader("../resources/shaders/Basic.shader");
    unsigned int shader = CreateShader(source.VertexSource(), source.FragmentSource());
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}
