#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <array>
#include <vector>
#include <string_view>
#include <algorithm>


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
    glBufferData(GL_ARRAY_BUFFER, std::size(positions), std::data(positions), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);

    const std::string_view vertexShader =
            "#version 440 core\n"
            "\n"
            "layout(location = 0) in vec4 position;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    gl_Position = position;\n"
            "}\n";

    const std::string_view fragmentShader =
            "#version 440 core\n"
            "\n"
            "layout(location = 0) out vec4 color;\n"
            "\n"
            "void main()\n"
            "{\n"
            "color = vec4(1.0, 1.0, 0.0, 1.0);\n"
            "}\n";

    glUseProgram(CreateShader(vertexShader, fragmentShader));

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

    glfwTerminate();
    return 0;
}
