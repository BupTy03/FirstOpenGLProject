#pragma once

#include <memory>


namespace test
{
    class Test;
    using TestPtr = std::unique_ptr<Test>;

    class Test
    {
    public:
        Test() = default;
        virtual ~Test() noexcept = default;

        virtual void OnUpdate(float deltaTime) = 0;
        virtual void OnRender() = 0;
        virtual void OnImGuiRender() = 0;
    };
}
