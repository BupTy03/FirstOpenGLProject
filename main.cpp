#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "GLUtils.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <array>
#include <algorithm>


int main()
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
    {
        constexpr std::array<float, 16> positions = {
                -0.5f, -0.5f, 0.0f, 0.0f, // 0
                 0.5f, -0.5f, 1.0f, 0.0f, // 1
                 0.5f,  0.5f, 1.0f, 1.0f, // 2
                -0.5f,  0.5f, 0.0f, 1.0f  // 3
        };

        constexpr std::array<unsigned int, 6> indices = {
                0, 1, 2,
                2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        VertexBuffer vb(
                std::data(positions),
                std::size(positions) * sizeof(decltype(positions)::value_type)
        );
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(std::data(indices), std::size(indices));

        Shader shader("../resources/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform("u_Color", {0.2f, 0.3f, 0.8f, 1.0f});

        Texture texture("../resources/textures/batman_vs_superman.png");
        texture.Bind(0);
        shader.SetUniform("u_Texture", 0);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        constexpr float delta{0.005f};
        float r{};
        float increment{delta};

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            shader.Bind();
            shader.SetUniform("u_Color", {r, 0.3f, 0.8f, 1.0f});

            renderer.Draw(va, ib, shader);

            if (r > 1.0f) increment = -delta;
            else if (r < 0.0f) increment = delta;

            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}
