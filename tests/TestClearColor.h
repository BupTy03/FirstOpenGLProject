#pragma once

#include "Test.h"
#include <array>


namespace test
{

    class TestClearColor : public Test
    {
    public:
        TestClearColor();
        ~TestClearColor() override;

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        std::array<float, 4> clearColor_;
    };

}
