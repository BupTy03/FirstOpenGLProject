#include "TestClearColor.h"

#include "GLUtils.h"

#include "imgui/imgui.h"
#include <GL/glew.h>


namespace test
{

    static constexpr std::array<float, 4> DEFAULT_COLOR = {0.2f, 0.3f, 0.8f, 1.0f};


    TestClearColor::TestClearColor() : Test(), clearColor_{DEFAULT_COLOR}
    {
    }

    void TestClearColor::OnUpdate(float deltaTime)
    {
    }

    void TestClearColor::OnRender()
    {
        GLCall(glClearColor(clearColor_.at(0), clearColor_.at(1), clearColor_.at(2), clearColor_.at(3)));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }

    void TestClearColor::OnImGuiRender()
    {
        ImGui::ColorEdit4("Clear Color", std::data(clearColor_));
    }



}
