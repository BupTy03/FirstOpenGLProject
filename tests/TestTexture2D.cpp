#include "TestTexture2D.h"

#include "GLUtils.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

#include <imgui/imgui.h>

#include <GL/glew.h>

#include <string>


namespace test
{

    static constexpr std::array<float, 16> DEFAULT_POSITIONS = {
            -50.0f, -50.0f, 0.0f, 0.0f, // 0
            50, -50.0f, 1.0f, 0.0f, // 1
            50,  50, 1.0f, 1.0f, // 2
            -50.0f,  50, 0.0f, 1.0f  // 3
    };

    static constexpr std::array<unsigned int, 6> DEFAULT_INDICES = {
            0, 1, 2,
            2, 3, 0
    };

    static glm::mat4x4 ApplyTranslation(const glm::mat4x4& proj, const glm::mat4x4& view, const glm::vec3& translation)
    {
        const auto model = glm::translate(glm::mat4{1.0f}, translation);
        const auto mvp = proj * view * model;
        return mvp;
    }


    TestTexture2D::TestTexture2D()
        : Test()
        , vertexArray_()
        , indexBuffer_(std::data(DEFAULT_INDICES), std::size(DEFAULT_INDICES))
        , vertexBuffer_(std::data(DEFAULT_POSITIONS),std::size(DEFAULT_POSITIONS) * sizeof(float))
        , shader_("../resources/shaders/Basic.shader")
        , texture_("../resources/textures/batman_vs_superman.png")
        , proj_(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f))
        , view_(glm::translate(glm::mat4{1.0f}, glm::vec3{0, 0, 0}))
        , translationA_(200, 200, 0)
        , translationB_(400, 200, 0)
    {
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        vertexArray_.AddBuffer(vertexBuffer_, layout);

        shader_.Bind();
        shader_.SetUniform("u_Color", std::make_tuple(0.2f, 0.3f, 0.8f, 1.0f));
        shader_.SetUniform("u_Texture", 0);

        vertexArray_.Unbind();
        vertexBuffer_.Unbind();
        indexBuffer_.Unbind();
        shader_.Unbind();
    }

    void TestTexture2D::OnUpdate(float deltaTime)
    {
    }

    void TestTexture2D::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        texture_.Bind(0);

        shader_.SetUniform("u_MVP", ApplyTranslation(proj_, view_, translationA_));
        renderer.Draw(vertexArray_, indexBuffer_, shader_);

        shader_.SetUniform("u_MVP", ApplyTranslation(proj_, view_, translationB_));
        renderer.Draw(vertexArray_, indexBuffer_, shader_);
    }

    void TestTexture2D::OnImGuiRender()
    {
        ImGui::SliderFloat3("Translation A", &translationA_.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("Translation B", &translationB_.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

}
