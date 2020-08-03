#include "TestMenu.h"

#include "ScopeExit.h"
#include "imgui/imgui.h"


namespace test
{

    void Show(TestMenu &menu)
    {
        menu.CurrentTest().OnUpdate(0.0f);
        menu.CurrentTest().OnRender();

        {
            ImGui::Begin("Test");
            SCOPE_EXIT { ImGui::End(); };

            if (menu.HasCurrentTest() && ImGui::Button("<-"))
                menu.BackToMenu();

            menu.CurrentTest().OnImGuiRender();
        }
    }

    TestMenu::TestMenu()
        : pCurrent_(nullptr)
        , tests_()
    {
    }

    void TestMenu::OnUpdate(float deltaTime)
    {
    }

    void TestMenu::OnRender()
    {
    }

    void TestMenu::OnImGuiRender()
    {
        for(auto&& test : tests_)
        {
            if(ImGui::Button(test.Label().c_str()))
                pCurrent_ = test.MakeTest();
        }
    }

    Test& TestMenu::CurrentTest()
    {
        if(!HasCurrentTest())
            return *this;

        return *pCurrent_;
    }

    bool TestMenu::HasCurrentTest() const
    {
        return pCurrent_ != nullptr;
    }

    void TestMenu::BackToMenu()
    {
        assert(HasCurrentTest());
        pCurrent_.reset();
    }

}
