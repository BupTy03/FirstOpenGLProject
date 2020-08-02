#include "TestMenu.h"

#include "imgui/imgui.h"


namespace test {

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
